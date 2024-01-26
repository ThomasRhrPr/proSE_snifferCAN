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

Resource    CANdroid_config.resource

#Library    Process
Library    AppiumLibrary
#Library    WatchUI
Library    Screenshot
Library    OperatingSystem
Library    Dialogs

*** Variables ***

# ID Bouton CANdroid
${ID_BOUTON_AJOUT_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id='ajouterObjet')]
${ID_BOUTON_VALIDER_AJOUT_OBJET}    locator=//android.widget.TextView[contains(@resource-id='popUpTrameBouton3')]
${ID_BOUTON_ANNULER_AJOUT_OBJET}    locator=//android.widget.AppCompatButton[contains(@resource-id='annulerModeEnvoi')]
${ID_BOUTON_FERMER_POPUP_ERREUR_AJOUT_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id='fermerErreurNombreObjet')]
${ID_BOUTON_DEPLIER_MENU_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id='replierMenuObjet')]
${ID_BOUTON_REPLIER_MENU_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id='deplierMenuObjet')]
${ID_BOUTON_AJOUT_TRAME}    locator=//android.widget.ImageButton[contains(@resource-id='enregistre')]
${ID_RADIO_BOUTON_PONCTUEL}    locator=//android.widget.ImageButton[contains(@resource-id='radio_ponctuel')]
${ID_BOUTON_VALIDER_MODE_ENVOI}    locator=//android.widget.TextView[contains(@resource-id='ajouterTrame')]

# ID Popup CANdroid
${ID_POPUP_AJOUT_OBJET}     locator=//android.widget.TextView[contains(@resource-id='popUpObjetTitre')]
${ID_POPUP_ERREUR_NOMBRE_OBJET}   locator=//android.widget.TextView[contains(@resource-id='popupErreurAjoutObjetText')]
${ID_POPUP_ERREUR_AJOUT_OBJET}   locator=//android.widget.TextView[contains(@resource-id='popupErreurSaisieObjetTitre')]

# ID Champ de texte
${ID_CHAMP_TEXTE_NOM_OBJET}     locator=//android.widget.EditText[contains(@resource-id='popupObjet_editText')]
${ID_CHAMP_TEXTE_AJOUT_TRAME}    locator=//android.widget.EditText[contains(@resource-id='champTrame')]
${ID_CHAMP_PERIODICITE}    locator=//android.widget.EditText[contains(@resource-id='editText_periodicity')]

# Variable pour le dossier_temporaire ajouter un objet
${NOM_OBJET_PAR_DEFAUT}     nom_objet_par_faut
${NOM_OBJET_SAISI}  nom_objet_saisi
${ID_NOUVEL_OBJET_AJOUTE}   locator=android=resourceId("object1_fragment")

# Variable pour le CU ajouter une trame
${ID_NOUVELLE_TRAME_AJOUTEE}    locator=//android.widget.TextView[contains(@resource-id='frame1')]
${ID_OBJET_DEPLIE}    locator=//android.widget.TextView[contains(@resource-id='objet_deplie')]
${ID_OBJET_REPLIE}    locator=//android.widget.TextView[contains(@resource-id='objet_replie')]


*** Keywords ***

#_______________ Démarrer le SàE (Scénario nominal) ________________________________________#

Utilisateur met en fonctionnement le Simulateur ICSim
    log to console    ${\n}Utilisateur met en fonctionnement le Simulateur ICSim
    # Vue PC -> Démarre Simulateur ICSim
    pause execution    Utilisateur démarre le simulateur ICSim sur PC
    # Vue PC -> Affiche Simulateur ICSim
    execute manual step    Le Simulateur ICSim est affiché sur le PC ?

Utilisateur connecte la Raspberry Pi au bus CAN
    log to console    Utilisateur connecte la Raspberry Pi au bus CAN
    # Vue Raspberry Pi -> Connecte la Raspberry Pi au bus CAN
    pause execution    Utilisateur connecte la Raspberry Pi au bus CAN

