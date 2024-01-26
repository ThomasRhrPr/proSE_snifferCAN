/**
 * @file sender.c
 * 
 * @brief Fichier source du module sender qui permet d'envoyer les trames CAN sur le bus.
 * 
 * @version 0.2.0
 * 
 * @date 09/05/2023
 * 
 * @author Elisa DECLERCK
 *
 * @copyright © Copyright Équipe ProSE B1 2024 CANvengers
 * 
 * \b License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*****************************\
|*	Déclaration des includes *|
\*****************************/

#include <pthread.h>
#include <mqueue.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#include "sender.h"
#include "../driverCAN/driverCAN.h"
#include "../proxyGUI/proxyGUI.h"
#include "../types.h"

/****************************\
|*	Déclaration des defines *|
\****************************/

#define MQ_NAME ("/mq_sender")
#define MQ_MSG_MAX (10)

#define FRAME_MAX (10)
#define TIMER_DELAY_NS (5000000)
#define SEM_NAME ("/sem_send")

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/
typedef enum
{   
    STATE_FORGET = 0,
    STATE_IDLE,
    STATE_SENDING,
    STATE_DEATH,
    STATE_COUNT
} State_e;

typedef enum
{
    EVENT_START_SENDING = 0,
    EVENT_STOP_SENDING,
    EVENT_ASK_SENDING_STATE,
    EVENT_STOP,
    EVENT_COUNT
} Event_e;

typedef enum
{
    ACTION_SENDING = 0,
    ACTION_STOP_SENDING,
    ACTION_SET_SENDING_STATE_IDLE,
    ACTION_SET_SENDING_STATE_SENDING,
    ACTION_STOP,
    ACTION_COUNT
} Action_e;

typedef struct 
{
    State_e next_state;
    Action_e action;
} Transition_t;

typedef struct 
{
    Event_e event;
    uint8_t nbFrames;
    Frame_t frames[FRAME_MAX+1]; /* Patch pour que l'envoi fonctionne dans tous les cas (cause inconnue)*/
} MsgQ_t;

typedef union 
{
    MsgQ_t msgQ;
    char buffer[sizeof(MsgQ_t)];
} MsgQ_u;

typedef ErrorState_e (*actionSending)(MsgQ_t p_msgQ);

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static ErrorState_e sendFrame(MsgQ_t p_msgQ);

static ErrorState_e stopSendFrame(MsgQ_t p_msgQ);

static ErrorState_e setSendingStateIdle(MsgQ_t p_msgQ);

static ErrorState_e setSendingStateSending(MsgQ_t p_msgQ);

static ErrorState_e doNothing(MsgQ_t p_msgQ);

static void *run(void *p_arg_pt);

static void *send(void *p_arg_pt);

static void sendCallback(union sigval p_value);

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

static Frame_t s_framesToSend[FRAME_MAX];

static mqd_t s_myMq = {0};

static pthread_t s_myThreadSender = {0};

static pthread_t s_myThreadSending = {0};

static timer_t s_myTimer = {0};

static sem_t *s_mySem = NULL;

static Transition_t s_transition[STATE_COUNT][EVENT_COUNT] = 
{
    [STATE_IDLE][EVENT_START_SENDING] = {STATE_SENDING, ACTION_SENDING},
    [STATE_IDLE][EVENT_ASK_SENDING_STATE] = {STATE_IDLE, ACTION_SET_SENDING_STATE_IDLE},
    [STATE_IDLE][EVENT_STOP] = {STATE_DEATH, ACTION_STOP},

    [STATE_SENDING][EVENT_STOP_SENDING] = {STATE_IDLE, ACTION_STOP_SENDING},
    [STATE_SENDING][EVENT_ASK_SENDING_STATE] = {STATE_SENDING, ACTION_SET_SENDING_STATE_SENDING},
    [STATE_SENDING][EVENT_STOP] = {STATE_DEATH, ACTION_STOP}
};

