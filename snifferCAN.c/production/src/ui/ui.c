/**
 * @file ui.c
 *
 * @brief Fichier source du module ui qui permet d'informer l'utilisateur de l'état du programme.
 *
 * @version 0.1.0
 *
 * @date 31/05/2023
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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ui.h"
#include "../sniffer/sniffer.h"

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/

typedef enum
{
    PROGRAM_STATE_ON = 0,
    PROGRAM_STATE_OFF,
} ProgramState_e;


/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static ErrorState_e signalState(ProgramState_e p_state);

/****************************************\
|*	Déclaration des fonctions globales  *|
\****************************************/

/**
 * @fn ErrorState_e ui_StartCANgateway()
 * 
 * @brief Fonction qui permet de lancer le programme
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e ui_StartCANgateway()
{
    ErrorState_e l_err = signalState(PROGRAM_STATE_ON);
    l_err |= sniffer_StartReading();
    return (l_err);
}

/**
 * @fn ErrorState_e ui_StopCANgateway()
 * 
 * @brief Fonction qui permet d'arrêter le programme
 * 
 * @return L'état d'erreur de la fonction
 */
ErrorState_e ui_StopCANgateway()
{
    ErrorState_e l_err = signalState(PROGRAM_STATE_OFF);
    l_err |= sniffer_StopReading();
    return (l_err);
}

/***************************************\
|*	Description des fonctions locales  *|
\***************************************/

/**
 * @fn static ErrorState_e signalState(ProgramState_e p_state)
 * 
 * @brief Fonction qui permet d'afficher l'état du programme
 * 
 * @param p_state état du programme
 * 
 * @return L'état d'erreur de la fonction
 */
static ErrorState_e signalState(ProgramState_e p_state)
{
    switch (p_state)
    {
        case PROGRAM_STATE_ON:
            fprintf(stdout, "Lancement du programme\n");
            return ERROR_STATE_SUCCESS;
            break;
        case PROGRAM_STATE_OFF:
            fprintf(stdout, "Arrêt du programme\n");
            return ERROR_STATE_SUCCESS;
            break;
        default:
            fprintf(stderr, "Erreur : état du programme inconnu\n");
            return ERROR_STATE_FAILURE;
            break;
    }
}