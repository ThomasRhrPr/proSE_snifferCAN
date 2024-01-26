/**
 * @file postman.c
 * 
 * @brief Fichier source du module postman qui réalise l'envoi et la réception des sockets.
 * 
 * @version 0.2.2
 * 
 * @date 09/05/2023
 * 
 * @author Thomas ROCHER
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
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "postman.h"
#include "../dispatcher/dispatcher.h"

/****************************\
|*	Déclaration des defines *|
\****************************/

#define PORT_DU_SERVEUR (12345)

#define MQ_NAME ("/mq_postman")
#define MQ_MSG_MAX (10)

#define SEM_NAME ("/sem_waitClient")

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/

typedef enum
{   
    STATE_FORGET = 0,
    STATE_WAITING_CONNECTION,
    STATE_CONNECTED,
    STATE_DEATH,
    STATE_COUNT
} State_e;

typedef enum
{
    EVENT_NOTHING = 0,
    EVENT_CONNECTION,
    EVENT_DISCONNECTION,
    EVENT_SEND_REQUEST,
    EVENT_STOP,
    EVENT_COUNT
} Event_e;

typedef enum
{
    ACTION_NOP = 0,
    ACTION_WAIT_FOR_CONNECTION,
    ACTION_SEND_DATA,
    ACTION_STOP
} Action_e;

typedef struct
{
	State_e destination_state;
	Action_e action;
} Transition_t;

typedef struct
{
	Event_e event;
    char message[MESSAGE_SIZE];
} MsgQ_t;

typedef union
{
	MsgQ_t msgQ;
  char buffer[sizeof(MsgQ_t)];
} MsgQ_u;

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

static int s_socket_ecoute = 0;
static int s_socket_donnees = 0;

static pthread_t s_myThread = {0};

static 	mqd_t s_myMq = {0};

static sem_t *s_mySem = NULL;

static Transition_t s_transition [STATE_COUNT-1][EVENT_COUNT] =
{ 
    [STATE_WAITING_CONNECTION][EVENT_NOTHING] = {STATE_WAITING_CONNECTION, ACTION_WAIT_FOR_CONNECTION},   
    [STATE_WAITING_CONNECTION][EVENT_CONNECTION] = {STATE_CONNECTED, ACTION_NOP},
    [STATE_WAITING_CONNECTION][EVENT_STOP] = {STATE_DEATH, ACTION_STOP},

    [STATE_CONNECTED][EVENT_DISCONNECTION] = {STATE_WAITING_CONNECTION, ACTION_WAIT_FOR_CONNECTION},
    [STATE_CONNECTED][EVENT_SEND_REQUEST] = {STATE_CONNECTED, ACTION_SEND_DATA},
    [STATE_CONNECTED][EVENT_STOP] = {STATE_DEATH, ACTION_STOP},    
};

// static bool first_semwait = true;

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static void *run(void *p_arg_pt);

static ErrorState_e performAction(Action_e p_action, char *p_message_pt);

static ErrorState_e waitForConnection();

static ErrorState_e sendData(char *p_message_pt);

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e postman_New()
 *
 * @brief Fonction qui initialise le module postman
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e postman_New()
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

    s_mySem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    /* Initialisation du sémaphore pour le tranmettre au thread suivant */
    if (s_mySem == SEM_FAILED)
    {
        perror("sem_open()");
		return ERROR_STATE_FAILURE;
    }

    struct sockaddr_in mon_adresse;

    /* Création d'un socket de type écoute */
    s_socket_ecoute = socket(PF_INET, SOCK_STREAM, 0);
    if (s_socket_ecoute == -1)
    {
        perror("Erreur lors de la création de la socket écoute");
        return ERROR_STATE_FAILURE;
    }

    /* Configuration du socket */
    mon_adresse.sin_family = AF_INET;
    mon_adresse.sin_port = htons(PORT_DU_SERVEUR);
    mon_adresse.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Association du socket à l'adresse du serveur */
    if (bind(s_socket_ecoute, (struct sockaddr*) &mon_adresse, sizeof(mon_adresse)) == -1) 
    {
        perror("Erreur lors de l'association du socket à l'adresse du serveur");
        close(s_socket_ecoute);
        return ERROR_STATE_FAILURE;
    }

    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e postman_Start()
 *
 * @brief Fonction qui démarre le module postman
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e postman_Start()
{
    /* Attente de connexions entrantes */
    if (listen(s_socket_ecoute, 1) == -1)
    {
        perror("Erreur lors de l'appel à la fonction listen");
        return ERROR_STATE_FAILURE;
    }
    
    /* Lancement du thread principal du module */
    if (pthread_create(&s_myThread, NULL, run, NULL) < 0)
    {
        perror("erreur pthread_create");
		return ERROR_STATE_FAILURE;
    }

    return ERROR_STATE_SUCCESS;
}


