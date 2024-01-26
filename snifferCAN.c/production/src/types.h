/**
 * @file types.h
 *
 * @brief Fichier header contenant les types de données utilisés dans le programme
 *
 * @version : 0.0.2
 *
 * @date 13/05/2023
 *
 * @author Elisa DECLERCK
 *
 * @copyright © Copyright Équipe ProSE B1 2024 CANvengers
*/

#ifndef TYPES_H_
#define TYPES_H_

#include <linux/can.h>
#include <stdint.h>

/*************\
|*	TypeDef  *|
\*************/

typedef enum
{
    ERROR_STATE_SUCCESS = 0,
    ERROR_STATE_FAILURE,
    ERROR_STATE_TIMEOUT,
} ErrorState_e;

typedef enum
{
    SENDER_MODE_PUNCTUAL = 0,
    SENDER_MODE_CYCLIC
} SenderMode_e;

typedef enum
{
    SENDING_STATE_ON = 0,
    SENDING_STATE_OFF
} SendingState_e;
typedef struct 
{
    struct can_frame frame;
    SenderMode_e senderMode;
    uint16_t periodicity;
} Frame_t;

#endif /* TYPES_H_ */