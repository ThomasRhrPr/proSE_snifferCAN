

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/can.h>
#include "driverCAN/driverCAN.h"
#include "sniffer/sniffer.h"
#include "sender/sender.h"
#include "postman/postman.h"

int main(int argc, char *argv[])
{
    postman_Free();
    /* Initialisation du module driverCAN */
    int err = driverCAN_New();

    /* Initialisation du module sniffer */
    err &= sniffer_New();
    err &= sniffer_Start();
    err &= sniffer_StartReading();

    /* Initialisation du module sender */
    err &= sender_New();
    err &= sender_Start();

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
        .periodicity = 1500,
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

    Frame_t framesToSend[3] = {frame1, frame2, frame3};
    /* Début de l'envoi */
    err &= sender_StartSending(framesToSend, 3);

    sleep(30);

    sender_StopSending();
    sniffer_StopReading();
    sender_Stop();
    sniffer_Stop();

    sender_Free();
    sniffer_Free();


    return err;
}
