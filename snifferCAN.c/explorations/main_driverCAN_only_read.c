/* FICHIER POUR TESTER LE FONCTIONNEMENT DU MODULE DRIVERCAN 
    A COPIER COLLER DANS LE MAIN DU DOSSIER DE PRODUCTION */

#include <stdio.h>
#include <stdlib.h>
#include "driverCAN/driverCAN.h"

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

    while(1)
    {
        /* Lecture de trames CAN */
        printf("Lecture de trames CAN\n");
        struct can_frame frameRead;
        if (driverCAN_Read(&frameRead) == EXIT_SUCCESS)
        {
            printf("ID : %x\n", frameRead.can_id);
            printf("DLC : %d\n", frameRead.can_dlc);
            printf("Data : ");
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
    return EXIT_SUCCESS;
}