static actionSending s_actionSending[ACTION_COUNT] = 
{
    [ACTION_SENDING] = &sendFrame,
    [ACTION_STOP_SENDING] = &stopSendFrame,
    [ACTION_SET_SENDING_STATE_IDLE] = &setSendingStateIdle,
    [ACTION_SET_SENDING_STATE_SENDING] = &setSendingStateSending,
    [ACTION_STOP] = &doNothing,
};

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e sender_New()
 * 
 * @brief Initialise le module sender
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sender_New()
{
    /* Initialisation des variables */
    for (uint8_t i = 0; i < FRAME_MAX; i++) 
    {
        s_framesToSend[i].frame.can_id = 0;
        s_framesToSend[i].frame.can_dlc = 0;
        s_framesToSend[i].periodicity = 0;
        s_framesToSend[i].senderMode = SENDER_MODE_PUNCTUAL;
    }

    s_mySem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    /* Initialisation du sémaphore */
    if (s_mySem == SEM_FAILED)
    {
        perror("sem_open()");
		return ERROR_STATE_FAILURE;
    }

    /* Initialisation de la BAL */
    struct mq_attr l_myMqAttr = 
    {
        .mq_maxmsg = MQ_MSG_MAX,
        .mq_msgsize = sizeof(MsgQ_u)
    };
    if ((s_myMq = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0644, &l_myMqAttr)) == -1) 
    {
		perror("mq_open()");
		return ERROR_STATE_FAILURE;
	}

    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sender_Start()
 * 
 * @brief Démarre le module sender
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sender_Start()
{
    /* Lancement du thread principal du module */
    if (pthread_create(&s_myThreadSender, NULL, run, NULL) < 0)
    {
        fprintf(stderr, "erreur pthread_create\n");
		return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sender_StartSending(Frame_t p_frames[], uint8_t nbFrames)
 * 
 * @brief Démarre le thread pour envoyer les trames CAN sur le bus
 * 
 * @param[in] p_frames tableau de trames à envoyer
 * 
 * @param[in] nbFrames nombre de trames à envoyer
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sender_StartSending(Frame_t p_frames[], uint8_t nbFrames)
{
    /* Vérification de la validité des trames */
    assert(nbFrames < FRAME_MAX);
    assert(nbFrames > 0);

    /* Préparation du message */
    MsgQ_u l_msgQu = {
        {
            .event = EVENT_START_SENDING,
            .nbFrames = nbFrames
        }
    }; 
    for (uint8_t i = 0; i < nbFrames; i++)
    {
        /* Sauvegarde des trames reçues */
        s_framesToSend[i] = p_frames[i];
        l_msgQu.msgQ.frames[i+1] = p_frames[i]; /* On laisse le 1er terme vide et on ne l'utilise pas */
    }

    /* Transmet l'évènement au thread */
    if (mq_send(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), 0) == -1) 
    {
        perror("mq_send()");
        mq_close(s_myMq);
        return ERROR_STATE_FAILURE;
    }

    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sender_StopSending()
 * 
 * @brief Arrête le thread d'envoi des trames CAN sur le bus
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sender_StopSending()
{
    MsgQ_u l_msgQu = 
    {
        {
            .event = EVENT_STOP_SENDING
        }
    };
    /* Transmet l'évènement au thread */
    if (mq_send(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), 0) == -1) 
    {
        perror("mq_send()");
        mq_close(s_myMq);
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sender_AskSendingState()
 * 
 * @brief Demander si le Sender est en mode d'envoi
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sender_AskSendingState()
{
    MsgQ_u l_msgQu = 
    {
        {
            .event = EVENT_ASK_SENDING_STATE
        }
    };
    /* Transmet l'évènement au thread */
    if (mq_send(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), 0) == -1) 
    {
        perror("mq_send()");
        mq_close(s_myMq);
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sender_Stop()
 * 
 * @brief Arrête le module sender
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sender_Stop()
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Arrêt de l'envoi en précaution */
    l_err = sender_StopSending();
    MsgQ_u l_msgQu =
    {
        {
            .event = EVENT_STOP
        }
    };
    /* Transmet l'évènement au thread */
    if (mq_send(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), 0) == -1) 
    {
        perror("mq_send()");
        mq_close(s_myMq);
        return ERROR_STATE_FAILURE;
    }

    /* Attent le retour du thread */
    pthread_join(s_myThreadSender, (void*)(&l_err));

    return l_err;
}

/**
 * @fn ErrorState_e sender_Free()
 * 
 * @brief Libère les ressources du module sender
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sender_Free()
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Désinitialisation de la BAL */
    if (mq_unlink(MQ_NAME) == -1) 
    {
        perror("mq_unlink()");
        l_err |= ERROR_STATE_FAILURE;
    }
    
    /* Destruction du sémaphore avant l'arrêt du programme */
    if(sem_close(s_mySem) != 0)
    {
        perror("sem_close()");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (sem_unlink(SEM_NAME) == -1) 
    {
        perror("sem_unlink()");
        l_err |= ERROR_STATE_FAILURE;
    }
    return l_err;
}

/***************************************\
|*	Description des fonctions locales  *|
\***************************************/

/**
 * @fn static void *run(void *p_arg_pt)
 * 
 * @brief Fonction de routine du module
 * 
 * @param[in] p_arg_pt argument à donner au thread
 */
static void *run(void *p_arg_pt)
{
    static MsgQ_u s_msgQu;
    static ErrorState_e l_err = 0;
    static State_e l_state = STATE_IDLE;

    while (l_state != STATE_DEATH)
    {
        /* Réception de l'évènement */
        if (mq_receive(s_myMq, s_msgQu.buffer, sizeof(MsgQ_u), NULL) == -1) 
        {
            perror("mq_receive()");
            pthread_exit((void*)ERROR_STATE_FAILURE);
        }
        if (s_transition[l_state][s_msgQu.msgQ.event].next_state != STATE_FORGET)
        {
            /* Appel de l'action correspondant à l'évènement reçu */
            l_err |= s_actionSending[s_transition[l_state][s_msgQu.msgQ.event].action](s_msgQu.msgQ);
            /* Changement de l'état */
            l_state = s_transition[l_state][s_msgQu.msgQ.event].next_state;
        }
    }
    
    pthread_exit((void*)&l_err);
}

/**
 * @fn static ErrorState_e sendFrame(MsgQ_t p_msgQ)
 * 
 * @brief Lance le thread permettant d'écouter les trames du bus CAN
 * 
 * @param[in] p_msgQ structure contenant diverses informations 
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e sendFrame(MsgQ_t p_msgQ)
{
    /* Initialisation du timer */
    struct sigevent event;

	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = (void*)(&p_msgQ);
	event.sigev_notify_function = &sendCallback;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_REALTIME, &event, &s_myTimer) != 0) 
    {
		perror("timer_create");
		return ERROR_STATE_FAILURE;
	}
    
    /* Création du thread d'envoi de trame CAN sur le bus */
    if (pthread_create(&s_myThreadSending, NULL, send, (void*)(&p_msgQ)) < 0)
    {
        fprintf(stderr, "erreur pthread_create\n");
		return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn static ErrorState_e stopSendFrame(MsgQ_t p_msgQ)
 * 
 * @brief Arrête le thread permettant d'envoyer les trames du bus CAN
 * 
 * @param[in] p_msgQ structure contenant diverses informations
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e stopSendFrame(MsgQ_t p_msgQ)
{
    if (s_mySem != NULL)
    {
        /* On ajoute un jeton au sémaphore pour arrêter l'envoi des trames cycliques */
        if (sem_post(s_mySem) == -1)
        {
            perror("sem_post()");
            mq_close(s_myMq);
            mq_unlink(MQ_NAME);
            return ERROR_STATE_FAILURE;
        }
    }

    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Attent le retour du thread */
    pthread_join(s_myThreadSending, (void*)(&l_err));
    
    /* Destruction du timer après la fin du thread */
    if (timer_delete(s_myTimer) != 0) 
    {
        perror("timer_delete");
        return ERROR_STATE_FAILURE;
    }

    return l_err;
}

/**
 * @fn static ErrorState_e doNothing(MsgQ_t p_msgQ)
 * 
 * @brief Fonction qui ne fait rien
 * 
 * @param[in] p_msgQ structure contenant diverses informations
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e doNothing(MsgQ_t p_msgQ)
{
    /* Ne fait rien */
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn static ErrorState_e setSendingStateIdle(MsgQ_t p_msgQ)
 * 
 * @brief Informe GUI de l'état d'envoi des trames
 * 
 * @param[in] p_msgQ structure contenant diverses informations
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e setSendingStateIdle(MsgQ_t p_msgQ)
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Transmission de l'information à ProxyGUI_setSendingState */
    l_err &= proxyGUI_SetSendingState(SENDING_STATE_OFF);
    return l_err;
}

