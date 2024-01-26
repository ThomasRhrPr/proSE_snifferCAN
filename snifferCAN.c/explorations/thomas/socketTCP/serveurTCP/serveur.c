#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/can.h>
#include "serveur.h"

// Nombre de connexion simultanée possible
#define BACKLOG 2

int periodicity = 0;
int * ptr_periodicity = &periodicity;

void string_to_can_frame(char* message, struct can_frame * frame) {
    // Get CAN ID
    char* id_str = message + 1; // Skip the '#'
    /* Convertit chaine de caractère en long int
     * 16 -> base 16 (hexadécimal)
     * pas besoin de faire de délimitation 'end' car la conversion s'arrête dès qu'un caractère non hexa est rencontré */
    frame->can_id = strtol(id_str, NULL, 16);

    // Get length
    /* strchr retourne un pointeur vers '$' et comme on en veut pas on fait +1
     * atoi retourne l'entie correspondant à la chaine de caractère en paramètre */
    char* len_str = strchr(message, '$') + 1;
    frame->len = atoi(len_str);

    // Get data
    char* data_str = strchr(message, '@') + 1;

    for (int i = 0; i < frame->len; i++) {
        /* pour stocker dans une sous chaine les 2 caractères correspond à un octet.
         * byte_str[3] car 2 caractères + \0 = 3 caractères 
         * data_str[i*2] -> i*2 car on va de 2 en 2 donc le 1er élément d'une sous chaine est forcément i*2
         * data_str[i*2+1] -> i*2+1 pour l'élément suivant */
        char byte_str[3] = {data_str[i*2], data_str[i*2+1], '\0'};
        frame->data[i] = strtol(byte_str, NULL, 16);
    }

    // Get periodicity
    char* period_str = strchr(message, '/') + 1;
    *ptr_periodicity = atoi(period_str);
}

char * can_frame_to_string(struct can_frame * frame)
{
    
    // sprintf() retourne le nombre de caractères écrits dans la chaine.
    char str_id[20];
    int id_size = sprintf(str_id, "%X", frame->can_id);
    int buffer_size = (frame->len * 2) + 1 + 3 + 1 + id_size;
    
    /* +1 pour le null_byte
     * +3 pour les séparateurs
     * +1 pour len
     * + id_size pour id 
     */ 
    char * buffer = (char *)malloc(buffer_size * sizeof(char));
    
    /* 'sprintf' permet de formater le buffer à partir des informations en paramètres
     * On ajoute l'ID en hexa (%X) et la taille en décimal (%clead).
     * On met str et pas &str car str est déjà un pointeur vers le 1er élément de la chaine de caractère*/
    sprintf(buffer, "#%X$%d@", frame->can_id, frame->len);
    for (int i = 0; i < frame->len; i++)
    {
        /* buffer + strlen(buffer) pour donner la position à laquelle il faut commencer à écrire
         * "%02X" avec : 
         - X pour le résultat en hexa
         - '0' pour dire que le remplissage se fera avec des '0'
         - '2' pour indiquer que la largeur sera de 2 caractères
         */
        sprintf(buffer + strlen(buffer), "%02X", frame->data[i]);
    }
    return buffer;
}


