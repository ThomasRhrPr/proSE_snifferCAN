##### Connexion en ssh à la Raspberry Pi en utilisant une adresse IP statique

Ce document explique les étapes pour se connecter à la Raspberry Pi en utilisant un accès SSH et une adresse IP statique.

## Prérequis

Pour réaliser ces étapes, vous devez :

- Avoir une Raspberry Pi
- Avoir installé OpenSSH sur la Raspberry Pi (ou tout autre logiciel de serveur SSH)
- Avoir un câble Ethernet pour connecter la Raspberry Pi à votre ordinateur portable
- Avoir un écran, un clavier et une souris pour accéder à la Raspberry Pi

## Configuration de l'adresse IP statique

1. Connectez l'écran, le clavier et la souris à la Raspberry Pi et allumez-la.

2. Connectez le câble Ethernet entre la Raspberry Pi et votre ordinateur portable.

3. Ouvrez un terminal sur la Raspberry Pi.

4. Modifiez le fichier dhcpd.conf en utilisant la commande suivante :
sudo nano /etc/dhcpcd.conf

5. Ajoutez les lignes suivantes à la fin du fichier pour configurer l'adresse IP statique :
interface eth0
static ip_address=10.42.0.42/24
static routers=10.42.0.1
static domain_name_servers=8.8.8.8

6. Enregistrez les modifications et quittez le fichier dhcpd.conf.

7. Redémarrez la Raspberry Pi en utilisant la commande suivante :
sudo reboot

8. Une fois que la Raspberry Pi a redémarré, ouvrez les paramètres réseau sur votre ordinateur portable et configurez les paramètres Ethernet pour IPv4 comme suit :
Adresse IP : 10.42.0.40
Masque de sous-réseau : 255.255.255.0
Passerelle : 10.42.0.1

## Connexion en SSH

1. Ouvrez un terminal sur votre ordinateur.

2. Entrez la commande suivante pour vous connecter à la Raspberry Pi en utilisant SSH :
ssh -p 2222 pi@10.42.0.42

Assurez-vous de remplacer a.out par le nom de votre fichier, 10.42.0.42 par l'adresse IP de votre Raspberry Pi, et 2222 par le numéro de port SSH que vous avez spécifié pour la connexion ssh.

3. Entrez le mot de passe suivant lorsque vous êtes invité à le faire :
network

## Transfert de fichiers vers la Raspberry Pi

1. Vous pouvez utiliser la commande scp pour transférer des fichiers depuis votre ordinateur vers votre Raspberry Pi. Voici un exemple de commande scp pour transférer un fichier nommé a.out depuis votre ordinateur vers votre dossier personnel sur la Raspberry Pi :
scp -P 2222 test.c pi@10.42.0.42:/home/pi/

2. Entrez le mot de passe suivant lorsque vous êtes invité à le faire :
network

Assurez-vous de remplacer test.c par le nom de votre fichier, 10.42.0.42 par l'adresse IP de votre Raspberry Pi, et 2222 par le numéro de port SSH que vous avez spécifié pour la connexion ssh.

Le dossier de destination sur la Raspberry Pi peut également être modifié. Dans l'exemple ci-dessus, le fichier est envoyé dans le dossier personnel de l'utilisateur pi. Si vous souhaitez envoyer le fichier dans un autre dossier, vous pouvez spécifier le chemin complet du dossier de destination à la place de /home/pi/.

N'oubliez pas que vous devez être connecté à votre Raspberry Pi en utilisant ssh avant de pouvoir utiliser la commande scp pour transférer des fichiers.


## Configurer la PI pour lancer le programme CANgateway au boot

1. Le programme ne pourra pas fonctionner si le bus CAN n'est pas fonctionnel. Pour cela, nous pouvons créer un service can0.service dans le dossier /etc/systemd/system/ :
    - Créez un fichier /etc/systemd/system/can0.service que vous éditez (en root)
    - Ecrivez :
        ```bash 
        [Unit]
        Description=CAN0 interface initialization

        [Service]
        ExecStart=/bin/bash -c "/sbin/ifconfig can0 down && /sbin/ip link set can0 type can bitrate 125000 && /sbin/ifconfig can0 up"

        [Install]
        WantedBy=multi-user.target
        ```
        Sauvegardez.
    - Tapez **sudo systemctl enable can0.service** pour activer le service 
    - Redémarrez la PI.
    - Pour vérifier que le CAN est bien enable, tapez : **sudo ip -details -statistics link show can0**. On doit vous répondre que le bus est dans l'état ERROR-ACTIVE (donc opérationnel).
    - (Optionnel) Vous pouvez vérifier que le service a bien démarré en tapant **systemctl status can0.service**. Le système doit être "loaded, inactive" et vous pouvez voir qu'il a exécuté les commandes du script pour enable le bus CAN.

2. Pour que le programme CANgateway démarre et soit fonctionnel, on peut utiliser la même méthode :
    - - Créez un fichier /etc/systemd/system/CANgateway.service que vous éditez (en root)
    - Ecrivez :
        ```bash 
        [Unit]
        Description=Start CANgateway
        Wants=can0.service

        [Service]
        ExecStart=/chemin/vers/le/programme (ici /home/pi/CANgateway.out)

        [Install]
        WantedBy=multi-user.target
        ```
        Sauvegardez.
    - Tapez **sudo systemctl enable CANgateway.service** pour activer le service 
    - Redémarrez la PI.
    - Vous pouvez confirmer que le programme est démarré en tapant **systemclt status CANgateway.service**. Il doit être "active".

Si un problème apparaît (le programme s'arrête de manière imprévue), vous pouvez le voir grace à **systemclt status CANgateway.service** où son état sera "failed" (avec les raisons du pourquoi).
Si vous souhaitez redémarrer le programme, vous pouvez le faire en tapant **sudo systemclt restart CANgateway.service** (refaite un status pour vérifier que le service a correctement redémarré).


    