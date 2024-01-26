/**
 * @file : main.c
 *
 * @brieffichier source du module main qui permet de lancer le programme.
 *
 * @version : 0.0.0
 *
 * @date : 129/05/2023
 *
 * @author : Elisa DECLERCK
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
|*	Déclaration des include  *|
\*****************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>

#include "driverCAN/driverCAN.h"
#include "dispatcher/dispatcher.h"
#include "postman/postman.h"
#include "sender/sender.h"
#include "sniffer/sniffer.h"


/****************************************\
|*	Déclaration des fonctions globales  *|
\****************************************/

int8_t starter_New();

int8_t starter_Start();

int8_t starter_Stop();

int8_t starter_Free();

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

static sem_t sem = {{0}};

/***************************************\
|*	Description des fonctions locales  *|
\***************************************/

/**
 * @brief Initialise tous les modules
 * @return EXIT_FAILURE si erreur, EXIT_SUCCESS sinon
 */
int8_t starter_New()
{
    postman_New();
    dispatcher_New();
    /* ui_New();*/
    int8_t err = EXIT_SUCCESS;
    err &= driverCAN_New();
    err &= sender_New();
    err &= sniffer_New();
    return err;
}

/**
 * @brief Démarre tous les modules
 * @return EXIT_FAILURE si erreur, EXIT_SUCCESS sinon
 */
int8_t starter_Start()
{
    postman_Start();
    dispatcher_Start();
    /* ui_Start() */
    int8_t err = EXIT_SUCCESS;
    err &= sender_Start();
    err &= sniffer_Start();
    return err;
}

/**
 * @brief Arrête tous les modules
 * @return EXIT_FAILURE si erreur, EXIT_SUCCESS sinon
 */
int8_t starter_Stop()
{
    int8_t err = EXIT_SUCCESS;
    err &= sniffer_Stop();
    err &= sender_Stop();
    /* ui_Stop() */
    dispatcher_Stop();
    postman_Stop();
    return err;
}

/**
 * @brief Déinitialise tous les modules
 * @return EXIT_FAILURE si erreur, EXIT_SUCCESS sinon
 */
int8_t starter_Free()
{
    int8_t err = EXIT_SUCCESS;
    err &= sniffer_Free();
    err &= sender_Free();
    err &= driverCAN_Free();
    /* ui_Free() */
    dispatcher_Free();
    postman_Free();
    return err;
}

/**
 * @brief Handler du signal SIGINT
 * @param signal argument du signal
 */
void signalSigintHandler(int signal)
{
    fprintf(stdout, "SIGINT intercepté : demande d'arrêt du programme\n");
    sem_post(&sem);
}

/*************************\
|*	Description du main  *|
\*************************/

/**
 * @fn main
 *
 * @brief Fonction principale du module.
*/
int main(int argc, char *argv[])
{
    /* Intitalisation du sémaphore */
    if (sem_init(&sem, 0, 0) == -1)
    {
        perror("sem_init");
        return EXIT_FAILURE;
    }

    struct sigaction action;

	/* Interception du SIGTERM pour sortie propre */
	action.sa_handler = signalSigintHandler;
	sigemptyset(&(action.sa_mask));
	action.sa_flags = SA_RESTART;

	sigaction(SIGTERM, &action, NULL);

    int8_t err = EXIT_SUCCESS;

    /* Initialisation des modules */
    err &= starter_New();
    err &= starter_Start();

    /* Attente de la demande de la fin du programme */
    sem_wait(&sem);

    /* Arrêt des modules */
    err &= starter_Stop();
    err &= starter_Free();

    /* Destruction du sémaphore */
    if (sem_destroy(&sem) == -1)
    {
        perror("sem_destroy");
        return EXIT_FAILURE;
    }

    return err;
}
