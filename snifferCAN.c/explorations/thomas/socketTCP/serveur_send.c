#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "serveur.h"
#include <linux/can.h>

// Nombre de connexion simultanée possible
#define BACKLOG 1

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
    listen(socket_ecoute, BACKLOG);

    while(1){
        
        // Si on avait juste "socket_donnees = accept(socket_ecoute, NULL, 0);", on ne pourrait pas récupérer l'IP du client
        struct sockaddr_in adresse_client;
        socklen_t taille_adresse_client = sizeof(adresse_client);
        
        /* Bloque l'exécution du programme jusqu'à ce qu'une connexion soit reçue.
         * Lorsqu'une connexion est reçue, accept() retourne un nouveau descripteur de fichier utilisé pour communiquer avec le client connecté. 
         * Ce descripteur de fichier est stocké dans la variable socket_donnees. */
        socket_donnees = accept(socket_ecoute, (struct sockaddr *)&adresse_client, &taille_adresse_client);

        struct can_frame frame;

        /* En définissant la structure frame de cette manière, on spécifie que les données de la trame sont stockées en mémoire sous la forme de valeurs binaires, codées sur 8 bits chacune.*/
        frame.can_id = 0x123;
        frame.len = 8;
        frame.data[0] = 0x01;
        frame.data[1] = 0x23;
        frame.data[2] = 0x45;
        frame.data[3] = 0x67;
        frame.data[4] = 0x89;
        frame.data[5] = 0xAB;
        frame.data[6] = 0xCD;
        frame.data[7] = 0xEF;
        char * chaine = can_frame_to_string(&frame);
        //printf("%s\n", str);
        // Envoi de la chaîne de caractères au client
        int size = strlen(chaine);
        if (send(socket_donnees, chaine, size, 0) == -1) {
            perror("Erreur lors de l'envoi des données au client");
        }
        
        // Nettoyage de la mémoire
        free(chaine);
    }
    close(socket_ecoute);
}