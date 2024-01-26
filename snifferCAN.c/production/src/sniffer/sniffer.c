/**
 * @file sniffer.c
 * 
 * @brief Fichier source du module sniffer qui permet de gérer la communication avec le bus CAN.
 * 
 * @version 0.1.0
 * 
 * @date 08/05/2023
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

#include "sniffer.h"
#include "../driverCAN/driverCAN.h"
#include "../proxyLogger/proxyLogger.h"

/****************************\
|*	Déclaration des defines *|
\****************************/

#define MQ_NAME ("/mq_sniffer")
#define MQ_MSG_MAX (10)

#define TEST_SANS_PROXY (0)

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/
typedef enum
{
    STATE_FORGET = 0,
    STATE_IDLE,
    STATE_LISTENING,
    STATE_DEATH,
    STATE_COUNT
} State_e;

typedef enum
{
    EVENT_START_LISTENING = 0,
    EVENT_STOP_LISTENING,
    EVENT_STOP,
    EVENT_COUNT
} Event_e;

typedef enum
{
    ACTION_LISTENING = 0,
    ACTION_STOP_LISTENING,
    ACTION_NOTHING,
    ACTION_STOP,
    ACTION_COUNT
} Action_e;

typedef struct 
{
    State_e next_state;
    Action_e action;
} Transition_t;

typedef union 
{
    Event_e event;
    char buffer[sizeof(Event_e)];
} MsgQ_u;

typedef ErrorState_e (*actionReading) (void);

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static ErrorState_e readCAN();

static ErrorState_e readFrame();

static ErrorState_e stopReadFrame();

static ErrorState_e doNothing();

static void *run(void *p_arg_pt);

static void *receiveFrame(void *p_arg_pt);

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

static State_e s_myState = STATE_IDLE;

static mqd_t s_myMq = {0};

static pthread_t s_myThreadSniffer = {0};

static pthread_t s_myThreadReading = {0};

static volatile bool sv_keepReading = false;

static Transition_t s_transition[STATE_COUNT][EVENT_COUNT] = 
{
    [STATE_IDLE][EVENT_START_LISTENING] = {STATE_LISTENING, ACTION_LISTENING},
    [STATE_IDLE][EVENT_STOP] = {STATE_DEATH, ACTION_STOP},

    [STATE_LISTENING][EVENT_STOP_LISTENING] = {STATE_IDLE, ACTION_STOP_LISTENING},
    [STATE_LISTENING][EVENT_STOP] = {STATE_DEATH, ACTION_STOP}
};