/**
 * @fn static ErrorState_e setSendingStateSending(MsgQ_t p_msgQ)
 * 
 * @brief Informe GUI de l'état d'envoi des trames
 * 
 * @param[in] p_msgQ structure contenant diverses informations
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e setSendingStateSending(MsgQ_t p_msgQ)
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Transmission de l'information à ProxyGUI_setSendingState */
    l_err &= proxyGUI_SetSendingState(SENDING_STATE_ON);
    return l_err;
}

/**
 * @fn void *send(void *p_arg_pt)
 * 
 * @brief Fonction de routine du thread d'écoute de trames
 * 
 * @param[in] p_arg_pt argument du thread
 */
void *send(void *p_arg_pt)
{
    /* Initialisation des variables */
    static ErrorState_e l_err = ERROR_STATE_SUCCESS;

    /* Lancement du timer */
    struct itimerspec itimer;

	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = TIMER_DELAY_NS;
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_nsec = TIMER_DELAY_NS;

    if (timer_settime(s_myTimer, 0, &itimer, NULL) != 0) 
    {
		perror("timer_settime");
        l_err = ERROR_STATE_FAILURE;
		pthread_exit((void*)&l_err);
	}

    /* Envoi des trames dont le mode d'envoi est ponctuel */
    for (uint8_t i = 1; i < ((MsgQ_t*)p_arg_pt)->nbFrames+1; i++)
    {
        if (((MsgQ_t*)p_arg_pt)->frames[i].senderMode == SENDER_MODE_PUNCTUAL)
        {
            /* Envoi de la trame */
            if (driverCAN_Write((struct can_frame*)&(((MsgQ_t*)p_arg_pt)->frames[i].frame)) == ERROR_STATE_FAILURE)
            {
                fprintf(stderr, "erreur driverCAN_Write\n");
                l_err = ERROR_STATE_FAILURE;
		        // pthread_exit((void*)&l_err);
            }
        }
    }

    /* On attend qu'il y ait un jeton disponible pour continuer  */
    sem_wait(s_mySem);

    /* Arrêt du timer */
    itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_nsec = 0;
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_nsec = 0;

    if (timer_settime(s_myTimer, 0, &itimer, NULL) != 0) 
    {
		perror("timer_settime");
        l_err = ERROR_STATE_FAILURE;
        pthread_exit((void*)&l_err);
	}

    pthread_exit((void*)&l_err);
}

