/**
 * @file proxyLogger.c
 * 
 * @brief Fichier source du module proxyLogger qui permet de substituer le composant Logger pour la communication TCP/IP.
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
#include <string.h>
#include <stdint.h>

#include "proxyLogger.h"
#include "../postman/postman.h"

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

void proxyLogger_EncodeMessage(Frame_t *p_frame_pt, char *p_message_pt);

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e proxyLogger_SetFrame(Frame_t *p_frame_pt)
 *
 * @brief Permet de définir Frame_t de Logger
 *
 * @param[in] p_frame_pt la trame à transmettre à Logger
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e proxyLogger_SetFrame(Frame_t *p_frame_pt)
{
    char buffer[MESSAGE_SIZE];
    proxyLogger_EncodeMessage(p_frame_pt, buffer);
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
 * @fn void proxyLogger_EncodeMessage(Frame_t *p_frame_pt, char *p_message_pt)
 *
 * @brief Permet l'encodage d'une structure Frame_t
 *
 * @param[in] p_frame_pt la structure représentant une trame CAN à encoder
 * 
 * @param[out] p_message_pt la chaine de caractère obtenu après encodage
*/
void proxyLogger_EncodeMessage(Frame_t *p_frame_pt, char *p_message_pt)
{
    /* Calcul la longueur de l'identifiant */
    int id_len = 0;
    uint32_t can_id = p_frame_pt->frame.can_id;
    if(can_id == 0)
    {
        id_len = 1;
    }
    else
    {
        while (can_id > 0) 
        {
            can_id >>= 4;
            id_len++;
        }
    }

    int message_len = snprintf(p_message_pt, MESSAGE_SIZE, "~frame#%X$%d@", p_frame_pt->frame.can_id, p_frame_pt->frame.can_dlc);

    for (int i = 0; i < p_frame_pt->frame.can_dlc; i++)
    {
        message_len += snprintf(p_message_pt + message_len, 3, "%02X", p_frame_pt->frame.data[i]);
    }

    message_len += snprintf(p_message_pt + message_len, 2, "\n");
}