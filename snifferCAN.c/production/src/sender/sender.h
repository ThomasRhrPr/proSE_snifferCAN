/**
 * @file sender.h
 *
 * @brief Fichier header du module sender qui permet d'envoyer les trames CAN sur le bus.
 *
 * @version 0.2.0
 *
 * @date 09/05/2023
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

#ifndef SENDER_H_
#define SENDER_H_

/**************\
|*	Includes  *|
\**************/

#include "../types.h"

/****************************************\
|*	Déclaration des fonctions globales  *|
\****************************************/

ErrorState_e sender_New();

ErrorState_e sender_Start();

ErrorState_e sender_StartSending(Frame_t p_frames[], uint8_t nb_frames);

ErrorState_e sender_StopSending();

ErrorState_e sender_AskSendingState();

ErrorState_e sender_Stop();

ErrorState_e sender_Free();

#endif /* SENDER_H_ */