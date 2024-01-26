

#include <stdio.h>
#include <unistd.h>
#include "driverCAN/driverCAN.h"
#include "sniffer/sniffer.h"

int main(int argc, char *argv[])
{
    /* Initialisation du module driverCAN */
    driverCAN_New();

    /* Initialisation du module sniffer */
    sniffer_New();
    sniffer_Start();
    sniffer_StartListening();

    sleep(30);

    sniffer_StopListening();
    sniffer_Stop();
    sniffer_Free();
    
    driverCAN_Free();
    
    return 0;
}
