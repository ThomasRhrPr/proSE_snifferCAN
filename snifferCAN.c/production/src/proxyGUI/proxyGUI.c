/**
 * @file proxyGUI.c
 *
 * @brief Fichier source du module proxyGUI qui permet de substituer le composant GUI pour la communication TCP/IP.
 *
 * @version 0.2.1
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

#include <stdio.h>
#include <assert.h>

#include "proxyGUI.h"
#include "../postman/postman.h"

/****************************\
|*	Déclaration des defines *|
\****************************/

#define BUFFER_SENDING_STATE_SIZE (18)

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static void proxyGUI_EncodeMessage(SendingState_e p_sendingState, char *p_message_pt);

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e proxyGUI_SetSendingState(SendingState_e p_sendingState)
 *
 * @brief Permet de définir un SendingState_e de GUI
 *
 * @param[in] p_sendingState état d'envoi du système
 *
 * @return L'état d'erreur de la fonction
 */
ErrorState_e proxyGUI_SetSendingState(SendingState_e p_sendingState)
{
    char buffer[BUFFER_SENDING_STATE_SIZE];
    proxyGUI_EncodeMessage(p_sendingState, buffer);
    if (postman_SendMessage(buffer) != 0)
    {
        perror("postman_SendMessage");
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/***************************************\
|*	Description des fonctions locales  *|
\***************************************/

/**
 * @fn void proxyGUI_EncodeMessage(SenderMode_e p_sendingState, char *p_message_pt)
 *
 * @brief Permet l'encodage d'un SenderMode_e.
 *
 * @param[in] p_sendingState état d'envoi du système
 *
 * @param[out] p_message_pt message encodé
 */
static void proxyGUI_EncodeMessage(SendingState_e p_sendingState, char *p_message_pt)
{
    /* Vérification de la validité de l'état du Mode Envoi */
    assert(p_sendingState == 0 || p_sendingState == 1);

    snprintf(p_message_pt, BUFFER_SENDING_STATE_SIZE, "~sendingState#%d\n", p_sendingState);
}