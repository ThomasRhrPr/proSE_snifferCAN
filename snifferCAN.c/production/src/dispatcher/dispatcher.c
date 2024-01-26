/**
 * @file dispatcher.c
 * 
 * @brief Fichier source du module dispatcher qui permet de décoder l'information reçue pour la fournir à Sender.
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dispatcher.h"
#include "../postman/postman.h"
#include "../sender/sender.h"

/****************************\
|*	Déclaration des defines *|
\****************************/

#define MAX_MESSAGES (10)

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

static pthread_t s_myThread = {0};

static volatile bool sv_keepReading = false;

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static void *run(void *p_arg_pt);

static ErrorState_e dispatcher_DecodeMessage(char *p_message_pt, Frame_t *p_frame_pt);

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e dispatcher_New()
 *
 * @brief Fonction qui initialise le module dispatcher
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e dispatcher_New()
{
    sv_keepReading = false;
    
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e dispatcher_Start()
 *
 * @brief Fonction qui démarre le module dispatcher
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e dispatcher_Start()
{
    sv_keepReading = true;

    /* Lancement du thread principal du module */
    if (pthread_create(&s_myThread, NULL, run, NULL) < 0)
    {
        perror("erreur pthread_create\n");
		return ERROR_STATE_FAILURE;
    }

    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e dispatcher_Stop()
 *
 * @brief Fonction qui arrête le module dispatcher
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e dispatcher_Stop()
{ 
    ErrorState_e l_err = ERROR_STATE_SUCCESS;

    sv_keepReading = false;

    /* Attent le retour du thread */
    pthread_join(s_myThread, (void*)(&l_err));

    return l_err;
}

/**
 * @fn ErrorState_e dispatcher_Free()
 *
 * @brief Fonction qui libère les ressources du module dispatcher
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e dispatcher_Free()
{
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
 * @param[in] p_arg_pt pointeur vers l'argument à donner au thread
 */
static void *run(void *p_arg_pt)
{
    static ErrorState_e l_err = ERROR_STATE_SUCCESS;

    bool l_nbAsk = false;
    bool l_first_call = true;

    char l_buffer[MESSAGE_SIZE];
    char l_messages[MAX_MESSAGES][MESSAGE_SIZE];

    int l_current_message_index = 0;
    int l_messages_count = 0;
    int l_expected_messages_count = 0;

    while (sv_keepReading)
    {    
        char l_current_char;
        
        l_err = postman_ReceiveData(&l_current_char, &l_first_call);

        /* On ne prend pas en compte le 1er caractère reçu après une connexion car il est vide */
        if(!l_first_call)
        {
            /* '\n' est le délimiteur d'une fin de message */
            if (l_current_char == '\n')
            {
                /* Termine le message actuel */
                l_buffer[l_current_message_index] = '\0';

                /* sender_AskSendingState correspond au message "?\n" */
                if ((l_buffer[0] == '?'))
                {
                    l_err = sender_AskSendingState();
                }
                /* sender_StopSending correspond au message "!\n" */
                else if ((l_buffer[0] == '!'))
                {
                    l_err = sender_StopSending();
                }
                else
                {
                    /* Si le nombre de trame dans la séquence n'a pas été indiqué */
                    if (!l_nbAsk)
                    {
                        int l_nbFrames = atoi(l_buffer + 3);
                        if((l_buffer[0] == 'n') && (l_buffer[1] == 'b') && (l_buffer[2] == '=') && (l_nbFrames != 0) && (l_nbFrames < 11))
                        {
                            /* Récupère le nombre de trames qui vont être envoyées */
                            l_expected_messages_count = l_nbFrames;
                            l_nbAsk = true;
                        }
                    }
                    else
                    {
                        if(strchr(l_buffer, '#') != NULL && strchr(l_buffer, '$') != NULL && strchr(l_buffer, '@') != NULL && strchr(l_buffer, '/') != NULL)
                        {
                            /* Ajout d'un message complet dans la séquence de messages */
                            strcpy(l_messages[l_messages_count], l_buffer);
                            l_messages_count++;
                        }
                    }

                }

                /* Réinitialise les variables */
                strcpy(l_buffer, "");
                l_current_message_index = 0;

                if ((l_messages_count >= l_expected_messages_count) && l_nbAsk)
                {    
                    Frame_t frames[MAX_MESSAGES];
                    for (int i = 0; i < (l_messages_count); i++)
                    {
                        l_err = dispatcher_DecodeMessage(l_messages[i], &(frames[i]));
                    }

                    /* Envoie la séquence de trames */
                    l_err = sender_StartSending(frames, l_messages_count);

                    /* Réinitialise les variables */
                    memset(l_messages, 0, sizeof(l_messages));
                    memset(frames, 0, sizeof(frames));
                    l_messages_count = 0;
                    l_expected_messages_count = 0;
                    l_nbAsk = false;
                }
            }
            else
            {
                /* Ajout d'un caractère dans le message en cours de réception */
                l_buffer[l_current_message_index] = l_current_char;
                l_current_message_index++;
            }
        }
        else
        {
            l_first_call = false;
            l_nbAsk = false;
        }
    }

    pthread_exit((void*)&l_err);
}


/**
 * @fn static ErrorState_e dispatcher_DecodeMessage(char *p_message_pt, Frame_t *p_frame_pt)
 *
 * @brief Fonction permettant le décodage d'une chaine de caractères
 *
 * @param[in] p_message_pt la chaine de caractères à décoder
 * 
 * @param[out] p_frame_pt la structure représentant une trame CAN après décodage
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e dispatcher_DecodeMessage(char *p_message_pt, Frame_t *p_frame_pt)
{
    /* Récupère l'ID de la trame */
    char* id_str = strchr(p_message_pt, '#');
    if (id_str == NULL)
    {
        return ERROR_STATE_FAILURE;
    }
    p_frame_pt->frame.can_id = strtol(id_str+1, NULL, 16);

    /* Récupère la taille de la trame */
    char* len_str = strchr(p_message_pt, '$');
    if (id_str == NULL)
    {
        return ERROR_STATE_FAILURE;
    }
    p_frame_pt->frame.can_dlc = strtol(len_str+1, NULL, 10);

    /* Récupère le message de la trame */
    char* data_str = strchr(p_message_pt, '@') + 1;
    if (data_str == NULL)
    {
        return ERROR_STATE_FAILURE;
    }
    for (int i = 0; i < p_frame_pt->frame.can_dlc; i++) 
    {
        char byte_str[3] = {data_str[i*2], data_str[i*2+1], '\0'};
        p_frame_pt->frame.data[i] = strtol(byte_str, NULL, 16);
    }

    /* Récupère la périodicité pour l'envoi de la trame */
    char* periodicity_str = strchr(p_message_pt, '/');
    if (periodicity_str == NULL)
    {
        return ERROR_STATE_FAILURE;
    }
    p_frame_pt->periodicity = strtol(periodicity_str+1, NULL, 10);

    if(p_frame_pt->periodicity > 0)
    {
        p_frame_pt->senderMode = SENDER_MODE_CYCLIC;
    }
    else
    {
         p_frame_pt->senderMode = SENDER_MODE_PUNCTUAL;
    }

    return ERROR_STATE_SUCCESS;
}