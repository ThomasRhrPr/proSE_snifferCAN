
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/can.h>
#include "driverCAN/driverCAN.h"
#include "sender/sender.h"


int main(int argc, char *argv[])
{
    /* Initialisation du module driverCAN */
    int err = driverCAN_New();

    /* Initialisation du module sender */
    err |= sender_New();

    /* Lancement du module sender */
    err |= sender_Start();

    /* Définition de trames à envoyer à des périodicités différentes */
    struct can_frame canFrame1 = 
    {
        .can_id = 0x123,
        .can_dlc = 4,
        .data = {0x00, 0x78, 0x95, 0x14}
    };
    Frame_t frame1 = 
    {
        .frame = canFrame1,
        .periodicity = 0,
        .senderMode = SENDER_MODE_PUNCTUAL
    };

    struct can_frame canFrame2 = 
    {
        .can_id = 0x213,
        .can_dlc = 3,
        .data = {0x7B, 0xA5, 0xCC}
    };
    Frame_t frame2 = 
    {
        .frame = canFrame2,
        .periodicity = 2000,
        .senderMode = SENDER_MODE_CYCLIC
    };

    struct can_frame canFrame3 = 
    {
        .can_id = 0x321,
        .can_dlc = 2,
        .data = {0xAA, 0xD8}
    };
    Frame_t frame3 = 
    {
        .frame = canFrame3,
        .periodicity = 1000,
        .senderMode = SENDER_MODE_CYCLIC
    };

    Frame_t framesToSend[3] = {frame2, frame1, frame3};
    /* Début de l'envoi */
    err |= sender_StartSending(framesToSend, 3);

    sleep(10);

    /* Arrêt de l'envoi des trames */
    err |= sender_StopSending();

    Frame_t framesToSend2[2] = {frame1, frame1};
    /* Début de l'envoi */
    err |= sender_StartSending(framesToSend2, 2);

    sleep(10);

    struct can_frame canFrame4 = 
    {
        .can_id = 0x456,
        .can_dlc = 2,
        .data = {0xBB, 0xFF}
    };
    Frame_t frame4 = 
    {
        .frame = canFrame4,
        .periodicity = 900,
        .senderMode = SENDER_MODE_CYCLIC
    };
    struct can_frame canFrame5 = 
    {
        .can_id = 0x654,
        .can_dlc = 1,
        .data = {0x66}
    };
    Frame_t frame5 = 
    {
        .frame = canFrame5,
        .periodicity = 500,
        .senderMode = SENDER_MODE_CYCLIC
    };

    /* Arrêt de l'envoi des trames */
    err |= sender_StopSending();

    Frame_t framesToSend3[2] = {frame4, frame5};
    /* Début de l'envoi */
    err |= sender_StartSending(framesToSend3, 2);

    sleep(10);

    /* Arrêt de l'envoi des trames */
    err |= sender_StopSending();

    /* Arrêt du module */
    err |= sender_Stop();

    /* Désinitialisation */
    err |= sender_Free();

    err |= driverCAN_Free();
    
    return err;
}
