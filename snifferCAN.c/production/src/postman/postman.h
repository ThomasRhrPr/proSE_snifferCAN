/**
 * @file postman.h
 *
 * @brief Fichier header du module postman qui réalise l'envoi et la réception des sockets.
 *
 * @version : 0.2.2
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

#ifndef POSTMAN_H_
#define POSTMAN_H_

/*****************************\
|*	Déclaration des includes *|
\*****************************/

#include <sys/time.h>
#include <sys/select.h>
#include <stdbool.h>

#include "../types.h"

/****************************\
|*	Déclaration des defines *|
\****************************/

#define MESSAGE_SIZE (31)

/****************************************\
|*	Déclaration des fonctions globales  *|
\****************************************/

ErrorState_e postman_New();

ErrorState_e postman_Start();

ErrorState_e postman_ReceiveData(char *p_char_pt, bool *p_firstTime_pt);

ErrorState_e postman_SendMessage(char *p_message_pt);

ErrorState_e postman_Stop();

ErrorState_e postman_Free();

#endif /* POSTMAN_H_ */
