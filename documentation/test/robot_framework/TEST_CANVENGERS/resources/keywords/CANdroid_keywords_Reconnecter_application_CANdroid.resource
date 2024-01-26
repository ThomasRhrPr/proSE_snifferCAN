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

Resource    resources/keywords/CANdroid_keywords.resource

Library    Process
Library    AppiumLibrary
Library    WatchUI
Library    Screenshot
Library    OperatingSystem
Library    Dialogs

*** Variables ***

${SCREENSHOT_NAME}   screenshot.png
${TEMP_FOLDER}  temp_folder

*** Keywords ***

#_______________ Reconnecter l’application CANdroid (Scénario nominal) _____________________#

Utilisateur demande la reconnexion de l’application CANdroid au programme CANgateway
    # Vue Smartphone :
    #   -> Réactive le wifi sur Smartphone
    #   -> Utilisateur peut reconnecter l’application CANdroid au programme CANgateway
    pause execution     Utilisateur réactive le wifi sur Smartphone
    # Vue Écran Principal -> Clique [connexion]
    click element       id=main_imagebutton_connection      #TODO modifer avec une variable ${ID_BOUTON_RECONNEXION}

L’application CANdroid aﬀiche PopupDemandeReconnexion
    # Vue popup demande reconnexion -> Affiche PopupDemandeReconnexion
    element should be visible    id=PopupDemandeReconnexion     #TODO modifer avec une variable ${ID_POPUP_RECONNEXION}

Utilisateur valide la reconnexion
    # Vue popup demande reconnexion -> Clique [validerReconnexion]
    click element       id=      #TODO modifer avec une variable ${ID_BOUTON_VALIDER_RECONNEXION}

L’application CANdroid affiche EcranPrincipal
    # Affiche Ecran Principal :
    #   -> le bouton [connexion] est grisé et barré
    #   -> le bouton [lancerEnvoi] est grisé
    #   -> le bouton [play] est grisé
    log to console    les boutons sont grisés

L’application CANdroid se reconnecte au programme CANgateway
    L’application CANdroid se connecte au programme CANgateway

L’application CANdroid met à jour EcranPrincipal
    # Affiche Ecran Principal :
    #   -> le bouton [connexion] est coloré
    #   -> le bouton [lancerEnvoi] est coloré
    #   -> le bouton [play] est coloré
    L’application CANdroid met à jour EcranPrincipal


#_______________ Reconnecter l’application CANdroid (Scénario nominal) _____________________#