static actionReading s_actionReading[ACTION_COUNT] = 
{
    [ACTION_LISTENING] = &readFrame,
    [ACTION_STOP_LISTENING] = &stopReadFrame,
    [ACTION_NOTHING] = &doNothing,
    [ACTION_STOP] = &doNothing
};

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e sniffer_New()
 * 
 * @brief Initialise le module sniffer
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sniffer_New()
{
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
 * @fn ErrorState_e sniffer_Start()
 * 
 * @brief Démarre le module sniffer
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sniffer_Start()
{
    /* Lancement du thread principal du module */
    if (pthread_create(&s_myThreadSniffer, NULL, run, NULL) < 0)
    {
        fprintf(stderr, "erreur pthread_create\n");
		return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sniffer_StartReading()
 * 
 * @brief Démarre le thread pour lire les trames CAN sur le bus
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sniffer_StartReading()
{
    MsgQ_u l_msgQ;
    l_msgQ.event = EVENT_START_LISTENING;
    /* Transmet l'évènement au thread */
    if (mq_send(s_myMq, l_msgQ.buffer, sizeof(MsgQ_u), 0) == -1) 
    {
        perror("mq_send()");
        mq_close(s_myMq);
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sniffer_StopReading()
 * 
 * @brief Arrête le thread pour lire les trames CAN sur le bus
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sniffer_StopReading()
{
    MsgQ_u l_msgQ;
    l_msgQ.event = EVENT_STOP_LISTENING;
    /* Transmet l'évènement au thread */
    if (mq_send(s_myMq, l_msgQ.buffer, sizeof(MsgQ_u), 0) == -1) 
    {
        perror("mq_send()");
        mq_close(s_myMq);
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e sniffer_Stop()
 * 
 * @brief Arrête le module sniffer
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sniffer_Stop()
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    MsgQ_u l_msgQ;
    l_msgQ.event = EVENT_STOP;
    /* Transmet l'évènement au thread */
    if (mq_send(s_myMq, l_msgQ.buffer, sizeof(MsgQ_u), 0) == -1) 
    {
        perror("mq_send()");
        mq_close(s_myMq);
        return ERROR_STATE_FAILURE;
    }
    /* Attent le retour du thread */
    pthread_join(s_myThreadSniffer, (void*)(&l_err));

    return l_err;
}

/**
 * @fn ErrorState_e sniffer_Free()
 * 
 * @brief Libère les ressources du module sniffer
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e sniffer_Free()
{
    /* Désinitialisation de la BAL */
    if (mq_unlink(MQ_NAME) == -1) 
    {
        perror("mq_unlink()");
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
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
    static MsgQ_u s_msgQ;
    static ErrorState_e l_err = ERROR_STATE_SUCCESS;
    while (s_myState != STATE_DEATH)
    {
        /* Réception de l'évènement */
        if (mq_receive(s_myMq, s_msgQ.buffer, sizeof(MsgQ_u), NULL) == -1) 
        {
            perror("mq_receive()");
            l_err = ERROR_STATE_FAILURE;
            pthread_exit((void*)&l_err);
        }
        if (s_transition[s_myState][s_msgQ.event].next_state != STATE_FORGET)
        {
            /* Appel de l'action correspondant à l'évènement reçu */
            l_err |= s_actionReading[s_transition[s_myState][s_msgQ.event].action]();
            /* Changement de l'état */
            s_myState = s_transition[s_myState][s_msgQ.event].next_state;
        }
    }
    pthread_exit((void*)&l_err);
}

/**
 * @fn static ErrorState_e readFrame()
 * 
 * @brief Lance le thread permettant de lire les trames du bus CAN
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e readFrame()
{
    sv_keepReading = true;
    /* Création du thread de réception de trame CAN sur le bus */
    if (pthread_create(&s_myThreadReading, NULL, receiveFrame, NULL) < 0)
    {
        fprintf(stderr, "erreur pthread_create\n");
		return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn static ErrorState_e stopReadFrame()
 * 
 * @brief Arrête le thread permettant de lire les trames du bus CAN
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e stopReadFrame()
{
    sv_keepReading = false;
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Attend le retour du thread */
    pthread_join(s_myThreadReading, (void*)(&l_err));
    return l_err;
}

/**
 * @fn static ErrorState_e doNothing()
 * 
 * @brief Fonction qui ne fait rien
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e doNothing()
{
    /* Ne fait rien */
    return ERROR_STATE_SUCCESS;
}


/**
 * @fn static ErrorState_e readCAN()
 * 
 * @brief Lit une trame sur le bus CAN et la transmet à ProxyLogger
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e readCAN()
{
    /* Déclaration des variables */
    struct can_frame l_can_frame;
    ErrorState_e l_err = ERROR_STATE_SUCCESS;

    /* Réception de la trame */
    l_err |= driverCAN_Read(&l_can_frame);
    if (l_err == ERROR_STATE_SUCCESS)
    {
        #if (TEST_SANS_PROXY == 1)
        fprintf(stdout, "Trame : %x  ", l_can_frame.can_id);
        fprintf(stdout, "%d  ", l_can_frame.can_dlc);
        fprintf(stdout, " ");
        for (int i = 0; i < l_can_frame.can_dlc; i++)
        {
            fprintf(stdout, "%x ", l_can_frame.data[i]);
        }
        fprintf(stdout, "\n");
        #else 
        Frame_t l_frame;
        /* On transmet la trame à ProxyLogger */
        l_frame.frame = l_can_frame;
        l_err = proxyLogger_SetFrame(&l_frame);
        #endif
        return l_err;
    }
    else if (l_err == ERROR_STATE_TIMEOUT)
    {
        return ERROR_STATE_TIMEOUT;
    }
    else
    {
        return ERROR_STATE_FAILURE;
    }

}

/**
 * @fn static void *receiveFrame(void *p_arg_pt)
 * 
 * @brief Fonction de routine du thread d'écoute de trames
 * 
 * @param[in] p_arg_pt argument du thread
 */
static void *receiveFrame(void *p_arg_pt)
{
    /* Initialisation des variables locales */
    static ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Boucle infinie */
    while(sv_keepReading)
    {
        /* Réception de la trame */
        l_err = readCAN();
        if (l_err == ERROR_STATE_FAILURE)
        {
            /* Report de l'erreur */
            fprintf(stderr, "Erreur de réception de la trame\n");
            pthread_exit((void*)&l_err);
        }
        else if (l_err == ERROR_STATE_TIMEOUT)
        {
            /* Report de l'erreur */
            fprintf(stderr, "Timeout de réception de la trame\n");
            /* On continue la potentielle lecture */
        }
    }
    l_err = ERROR_STATE_SUCCESS;
    pthread_exit((void*)&l_err);
}