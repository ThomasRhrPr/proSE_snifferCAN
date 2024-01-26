/* FICHIER POUR TESTER LE FONCTIONNEMENT DU MODULE DRIVERCAN 
    A COPIER COLLER DANS LE MAIN DU DOSSIER DE PRODUCTION */

#include <stdio.h>
#include <stdlib.h>
#include "driverCAN/driverCAN.h"
#include <pthread.h>

pthread_t thread;
pthread_t thread2;

void * Read(void * arg);
void * Read2(void * arg);

int main(int argc, char *argv[])
{
    /*
        exemple d'utilisation de la librairie driverCAN
    */
    /*
        Initialisation du driver
    */
    if (driverCAN_New() == 1)
    {
        printf("Erreur d'initialisation du driver CAN\n");
        return EXIT_FAILURE;
    }

    pthread_create(&thread, NULL, Read, NULL);

    while(1)
    {

    }
    return EXIT_SUCCESS;
}

void * Read(void * arg)
{
    pthread_create(&thread2, NULL, Read2, NULL);
    while(1)
    {

    }
}

void * Read2(void * arg)
{
    while(1)
    {
        /* Lecture de trames CAN */
        printf("Lecture de trames CAN\n");
        struct can_frame frameRead;
        if (driverCAN_Read(&frameRead) == EXIT_SUCCESS)
        {
            printf("Trame : %x  ", frameRead.can_id);
            printf("%d  ", frameRead.can_dlc);
            printf(" ");
            for (int i = 0; i < frameRead.can_dlc; i++)
            {
                printf("%x ", frameRead.data[i]);
            }
            printf("\n");
        }
        else
        {
            printf("Erreur de lecture de trames CAN\n");
        }
    }
}