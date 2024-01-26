/**
 * @file main.c
 *
 * @brief Fichier source du module main qui permet de lancer le programme.
 *
 * @version 0.1.0
 *
 * @date 29/05/2023
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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>

#include "driverCAN/driverCAN.h"
#include "dispatcher/dispatcher.h"
#include "postman/postman.h"
#include "sender/sender.h"
#include "sniffer/sniffer.h"
#include "ui/ui.h"
#include "driverCAN/driverCAN.h"


/****************************************\
|*	Déclaration des fonctions locales   *|
\****************************************/

static ErrorState_e New();

static ErrorState_e Start();

static ErrorState_e Stop();

static ErrorState_e Free();

static void signalSigintHandler(int signal);

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

static sem_t sem = {{0}};

/***************************************\
|*	Description des fonctions locales  *|
\***************************************/

/**
 * @fn static ErrorState_e New()
 * 
 * @brief Initialise tous les modules
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e New()
{
    if (postman_New() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème initialisation Postman, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    if (dispatcher_New() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème initialisation Disptacher, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    if (driverCAN_New() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème initialisation DriverCAN, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    if (sender_New() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème initialisation Sender, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    if (sniffer_New() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème initialisation Sniffer, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn static ErrorState_e Start()
 * 
 * @brief Démarre tous les modules
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e Start()
{
    if (postman_Start() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème lancement Postman, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    if (dispatcher_Start() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème lancement Dispatcher, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    if (sender_Start() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème lancement Sender, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    if (sniffer_Start() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème lancement Sniffer, abandon\n");
        return ERROR_STATE_FAILURE;
    }
    return ERROR_STATE_SUCCESS;
}

/**
 * @fn static ErrorState_e Stop()
 * 
 * @brief Arrête tous les modules
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e Stop()
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    if (sniffer_Stop() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème arrêt Sniffer\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (sender_Stop() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème arrêt Sender\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (postman_Stop() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème arrêt Postman\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    return l_err;
}

/**
 * @fn static ErrorState_e Free()
 * 
 * @brief Déinitialise tous les modules
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e Free()
{
    ErrorState_e l_err = ERROR_STATE_SUCCESS;
    if (sniffer_Free() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème libération Sniffer\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (sender_Free() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème libération Sender\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (driverCAN_Free() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème libération DriverCAN\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (dispatcher_Free() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème libération Dispatcher\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    if (postman_Free() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème libération Postman\n");
        l_err |= ERROR_STATE_FAILURE;
    }
    return l_err;
}

/**
 * @fn static void signalSigintHandler(int signal)
 * 
 * @brief Handler du signal SIGINT
 * 
 * @param[in] signal argument du signal
 */
static void signalSigintHandler(int signal)
{
    fprintf(stdout, "SIGINT intercepté : demande d'arrêt du programme\n");
    sem_post(&sem);
}

/*************************\
|*	Description du main  *|
\*************************/

/**
 * @fn int main(int argc, char *argv[])
 *
 * @brief Fonction principale du module
 * 
 * @param[in] argc nombre d'arguments
 * 
 * @param[in] argv arguments
*/
int main(int argc, char *argv[])
{
    /* Intitalisation du sémaphore */
    if (sem_init(&sem, 0, 0) == -1)
    {
        perror("sem_init");
        return EXIT_FAILURE;
    }
    /* Interception du SIGINT pour sortie propre */
    signal(SIGINT, signalSigintHandler);

    /* Initialisation des modules */
    if (New() ==  ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problèmes initialisations \nDébut libération \n");
        Free();
        return ERROR_STATE_FAILURE;
    }
    
    if (Start() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problèmes lancement \nDébut arrêt \n");
        Stop();
        fprintf(stderr, "Début libération \n");
        Free();
        return ERROR_STATE_FAILURE;
    }

    /* Démarrage de CANgateway */
    ui_StartCANgateway();

    /* Attente de la demande de la fin du programme */
    sem_wait(&sem);

    /* Arrêt de CANgateway */
    ui_StopCANgateway();

    /* Arrêt des modules */
    if (Stop() == ERROR_STATE_FAILURE)
    {
        fprintf(stderr, "Problème arrêt \n");
    }

    if (Free() == ERROR_STATE_FAILURE)
    {
        
        fprintf(stderr, "Problème libération \n");
    }

    /* Destruction du sémaphore */
    if (sem_destroy(&sem) == -1)
    {
        perror("sem_destroy");
        return ERROR_STATE_FAILURE;
    }

    return ERROR_STATE_SUCCESS;
}
