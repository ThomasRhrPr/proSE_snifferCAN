#
# Ressource RobotFramework pour le test de Ajouter une trame.
#
# Version pour l'implémentation ProSE B1 2024
#
# Fournit l'implémentation des keywords des scénarios Ajouter une trame de la spécification.
#
# @version 0.0.1
# @author Paul Trémoureux
#

*** Settings ***

#Resource    resources/keywords/CANdroid_keywords_temp.resource

Library    AppiumLibrary
Library    WatchUI
Library    OperatingSystem
Library    Dialogs

*** Variables ***

${SCREENSHOT_NAME}   screenshotAjouterTrame.png
${IMAGE_FOLDER}  image

*** Keywords ***

#_______________ Ajouter une trame (Scénario nominal) ______________________________________#

# 1.
Utilisateur déroule le menu de l'objet
    [Arguments]    ${objet}
    #pause execution    Utilisateur clique sur [deplierMenuObjet] de ${objet}
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    switch application    CANdroid
    click element    id=deplierMenuObjet
    # TODO ajouter le tag du bouton
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 2.
L'application CANdroid met à jour EcranPrincipal
    #execute manual step    Pour l'objet choisi, est-ce que l'application CANdroid affiche <champTrame> et [replierMenuObjet] à la place de [deplierMenuObjet] ?
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    # On récupère une capture d'écran de l'application CANdroid
    Capture Page Screenshot    ${SCREENSHOT_NAME}
    # On récupère uniquement la partie haute de l'écran (la partie sniffer n'ayant pas de lien avec notre test)
    Crop Image    report/${SCREENSHOT_NAME}     69       112      166     208     ${IMAGE_FOLDER}
    # On récupère dans une liste la capture d'écran de l'icon que l'on veut
    ${files}       OperatingSystem.List Files In Directory    ${IMAGE_FOLDER}
    # On compare l'image récupérée avec le template
    ${result}    Compare Images    ${IMAGE_FOLDER}/${files}[0]    image/menuObjetDeplie.png
    # On fait du nettoyage
    Remove File    report/${SCREENSHOT_NAME}
    Remove Directory    ${IMAGE_FOLDER}      recursive=True
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 3.
Utilisateur saisit le trame
    [Arguments]    ${trame}
    pause execution    Utilisateur saisie ${trame} dans <champTrame>
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    #input text    id=champTrame    ${trame}
    # TODO ajouter le tag du champ
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 4.
Utilisateur valide la saisie de la trame
    pause execution    Utilisateur clique [ajouterTrame]
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    #click button    id=ajouterTrame
    # TODO ajouter le tag du bouton
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 5.
L'application CANdroid affiche PopupModeEnvoiTrame
    execute manual step    Est-ce que l'application CANdroid affiche PopupModeEnvoiTrame avec la trame saisie [radioBoutonPOnctuel] coché et la périodicité par défaut dans <champPeriodicite> ?
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    #log    TODO (L'application CANdroid affiche PopupModeEnvoiTrame)
    # On récupère une capture d'écran de l'application CANdroid
    #Capture Page Screenshot    ${SCREENSHOT_NAME}
    # On récupère uniquement la partie haute de l'écran (la partie sniffer n'ayant pas de lien avec notre test)
    # Crop Image    report/${SCREENSHOT_NAME}    INSERER COORDONNEES    ${TEMP_FOLDER}
    # On récupère dans une liste la capture d'écran de l'icon que l'on veut
    #${files}       OperatingSystem.List Files In Directory    ${IMAGE_FOLDER}
    # On compare l'image récupérée avec le template
    #${result}    Compare Images    ${IMAGE_FOLDER}/${files}[0]    image/popupModeEnvoiTrame.png
    # On fait du nettoyage
    #Remove File    report/${SCREENSHOT_NAME}
    #Remove Directory    ${IMAGE_FOLDER}      recursive=True
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 6.
Utilisateur choisit le mode d'envoi ponctuel
    pause execution    Utilisateur clique [radioBoutonPonctuel]
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    #click button    id=radioBoutonPonctuel
    # TODO ajouter le tag du bouton
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 7.
Utilisateur valide le mode d'envoi
    pause execution    Utilisateur clique [validerModeEnvoi]
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    #click button    id=validerModeEnvoi
    # TODO ajouter le tag du bouton
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

# 8.
L'application CANdroid affiche EcranPrincipal
    execute manual step    Est-ce que l'application CANdroid affiche EcranPrincipal avec la trame saisie dans l'objet choisi ?
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#
    # On récupère une capture d'écran de l'application CANdroid
    #Capture Page Screenshot    ${SCREENSHOT_NAME}
    # On récupère uniquement la partie haute de l'écran (la partie sniffer n'ayant pas de lien avec notre test)
    # Crop Image    report/${SCREENSHOT_NAME}    INSERER COORDONNEES    ${TEMP_FOLDER}
    # On récupère dans une liste la capture d'écran de l'icon que l'on veut
    #${files}       OperatingSystem.List Files In Directory    ${IMAGE_FOLDER}
    # On compare l'image récupérée avec le template
    #${result}    Compare Images    ${IMAGE_FOLDER}/${files}[0]    image/ecranPrincipalFinal.png
    # On fait du nettoyage
    #Remove File    report/${SCREENSHOT_NAME}
    #Remove Directory    ${IMAGE_FOLDER}      recursive=True
    #___________ PARTIE A COMPLETER POUR AUTOMATISATION KEYWORD ____________________________#

#_______________ Ajouter une trame (Scénario nominal) ______________________________________#


# Variantes

# 5.a.1.
#L'application CANdroid affiche PopupErreurNombreTrame (5.a)
#    log    TODO (L'application CANdroid affiche PopupErreurNombreTrame (5.a))
# 5.a.2.
#Utilisateur ferme PopupErreurNombreTrame (5.a)
#    click button    id=fermerErreurNombreTrame
# 5.a.3.
#L'application CANdroid affiche EcranPrincipal (5.a)
#    log    TODO (L'application CANdroid affiche EcranPrincipal (5.a))

# 5.b.1.
#L'application CANdroid affiche PopupErreurSaisieTrame (5.b)
#    log    TODO (L'application CANdroid affiche PopupErreurSaisieTrame (5.b))
# 5.b.2.
#Utilisateur ferme PopupErreurSaisieTrame (5.b)
#    click button    id=fermerErreurTrame
# 5.b.3.
#L'application CANdroid affiche EcranPrincipal (5.b)
#    log    TODO (L'application CANdroid affiche EcranPrincipal (5.b))

# 6.a.1.
#Utilisateur choisit le mode d'envoi cyclique (6.a)
#    click button    id=radioBoutonCyclique
# 6.a.2.
#Utilisateur saisit la périodicité (6.a)
#    [Arguments]    ${periodicite}
#    input text    id=champPeriodicite    ${periodicite}

# 6.b.1.
#Utilisateur choisit le mode d'envoi cyclique (6.b)
#    click button    id=radioBoutonCyclique

# Extensions

# 6-7.a.1.
#Utilisateur ferme PopupModeEnvoiTrame (6-7.a)
#    click button    id=annulerModeEnvoi
# 6-7.a.2.
#L'application CANdroid affiche EcranPrincipal (6-7.a)
#    log    TODO (L'application CANdroid affiche EcranPrincipal (6-7.a))