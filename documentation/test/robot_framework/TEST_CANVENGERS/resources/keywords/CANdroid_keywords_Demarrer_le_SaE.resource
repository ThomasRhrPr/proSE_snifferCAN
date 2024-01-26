#
# Ressource RobotFramework pour le test du Chat (Felix/Camix).
#
# Version pour l'implémentation Android de Felix.
#
# Fournit l'implémentation des keywors des points des dossier_temporaire des spécifications du Chat.
# Implémentation des manipulations de Felix et Camix avec les bibliothèques Process et AppiumLibrary.
#
# @version 0.4.3.etu.0
# @author Matthias Brun
#

*** Settings ***

Resource    resources/keywords/CANdroid_keywords_temp.resource

Library    AppiumLibrary
Library    WatchUI
Library    OperatingSystem
Library    Dialogs

*** Variables ***

${SCREENSHOT_NAME}   screenshot.png
${TEMP_FOLDER}  temp_folder

*** Keywords ***

#_______________ Démarrer le SàE (Scénario nominal) ________________________________________#

# 1.
Utilisateur met en fonctionnement le Simulateur ICSim
    # Vue PC -> Démarre Simulateur ICSim
    Pause Execution    Utilisateur démarre le simulateur ICSim sur E_PC
    # Vue PC -> Affiche Simulateur ICSim
    Execute Manual Step    Le Simulateur ICSim est affiché sur E_PC ?

# 2.
Utilisateur connecte la Raspberry Pi au bus CAN
    # Vue Raspberry Pi -> Connecte la Raspberry Pi au bus CAN
    Pause Execution    Utilisateur connecte la Raspberry Pi au bus CAN

# 3.
Utilisateur met sous tension la Raspberry Pi
    # Vue Raspberry Pi -> Branche Raspberry Pi
    Pause Execution    Utilisateur branche la Raspberry Pi
    # Vue Raspberry Pi -> Allume la LED de tension
    Execute Manual Step    La LED de tension est allumée sur la Raspberry Pi ?

# 4.
Utilisateur lance le programme CANgateway via la connexion SSH
    # Vue PC -> Démarre le programme CANgateway
    Pause Execution     Utilisateur démarre le programme CANgateway sur E_PC

# 5.
E_PC informe Utilisateur que le programme CANgateway est lancé
    # Vue PC -> Affiche le message "Lancement du programme" sur la console
    Execute Manual Step     "Lancement du programme" est-il affiché sur la console ?

# 6.
Utilisateur connecte E_Smartphone au réseau TCP/IP de E_Raspberry
    [Arguments]    ${reseauWifi}    ${motDePasse}
    # Vue Smartphone -> Connecte le smartphone au reseau wifi en utilisant le mot de passe
    Pause Execution     Utilisateur connecte le smartphone à ${reseauWifi} en utilisant ${motDePasse}


# 7.
E_Smartphone informe Utilisateur qu'il est connecté au réseau TCP/IP de E_Raspberry
    [Arguments]    ${reseauWifi}
    # Vue Smartphone -> Affiche le reseau wifi dans "Réseau actuel"
    Execute Manual Step     ${reseauWifi} est-il affiché dans "Réseau actuel" ?

Utilisateur démarre l’application CANdroid
    L'utilisateur lance l'exécution du composant CANdroid

L’application CANdroid affiche EcranPrincipal
    log to console    Vérifier que EcranPrincipal est affiché avec les logos grisés
    # TODO modifier les coordonnées en dur pour les icon ?
    # TODO utiliser "execute manual step" pour vérifier l'état de l'icon ?

    #CANdroid affiche le bouton [connexion] grisé et barré

    #CANdroid affiche le bouton [lancerEnvoi] grisé

    #CANdroid affiche le bouton [play] grisé