/**
 * @fn static void sendCallback(union sigval p_value)
 * 
 * @brief Fonction appelée par un timer toutes les TIMER_DELAY_NS ns
 * 
 * @param[in] p_value argument de la méthode de callback
 */
static void sendCallback(union sigval p_value)
{
    /* Initialisation des compteurs */
    static bool s_entrance = true;
    static uint64_t s_counter[FRAME_MAX];
    if (s_entrance)
    {
        for (uint8_t j = 0; j < ((MsgQ_t*)p_value.sival_ptr)->nbFrames; j++)
        {
            s_counter[j] = 0;
        }
        s_entrance = false;
    }
    /* Envoi des trames dont le mode d'envoi est cyclique */
    for (uint8_t i = 1; i < ((MsgQ_t*)p_value.sival_ptr)->nbFrames+1; i++)
    {
        if (((MsgQ_t*)p_value.sival_ptr)->frames[i].senderMode == SENDER_MODE_CYCLIC)
        {
            s_counter[i] = (s_counter[i] + (TIMER_DELAY_NS/1000000)) % (((MsgQ_t*)p_value.sival_ptr)->frames[i].periodicity);
            if (s_counter[i] == 0)
            {
                /* Envoi de la trame */
                if (driverCAN_Write((struct can_frame*)(&(((MsgQ_t*)p_value.sival_ptr)->frames[i].frame))) == ERROR_STATE_FAILURE)
                {
                    fprintf(stderr, "erreur driverCAN_Write\n");
                    /* On débloque le sémaphore pour forcer l'arrêt de l'envoi des trames */
                    // sem_post(s_mySem);
                }
            }
        }
    }
}