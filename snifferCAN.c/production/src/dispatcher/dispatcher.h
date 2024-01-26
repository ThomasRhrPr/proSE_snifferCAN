/**
 * @file dispatcher.h
 *
 * @brief Fichier header du module dispatcher qui permet de décoder l'information reçue via le protocole de communication TCP/IP, pour la fournir à Sender.
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

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

/*****************************\
|*	Déclaration des includes *|
\*****************************/

#include "../types.h"

/****************************************\
|*	Déclaration des fonctions globales  *|
\****************************************/

ErrorState_e dispatcher_New();

ErrorState_e dispatcher_Start();

ErrorState_e dispatcher_Stop();

ErrorState_e dispatcher_Free();

#endif /* DISPATCHER_H_ */