L’application CANdroid se connecte au programme CANgateway
    # Vue Smartphone : Connecte le Smartphone au réseau TCP/IP de la Raspberry Pi
    Pause Execution    Utilisateur connecte le Smartphone au réseau TCP/IP de la Raspberry Pi
    # Vue Smartphone : Affiche que la connexion avec la Raspberry Pi est établie
    execute manual step    L'icon wifi est affiché en haut à droite de l'écran ?
    #_________________________________________________________________________#
    #   TODO Début pour connecter automatiquement CANdroid au réseau de la Pi
    #Open Application    http://localhost:4723/wd/hub    platformName=Android
    #...    appPackage=com.android.settings
    #...    appActivity=.Settings
    #...    alias=Paramètres

    #Click Element    xpath=//android.widget.LinearLayout[1]
    #_________________________________________________________________________#

L’application CANdroid met à jour EcranPrincipal
    log to console    Vérifier que EcranPrincipal est affiché avec les logos colorés
    # TODO vérifier la mise à jour des icon une fois la connexion établie

CANdroid affiche le bouton [connexion] grisé et barré
    Create Directory    ${TEMP_FOLDER}
    # on récupère une capture d'écran de l'application CANdroid
    # TODO ${SCREENSHOT_NAME} s'enregistre automatiquement dans le dossier "report" je ne peux pas le modifier ...
    Capture Page Screenshot    ${SCREENSHOT_NAME}
    # on récupère uniquement l'icon qui nous interesse sur la capture d'écran
    Crop Image    report/${SCREENSHOT_NAME}     69       8      166     104     ${TEMP_FOLDER}
    # on récupère dans une liste la capture d'écran de l'icon que l'on veut
    ${files}       OperatingSystem.List Files In Directory    ${TEMP_FOLDER}
    # on compare l'image récupérée avec le template
    ${result}    Compare Images    ${TEMP_FOLDER}/${files}[0]    image/rasprgb.png
    # on fait du nettoyage
    Remove File    report/${SCREENSHOT_NAME}
    Remove Directory    ${TEMP_FOLDER}      recursive=True
    log to console    fin CANdroid affiche le bouton [connexion] grisé et barré

CANdroid affiche le bouton [lancerEnvoi] grisé
    Create Directory    ${TEMP_FOLDER}
    # on récupère une capture d'écran de l'application CANdroid
    # TODO ${SCREENSHOT_NAME} s'enregistre automatiquement dans le dossier "report" je ne peux pas le modifier ...
    Capture Page Screenshot    ${SCREENSHOT_NAME}
    # on récupère uniquement l'icon qui nous interesse sur la capture d'écran
    Crop Image    report/${SCREENSHOT_NAME}     69       112      166     208     ${TEMP_FOLDER}
    # on récupère dans une liste la capture d'écran de l'icon que l'on veut
    ${files}       OperatingSystem.List Files In Directory    ${TEMP_FOLDER}
    # on compare l'image récupérée avec le template
    ${result}    Compare Images    ${TEMP_FOLDER}/${files}[0]    image/sentmode.png
    # on fait du nettoyage
    Remove File    report/${SCREENSHOT_NAME}
    Remove Directory    ${TEMP_FOLDER}      recursive=True
    log to console    fin CANdroid affiche le bouton [lancerEnvoi] grisé

CANdroid affiche le bouton [play] grisé
    Create Directory    ${TEMP_FOLDER}
    # on récupère une capture d'écran de l'application CANdroid
    # TODO ${SCREENSHOT_NAME} s'enregistre automatiquement dans le dossier "report" je ne peux pas le modifier ...
    Capture Page Screenshot     ${SCREENSHOT_NAME}
    # on récupère uniquement l'icon qui nous interesse sur la capture d'écran
    Crop Image    report/${SCREENSHOT_NAME}     20      166     208     ${TEMP_FOLDER}
    # on récupère dans une liste la capture d'écran de l'icon que l'on veut

#_______________ Démarrer le SàE (Scénario nominal) ________________________________________#