/**
 * @fn ErrorState_e postman_ReceiveData(char *p_char_pt)
 *
 * @brief Fonction qui appelle la fonction recv permettant de recevoir des données via TCP/IP
 * 
 * @param[out] p_char_pt pointeur vers le caractère reçu via TCP/IP
 * 
 * @param[in] p_firstTime_pt pointeur indiquant si on appelle postman_ReceiveData pour la première après une connexion
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e postman_ReceiveData(char *p_char_pt, bool *p_firstTime_pt)
{
    char l_buffer;
    int l_receive_str_size;

    if(*p_firstTime_pt)
    {
        /* On attend qu'il y ait un jeton disponible pour continuer  */
        sem_wait(s_mySem);
    }
    else
    {
        /* Lecture octet par octet des données reçues */
        l_receive_str_size = recv(s_socket_donnees, &l_buffer, 1, 0);

        if (l_receive_str_size == -1)
        {   
            perror("Erreur lors de la réception des données");
            return ERROR_STATE_FAILURE;
        }
        else if (l_receive_str_size == 0)
        {
            fprintf(stdout, "Deconnexion du client\n");
            MsgQ_u l_msgQu =
            {
                {
                    .event = EVENT_DISCONNECTION
                }
            };
            /* Transmet l'évènement au thread */
            if (mq_send(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), 0) == -1) 
            {
                perror("mq_send()");
                return ERROR_STATE_FAILURE;
            }

            /* On attend qu'il y ait un jeton disponible pour continuer  */
            sem_wait(s_mySem);
            *p_firstTime_pt = true;
        }
        else
        {
            *p_char_pt = l_buffer;
        }
    }

    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e postman_SendMessage(char *p_message_pt)
 *
 * @brief Fonction permettant l'envoi des messages dans la BAL stockant les messages à envoyer à CANdroid
 *
 * @param[in] p_message_pt pointeur vers la donnée à envoyer via TCP/IP
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e postman_SendMessage(char *p_message_pt)
{ 
    MsgQ_u l_msgQu =
    {
        {
            .event = EVENT_SEND_REQUEST,
        }
    };
    strcpy(l_msgQu.msgQ.message, p_message_pt);
    
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
 * @fn ErrorState_e postman_Stop()
 *
 * @brief Fonction qui arrête le module postman
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e postman_Stop()
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;

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
    
    l_err = dispatcher_Stop();

    MsgQ_u l_msgQu =
    {
        {
            .event = EVENT_STOP,
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
    pthread_join(s_myThread, (void*)(&l_err));

    close(s_socket_ecoute);
    close(s_socket_donnees);

    return l_err;
}

/**
 * @fn ErrorState_e postman_Free()
 *
 * @brief Fonction qui libère les ressources du module postman
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e postman_Free()
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

/****************************************\
|*	Description des fonctions locales  *|
\****************************************/

/**
 * @fn static void *run(void *p_arg_pt)
 * 
 * @brief Fonction de routine du module
 * 
 * @param[in] p_arg_pt pointeur vers l'argument à donner au thread
 */
static void *run(void *p_arg_pt)
{
    static ErrorState_e l_err = ERROR_STATE_SUCCESS;
    MsgQ_u l_msgQu;
    l_err = performAction(ACTION_WAIT_FOR_CONNECTION, l_msgQu.msgQ.message);
    State_e l_myState = STATE_WAITING_CONNECTION;

    while (l_myState != STATE_DEATH)
    {
        /* Réception de l'évènement */
        if (mq_receive(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), NULL) == -1) 
        {
            perror("mq_receive()");
            l_err = ERROR_STATE_FAILURE;
            pthread_exit((void*)&l_err);
        }

        if (s_transition[l_myState][l_msgQu.msgQ.event].destination_state != STATE_FORGET)
        {
            l_err = performAction(s_transition[l_myState][l_msgQu.msgQ.event].action, l_msgQu.msgQ.message);
            l_myState = s_transition[l_myState][l_msgQu.msgQ.event].destination_state;
        }
    }
    
    pthread_exit((void*)&l_err);
}

/**
 * @fn static ErrorState_e performAction(Action_e p_action, char *p_message_pt)
 * 
 * @brief Fonction appelant les méthodes correspondantes au contexte
 * 
 * @param[in] p_action Action_e a réalisé
 * 
 * @param[in] p_message_pt pointeur vers le message qui sera envoyé via TCP/IP en le passant en paramètre de la fonction sendData
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e performAction(Action_e p_action, char *p_message_pt)
{
	switch (p_action)
    {
    case ACTION_NOP:
        break;
    case ACTION_WAIT_FOR_CONNECTION:
        if (waitForConnection() == ERROR_STATE_FAILURE)
        {
            perror("waitForConnection");
            return ERROR_STATE_FAILURE;
        }
        break;
    case ACTION_SEND_DATA:
        if (sendData(p_message_pt) == ERROR_STATE_FAILURE)
        {
            perror("sendData");
            return ERROR_STATE_FAILURE;
        }
        break;
    case ACTION_STOP:
        break;
    default:
        fprintf(stdout, "Action inconnue, pb ds la MAE\n");
        return ERROR_STATE_FAILURE;
        break;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn static ErrorState_e waitForConnection() 
 *
 * @brief Fonction bloquante permettant l'attente de connexion d'un client
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e waitForConnection() 
{
    struct sockaddr_in l_adresse_client;
    socklen_t l_taille_adresse_client = sizeof(l_adresse_client);  

    /* Configuration du délai d'attente */
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    /* Création des descripteurs de fichiers pour la fonction select() */
    fd_set l_read_fds;
    FD_ZERO(&l_read_fds);
    FD_SET(s_socket_ecoute, &l_read_fds);

    /* Vérification de la présence d'un client avec délai */
    int l_result = select(s_socket_ecoute + 1, &l_read_fds, NULL, NULL, &timeout);
    if (l_result == -1) 
    {
        perror("Erreur lors de la sélection");
        return ERROR_STATE_FAILURE;
    } 
    else if (l_result == 0) 
    {
        MsgQ_u l_msgQu =
        {
            {
                .event = EVENT_NOTHING
            }
        };
        /* Transmet l'évènement au thread */
        if (mq_send(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), 0) == -1) 
        {
            perror("mq_send()");
            mq_close(s_myMq);
            return ERROR_STATE_FAILURE;
        }
    }
    else
    {
        /* Acceptation de connexion entrante */
        s_socket_donnees = accept(s_socket_ecoute, (struct sockaddr *)&l_adresse_client, &l_taille_adresse_client);
        if(s_socket_donnees == -1)
        {
            perror("Erreur lors de l'acceptation de la connexion");
            return ERROR_STATE_FAILURE;
        }

        MsgQ_u l_msgQu =
        {
            {
                .event = EVENT_CONNECTION
            }
        };
        /* Transmet l'évènement au thread */
        if (mq_send(s_myMq, l_msgQu.buffer, sizeof(MsgQ_u), 0) == -1) 
        {
            perror("mq_send()");
            mq_close(s_myMq);
            return ERROR_STATE_FAILURE;
        }

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
    }

    return ERROR_STATE_SUCCESS;
}

/**
 * @fn static ErrorState_e sendData(char *p_message_pt)
 *
 * @brief Fonction qui envoie les données au client via le protocole de communication TCP/IP
 * 
 * @param[in] p_message_pt pointeur vers le message a envoyé au socket client
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e sendData(char *p_message_pt)
{
    /* Envoi des données */
    if (send(s_socket_donnees, p_message_pt, strlen(p_message_pt), 0) == -1)
    {
        perror("Erreur lors de l'envoi des données au client");
        return ERROR_STATE_FAILURE;
    }

    return ERROR_STATE_SUCCESS;
}