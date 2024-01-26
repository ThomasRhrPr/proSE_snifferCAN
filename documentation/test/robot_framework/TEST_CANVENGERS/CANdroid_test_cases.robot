#
# Test RobotFramework du Chat (Felix/Camix).
#
# Tests de connexions possibles et impossibles au serveur Camix,
# basés sur les Cas d'Utilisation (dossier_temporaire).
#
# @version 0.4.3.etu.0
# @author Matthias Brun
#

*** Settings ***

Resource    resources/android/CANdroid_config.resource
Resource    resources/android/CANdroid_CU.resource

# Resource créée pour dispatcher ce fichier en différent sous fichier
#Resource    resources/dossier_temporaire/CANdroid_CU_Reconnecter_application_CANdroid.resource
#Resource    resources/dossier_temporaire/CANdroid_CU_Ajouter_un_objet.resource
#Resource    resources/dossier_temporaire/CANdroid_CU_Ajouter_une_trame.resource

Suite Setup         Exécution SUT (sur cible)
Suite Teardown      Arrêt SUT (sur cible)

*** Test Cases ***

Démarrer le SàE
    # Préconditions :
    #   - L'application CANdroid est installée sur le Smartphone.
    #   - Le programme CANgateway est installée sur la Raspberry Pi.
    #   - Utilisateur est connecté à la Raspberry Pi en SSH.
    #   - Simulateur ICSim est installé sur le PC.
    #   - Le matériel est fonctionnel.

    [Teardown]    Arrêt CANdroid
    Démarrer le SàE

Reconnecter l’application CANdroid
    # Préconditions :
    #   - Le SàE est démarré.
    #   - L'application CANdroid est déconnecté du programme CANgateway par coupure du wifi sur Smartphone.
    #   - EcranPrincipal :
    #       - Le bouton [connexion] est grisé et barré
    #       - Le bouton [lancerEnvoi] est grisé
    #       - Le bouton [play] est grisé

    [Teardown]    Arrêt CANdroid
    Reconnecter l’application CANdroid

#Recevoir des trames
    # Préconditions :
    #   - Le SàE est démarré
    #   - L’application CANdroid est connectée au programme CANgateway
    #   - La Raspberry Pi est connectée au bus CAN
    #   - Tableau de Bord est connecté au bus CAN

    #[Teardown]    Arrêt CANdroid
    #[Template]    Recevoir des trames
    #Clignotant droit       02 00 00 00
    #Clignotant gauche      01 00 00 00

#Interagir avec le sniffer
    # Préconditions :
    #   - Variable en fonction de la variante sélectionnée car pas de scénario nominal

    # TODO
    #Interagir avec le sniffer

#Ajouter un objet
    # Précondition :
    #   - L'application CANdroid est démarrée.
    #   - ${objet} n'existe pas encore.
    #   - Le nombre maximal d'objet n'a pas été atteint.

    #[Teardown]    Arrêt CANdroid
    #Ajouter un objet

Ajouter une trame
    # Préconditions :
    #   - L'application CANdroid est démarrée.
    #   - Un ${objet} est créé.
    #   - Le nombre maximal de trames n'a pas été atteint dans ${objet}.
    #   - Aucune trame n'est créée dans ${objet}.

    [Teardown]    Arrêt CANdroid
    Ajouter une trame
    Portières    "#19B$6@00000E000000"    100

#Envoyer des trames
    # Précondition :
    #   - Le SàE est démarré.
    #   - L'objet ${objet} a été créé.
    #   - La trame ${trame} a été crée.
    #   - L'application CANdroid est connectée programme CANgateway.
    #   - La Raspberry Pi est connectée au bus CAN.
    #   - Un tableau de bord est connecté au bus CAN.
    #   - Aucun envoi de trames n'est en cours.

    #[Teardown]    Arrêt CANdroid
    #Envoyer des trames

#Arrêter l’envoi des trames
    # Préconditions :
    #   - Le SàE est démarré.
    #   - Un ${objet} a été créé
    #   - Une ${trame} cyclique a été enregistrée
    #   - Cette ${trame} cyclique a été envoyé

    #[Teardown]    Arrêt CANdroid
    #Arrêter l’envoi des trames

#Supprimer un élément
    # Préconditions:
    #   - CANdroid est démarré.
    #   - Un ${objet} est créé.
    #   - Des ${trames} sont créées dans ${objet}.

    #[Teardown]    Arrêt CANdroid
    #Supprimer un élément

#Stopper le SàE
    # Préconditions:
    #   - Le SàE est démarré.
    #   - Le logiciel ICSim est démarré sur le PC.

    #Stopper le SàE