Utilisateur met sous tension la Raspberry Pi
    log to console    Utilisateur met sous tension la Raspberry Pi
    # Vue Raspberry Pi -> Branche Raspberry Pi
    pause execution    Utilisateur branche la Raspberry Pi
    # Vue Raspberry Pi -> Allume la LED de tension
    execute manual step    La LED de tension est allumée sur la Raspberry Pi ?

Le programme CANgateway essaye de se lancer
    log to console    Le programme CANgateway essaye de se lancer
    # Vue PC -> Démarre le programme CANgateway
    pause execution    Utilisateur démarre le programme CANgateway en SSH sur la Raspberry Pi

La Raspberry Pi informe Utilisateur que le programme CANgateway est lancé
    log to console    La Raspberry Pi informe Utilisateur que le programme CANgateway est lancé
    # Vue PC -> Affiche le message "Lancement du programme" sur la console
    execute manual step    Le message "Lancement du programme" est visible dans la console ?

Utilisateur démarre l’application CANdroid
    log to console    Utilisateur démarre l’application CANdroid
    # Vue Smartphone -> Démarre l'application CANdroid
    Utilisateur lance l'exécution du composant CANdroid

L’application CANdroid affiche EcranPrincipal [demarrer]
    log to console    L’application CANdroid affiche EcranPrincipal avec les logos grisés
    # Vue EcranPrincipal :
    #   -> le bouton [connexion] est grisé et barré
    #   -> le bouton [lancerEnvoi] est grisé
    #   -> le bouton [play] est grisé
    L’application CANdroid affiche EcranPrincipal non connecté
    close application

L’application CANdroid se connecte au programme CANgateway
    log to console    L’application CANdroid se connecte au programme CANgateway
    # Vue Smartphone : Connecte le Smartphone au réseau TCP/IP de la Raspberry Pi
    # Vue Smartphone : Affiche que la connexion avec la Raspberry Pi est établie
    Le Smartphone se connecte au hotspot wifi de la Raspberry Pi    raspi-webgui

L’application CANdroid met à jour EcranPrincipal [demarrer]
    log to console    L’application CANdroid affiche EcranPrincipal avec les logos dégrisés
    # Vue EcranPrincipal :
    #   -> le bonton [connexion] est coloré et non barré
    #   -> le bouton [lancerEnvoi] est dégrisé
    #   -> le bouton [play] est dégrisé
    Utilisateur lance l'exécution du composant CANdroid
    #L’application CANdroid affiche EcranPrincipal connecté

    # on coupe le wifi du smartphone pour le prochain CU Reconnecter l'application CANdroid
    Le Smartphone désactive le wifi
    close application

#_______________ Démarrer le SàE (Scénario nominal) ________________________________________#

#_______________ Reconnecter l’application CANdroid (Scénario nominal) _____________________#

Utilisateur demande la reconnexion de l’application CANdroid au programme CANgateway
    log to console    ${\n}Utilisateur demande la reconnexion de l’application CANdroid au programme CANgateway
    # Vue Smartphone :
    #   -> Réactive le wifi sur Smartphone
    #   -> Utilisateur peut reconnecter l’application CANdroid au programme CANgateway
    Utilisateur lance l'exécution du composant CANdroid
    Le Smartphone se connecte au hotspot wifi de la Raspberry Pi    raspi-webgui
    Utilisateur lance l'exécution du composant CANdroid

    # Vue Écran Principal -> Clique [connexion]
    wait until element is visible       id=connexion       timeout=20
    click element       id=connexion      #TODO modifer avec une variable ${ID_BOUTON_RECONNEXION}

L’application CANdroid affiche PopupDemandeReconnexion
    log to console    L’application CANdroid affiche PopupDemandeReconnexion
    # Vue popup demande reconnexion -> Affiche PopupDemandeReconnexion
    wait until element is visible    id=popupReconnexion     #TODO modifer avec une variable ${ID_POPUP_RECONNEXION}

Utilisateur valide la reconnexion
    log to console    Utilisateur valide la reconnexion
    # Vue popup demande reconnexion -> Clique [validerReconnexion]
    wait until element is visible    id=validerReconnexion
    click element       id=validerReconnexion      #TODO modifer avec une variable ${ID_BOUTON_VALIDER_RECONNEXION}