int main(){
    // Descripteur de fichier qui sera utilisé pour écouter les connexions entrantes.
    int socket_ecoute;
    // Descripteur de fichier qui sera utilisé pour communiquer avec un client connecté.
    int socket_donnees;    
    // Structure qui contient l'adresse IP et le numéro de port du serveur.
    struct sockaddr_in mon_adresse;

    /* Créer un socket écoutant utilisant :
     * - PF_INET pour utiliser IPv4
     * - SOCK_STREAM pour utiliser les connexions TCP (informations envoyées en flux de données)
     * - 0 pour indiquer que le protocole sera choisi automatiquement */
    socket_ecoute = socket (PF_INET, SOCK_STREAM, 0);

    // Cette ligne indique que la famille d'adresse utilisée est IPv4.
    mon_adresse.sin_family = AF_INET;

    /* htons convertit le numéro de port de l'ordre des octet de l'hôte à l'ordre des octets du réseau. 
     * Cela permet de s'assurer que l'ordre des octets est correct, car des architectures matérielles différentes peuvent stockers les octets dans un ordre différents. */
    mon_adresse.sin_port = htons(PORT_DU_SERVEUR);

    /* Permet de spécifier l'IP sur laquelle le serveur va écouter les connexions entrantes.
     * INADDR_ANY signifie que le serveur doit écouter les connexions entrantes sur toutes les interfaces réseau disponibles.
       -> le serveur sera en mesure d'écouter les connexions sur n'importe quelle adresse IP qui est disponible sur la machine.
     * htonl pour convertir l'ordre des octets de l'hôte à l'ordre des octets du réseau pour l'adresse IP spécifiée. 
      -> s'assure que l'IP est correctement encodée, car différents types de processeurs ont des représentations différentes des octets. */
    mon_adresse.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Associer le socket à l'adresse et au port du serveur.
     * Renvoie 0 en cas de succès et -1 en cas d'erreur. */
    if (bind(socket_ecoute, (struct sockaddr*) &mon_adresse, sizeof(mon_adresse)) == -1) {
        perror("Erreur lors de l'association du socket à l'adresse du serveur");
        exit(EXIT_FAILURE);
    }

    /* Permet de mettre en écoute un socket pour les connexions entrantes. 
     * Envoie une demande au système d'exploitation pour qu'il surveille le socket et accepte les connexions entrantes. 
     * Après cela, le socket est en attente de connexions entrantes et toute tentative de connexion sera enregistrée dans la file d'attente de connexions. */
    if (listen(socket_ecoute, BACKLOG) == -1) {
        perror("Erreur lors de l'appel à la fonction listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        
        // Si on avait juste "socket_donnees = accept(socket_ecoute, NULL, 0);", on ne pourrait pas récupérer l'IP du client
        struct sockaddr_in adresse_client;
        socklen_t taille_adresse_client = sizeof(adresse_client);
        
        /* Bloque l'exécution du programme jusqu'à ce qu'une connexion soit reçue.
         * Lorsqu'une connexion est reçue, accept() retourne un nouveau descripteur de fichier utilisé pour communiquer avec le client connecté. 
         * Ce descripteur de fichier est stocké dans la variable socket_donnees. */
        socket_donnees = accept(socket_ecoute, (struct sockaddr *)&adresse_client, &taille_adresse_client);

        /////recevoir/////
        char buffer[1024];
        int rcv_str_size = recv(socket_donnees, buffer, sizeof(buffer), 0);
        //printf("%d", rcv_str_size);
        CAN_frame frame;
        if (rcv_str_size == -1) {
            perror("Erreur lors de la réception des données du client");
        }
        else if (rcv_str_size == 0) {
            printf("Déconnexion du client\n");
            //close(socket_donnees);
        }
        else {
            //on prend la taille du buffer et on finit la chaine par le byte nulle
            buffer[rcv_str_size] = '\0';
            string_to_can_frame(buffer, &frame);
            // 0x%08X 08X pour avoir un remplissage hexadécimal de 8 caractères.
            printf("can_id: 0x%08X\n", frame.can_id);
            printf("len: %d\n", frame.len);
            printf("data: ");
            for (int i = 0; i < frame.len; i++) {
                printf("%02X ", frame.data[i]);
            }
            printf("\n");
            printf("periodicity : %d\n\n\n", *ptr_periodicity);
            //close(socket_donnees);        
        }


        /////envoyer/////
        struct can_frame frame_send;

        /* En définissant la structure frame de cette manière, on spécifie que les données de la trame sont stockées en mémoire sous la forme de valeurs binaires, codées sur 8 bits chacune.*/
        frame_send.can_id = 0x123;
        frame_send.len = 8;
        frame_send.data[0] = 0x01;
        frame_send.data[1] = 0x23;
        frame_send.data[2] = 0x45;
        frame_send.data[3] = 0x67;
        frame_send.data[4] = 0x89;
        frame_send.data[5] = 0xAB;
        frame_send.data[6] = 0xCD;
        frame_send.data[7] = 0xEF;
        char * chaine = can_frame_to_string(&frame_send);
        //printf("%s\n", str);
        // Envoi de la chaîne de caractères au client
        int size = strlen(chaine);
        if (send(socket_donnees, chaine, size, 0) == -1) {
            perror("Erreur lors de l'envoi des données au client");
        }
        
        // Nettoyage de la mémoire
        free(chaine);
        close(socket_donnees);
    }
    close(socket_ecoute);
}