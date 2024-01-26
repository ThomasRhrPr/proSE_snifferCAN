/**
 * @file driverCAN.c
 * 
 * @brief Fichier source du module driverCAN qui permet de gérer la communication avec le bus CAN. Objet frontière.
 * 
 * @version 0.1.0
 * 
 * @date 06/05/2023
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <linux/can/raw.h>
#include <linux/if.h>
#include <errno.h>
#include <pthread.h>

#include "driverCAN.h"

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

static int s_myRet = 0;
static int s_mySocketRead = 0;
static int s_mySocketWrite = 0;

static pthread_mutex_t s_mutex = {{0}};

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e driverCAN_New()
 * 
 * @brief Fonction d'initialisation du module driverCAN
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e driverCAN_New()
{
    /* Initilatisation des variables locales */
    struct sockaddr_can l_myAddr = {0};
    struct ifreq l_myIfr = {{{0}}};
    struct timeval l_tv;
    l_tv.tv_sec = 2;
    l_tv.tv_usec = 0;

    /* Création de la socketRead CAN */
    s_mySocketRead = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s_mySocketRead < 0) 
    {
        perror("socketRead PF_CAN failed");
        return ERROR_STATE_FAILURE;
    }

    /* Création de la socketWrite CAN */
    s_mySocketWrite = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s_mySocketWrite < 0) 
    {
        perror("socketWrite PF_CAN failed");
        close(s_mySocketRead);
        return ERROR_STATE_FAILURE;
    }

    /* Spécification du device can0 */
    strcpy(l_myIfr.ifr_name, "can0");
    s_myRet = ioctl(s_mySocketRead, SIOCGIFINDEX, &l_myIfr);
    if (s_myRet < 0) 
    {
        perror("ioctl failed");
        close(s_mySocketRead);
        close(s_mySocketWrite);
        return ERROR_STATE_FAILURE;
    }
    strcpy(l_myIfr.ifr_name, "can0");
    s_myRet = ioctl(s_mySocketWrite, SIOCGIFINDEX, &l_myIfr);
    if (s_myRet < 0) 
    {
        perror("ioctl failed");
        close(s_mySocketWrite);
        close(s_mySocketRead);
        return ERROR_STATE_FAILURE;
    }

    /* Lien entre la socketRead et can0 */
    l_myAddr.can_family = PF_CAN;
    l_myAddr.can_ifindex = l_myIfr.ifr_ifindex;
    s_myRet = bind(s_mySocketRead, (struct sockaddr *)&l_myAddr, sizeof(l_myAddr));
    if (s_myRet < 0) 
    {
        perror("bind failed");
        close(s_mySocketWrite);
        close(s_mySocketRead);
        return ERROR_STATE_FAILURE;
    }
    /* Lien entre la socketWrite et can0 */
    l_myAddr.can_family = PF_CAN;
    l_myAddr.can_ifindex = l_myIfr.ifr_ifindex;
    s_myRet = bind(s_mySocketWrite, (struct sockaddr *)&l_myAddr, sizeof(l_myAddr));
    if (s_myRet < 0) 
    {
        perror("bind failed");
        close(s_mySocketWrite);
        close(s_mySocketRead);
        return ERROR_STATE_FAILURE;
    }

    /* Paramétrage de la socketRead afin qu'elle ait un TimeOUT si aucune trame n'est émise */
    if (setsockopt(s_mySocketRead, SOL_SOCKET, SO_RCVTIMEO, (const char*)&l_tv, sizeof(l_tv)) < 0)
    {
        perror("set socket option");
        close(s_mySocketWrite);
        close(s_mySocketRead);
        return ERROR_STATE_FAILURE;
    }

    /* Initialisation du mutex */
    if (pthread_mutex_init(&s_mutex, NULL) != 0)
    {
        fprintf(stderr, "erreur pthread_mutex_init\n");
		close(s_mySocketRead);
        close(s_mySocketWrite);
        return ERROR_STATE_FAILURE;
    }

    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e driverCAN_Read(struct can_frame *p_frame_pt)
 * 
 * @brief Fonction de lecture d'une trame CAN
 * 
 * @param[out] p_frame_pt pointeur vers la trame CAN à lire
 * 
 * @return L'état d'erreur de la fonction. Peut renvoyer l'état TIMEOUT si aucune trame n'est lue à temps
 */
ErrorState_e driverCAN_Read(struct can_frame *p_frame_pt)
{
    /* Lecture d'une trame CAN */
    struct can_frame v_readFrame;
    int8_t v_nbytes = 0;
    v_nbytes = read(s_mySocketRead, &v_readFrame, sizeof(v_readFrame));
    if ((v_nbytes == -1) && (errno == EAGAIN))
    {
        return ERROR_STATE_TIMEOUT;
    }
    else if (((v_nbytes == -1) && (errno != EAGAIN)) || (v_nbytes != sizeof(v_readFrame))) 
    {
        perror("read failed\r\n");
        return ERROR_STATE_FAILURE;
    }
    else
    {
        /* La trame CAN lue est accessible via le pointeur p_frame_pt */
        *p_frame_pt = v_readFrame;
        return ERROR_STATE_SUCCESS;
    }
}

/**
 * @fn ErrorState_e driverCAN_Write(struct can_frame *p_frame_pt)
 * 
 * @brief Fonction d'écriture d'une trame CAN
 * 
 * @param[in] p_frame_pt pointeur vers la trame CAN à écrire
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e driverCAN_Write(struct can_frame *p_frame_pt)
{
    pthread_mutex_lock(&s_mutex);
    /* Ecriture d'une trame CAN sur le bus */
    int8_t v_nbytes = write(s_mySocketWrite, p_frame_pt, sizeof(*p_frame_pt)); 
    if(v_nbytes != sizeof(*p_frame_pt)) 
    {
        perror("write failed");
        pthread_mutex_unlock(&s_mutex);
        return ERROR_STATE_FAILURE;
    }
    pthread_mutex_unlock(&s_mutex);
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn ErrorState_e driverCAN_Free()
 * 
 * @brief Fonction de désinitialisation du module driverCAN
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e driverCAN_Free()
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    /* Fermeture de la socketRead */
    if (close(s_mySocketRead) < 0)
    {
        perror("close socket failed");
        close(s_mySocketWrite);
        l_err |= ERROR_STATE_FAILURE;
    }
    /* Fermeture de la socketWrite */
    if (close(s_mySocketWrite) < 0)
    {
        perror("close socket failed");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (pthread_mutex_destroy(&s_mutex))
    {
        perror("destroy mutex failed");
        l_err |= ERROR_STATE_FAILURE;
    }
    return l_err;
}