L’application CANdroid affiche EcranPrincipal [reconnecter]
    log to console    L’application CANdroid affiche EcranPrincipal avec les logos grisés
    # Affiche Ecran Principal :
    #   -> le bouton [connexion] est grisé et barré
    #   -> le bouton [lancerEnvoi] est grisé
    #   -> le bouton [play] est grisé
    L’application CANdroid affiche EcranPrincipal non connecté

L’application CANdroid se reconnecte au programme CANgateway
    log to console    L’application CANdroid se connecte au programme CANgateway

L’application CANdroid met à jour EcranPrincipal [reconnecter]
    log to console    L’application CANdroid affiche EcranPrincipal avec les logos dégrisés
    # Affiche Ecran Principal :
    #   -> le bouton [connexion] est coloré
    #   -> le bouton [lancerEnvoi] est coloré
    #   -> le bouton [play] est coloré
    sleep   10s
    L’application CANdroid affiche EcranPrincipal connecté

#_______________ Reconnecter l’application CANdroid (Scénario nominal) _____________________#

#_______________ Envoyer des trames (Scénario nominal) _____________________________________#

Utilisateur commande un actionneur sur Tableau de Bord
    [Arguments]    ${actionneur}
    # Todo ajouter des paramètres pour gérer différents éléments
    log to console      ${\n}Utilisateur commande un actionneur sur Tableau de Bord
    # Vue Utilisateur -> Commande ${actionneur} de Tableau de Bord
    pause execution    Utilisateur actionne '${actionneur}' sur le simulateur ICSim
    execute manual step    '${actionneur}' est en action sur le simulateur ICSim

Le SàE récupère les trames du réseau CAN
    log to console      Le SàE récupère les trames du réseau CAN

L’application CANdroid met à jour EcranPrincipal [recevoir]
    [Arguments]    ${trameActionneur}
    # Vue Ecran Principal -> ${trameActionneur} est affichée sur le sniffer
    # TODO regarder si la trame du clignotant droit est visible
    log to console      L'application CANdroid met à jour EcranPrincipal avec '${trameActionneur}'
    #element should be visible    ${trameActionneur}

Utilisateur demande à interagir avec le sniffer
    log to console      Utilisateur demande à interagir avec le sniffer

#_______________ Envoyer des trames (Scénario nominal) _____________________________________#

#_______________ Interagir avec le sniffer (Scénario nominal) ______________________________#

Utilisateur demande de revenir en haut du fil
    log to console    ${\n}Utilisateur demande de revenir en haut du fil
    # je slide vers le bas du sniffer pour pouvoir revenir en haut
    #todo modifier les coordonnées en fonction de l'emplacement du sniffer (regarder le paramètre 'element')
    Swipe    500    1000    500     -3000
    # Todo


#_______________ Interagir avec le sniffer (Scénario nominal) ______________________________#

#_______________ Ajouter un objet (Scénario nominal) _______________________________________#

Utilisateur demande l’ajout d’un objet
    log to console      ${\n}Utilisateur commande un actionneur sur Tableau de Bord
    # Vue Ecran Principal -> Clique [ajouterObjet]
    wait until element is visible    id=main_imagebutton_add
    click element    id=main_imagebutton_add

Le SàE choisit un Nom d’objet par défaut
    log to console    Le SàE choisit un Nom d’objet par défaut

L’application CANdroid affiche PopupAjoutObjet
    log to console    ${\n}L’application CANdroid affiche PopupAjoutObjet avec le nom d'objet par défaut
    # Vue PopupAjoutObjet -> ${objetDefaut} est affiché dans le champ <champNomObjet>
    wait until element is visible    id=popupAjoutObjet
    element text should be    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_PAR_DEFAUT}

Utilisateur saisit un nom d’objet
    input text    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_SAISI}

Utilisateur valide l’ajout
    click element    ${ID_BOUTON_VALIDER_AJOUT_OBJET}

L’application CANdroid met à jour EcranPrincipal [objet]
    # TODO modifier l'id du nouvel objet ajouté (c'est le même dans plusieurs SV)
    element should be visible    ${ID_NOUVEL_OBJET_AJOUTE}

#_______________ Ajouter un objet (Scénario nominal) _______________________________________#

#_______________ Ajouter une trame (Scénario nominal) ______________________________________#

# 1.
Utilisateur déroule le menu de l'objet
    [Arguments]    ${objet}
    #pause execution    Utilisateur clique sur [deplierMenuObjet] de ${objet}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}Utilisateur déroule le menu de l'objet
    switch application    CANdroid
    wait until element is visible    ${ID_BOUTON_DEPLIER_MENU_OBJET}
    click element    ${ID_BOUTON_DEPLIER_MENU_OBJET}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 2.
L'application CANdroid met à jour EcranPrincipal
    #execute manual step    Pour l'objet choisi, est-ce que l'application CANdroid affiche <champTrame> et [replierMenuObjet] à la place de [deplierMenuObjet] ?
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}L'application CANdroid met à jour EcranPrincipal
    switch application    CANdroid
    sleep    3s
    Element Should Be Visible    ${ID_BOUTON_REPLIER_MENU_OBJET}
    Element Should Be Visible    ${ID_CHAMP_TEXTE_AJOUT_TRAME}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 3.
Utilisateur saisit le trame
    [Arguments]    ${trame}
    #pause execution    Utilisateur saisie ${trame} dans <champTrame>
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}Utilisateur saisit le trame
    switch application    CANdroid
    input text    ${ID_CHAMP_TEXTE_AJOUT_TRAME}    ${trame}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 4.
Utilisateur valide la saisie de la trame
    #pause execution    Utilisateur clique [ajouterTrame]
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}Utilisateur valide la saisie de la trame
    switch application    CANdroid
    wait until element is visible    ${ID_BOUTON_AJOUT_TRAME}
    click element    ${ID_BOUTON_AJOUT_TRAME}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 5.
L'application CANdroid affiche PopupModeEnvoiTrame
    [Arguments]    ${trame}    ${periodiciteDefaut}
    #execute manual step    Est-ce que l'application CANdroid affiche PopupModeEnvoiTrame avec la trame saisie [radioBoutonPOnctuel] coché et la périodicité par défaut dans <champPeriodicite> ?
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}L'application CANdroid affiche PopupModeEnvoiTrame
    switch application    CANdroid
    sleep    3s
    element should contain text    ${ID_POPUP_AJOUT_OBJET}    ${trame}
    Element Should Be Visible    ${ID_RADIO_BOUTON_PONCTUEL}
    element should contain text    ${ID_CHAMP_PERIODICITE}    ${periodiciteDefaut}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 6.
Utilisateur choisit le mode d'envoi ponctuel
    #pause execution    Utilisateur clique [radioBoutonPonctuel]
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}Utilisateur choisit le mode d'envoi ponctuel
    switch application    CANdroid
    wait until element is visible    ${ID_RADIO_BOUTON_PONCTUEL}
    click element    ${ID_RADIO_BOUTON_PONCTUEL}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 7.
Utilisateur valide le mode d'envoi
    #pause execution    Utilisateur clique [validerModeEnvoi]
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}Utilisateur valide le mode d'envoi
    switch application    CANdroid
    wait until element is visible    ${ID_BOUTON_VALIDER_MODE_ENVOI}
    click element    ${ID_BOUTON_VALIDER_MODE_ENVOI}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 8.
L'application CANdroid affiche EcranPrincipal
    #execute manual step    Est-ce que l'application CANdroid affiche EcranPrincipal avec la trame saisie dans l'objet choisi ?
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    log to console      ${\n}L'application CANdroid affiche EcranPrincipal
    switch application    CANdroid
    sleep    3s
    Element Should Be Visible    ${ID_NOUVELLE_TRAME_AJOUTEE}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

#_______________ Ajouter une trame (Scénario nominal) ______________________________________#