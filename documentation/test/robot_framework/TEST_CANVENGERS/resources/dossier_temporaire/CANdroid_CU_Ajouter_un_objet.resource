#
# Ressource RobotFramework pour le test du Chat (Felix/Camix).
#
# Construction des scénarions de test dans les termes des dossier_temporaire des spécifications du Chat.
# Les keywords des points des dossier_temporaire sont définis dans les resources propres aux technologies
# d'implémentation (java ou android) (cf. resources/).
#
# @version 0.4.3.etu.0
# @author Matthias Brun
#


*** Variables ***

# ID Bouton CANdroid
${ID_BOUTON_AJOUT_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id, ' ID BOUTON AJOUT OBJET ')]
${ID_BOUTON_VALIDER_AJOUT_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id, ' ID BOUTON VALIDER AJOUT OBJET ')]
${ID_BOUTON_ANNULER_AJOUT_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id, ' ID BOUTON ANNULER AJOUT OBJET ')]
${ID_BOUTON_FERMER_POPUP_ERREUR_AJOUT_OBJET}    locator=//android.widget.ImageButton[contains(@resource-id, ' ID BOUTON FERMER POPUP ERREUR AJOUT OBJET ')]

# ID Popup CANdroid
${ID_POPUP_AJOUT_OBJET}     locator=//android.widget.ImageButton[contains(@resource-id, ' ID DU POPUP ')]
${ID_POPUP_ERREUR_NOMBRE_OBJET}   locator=//android.widget.ImageButton[contains(@resource-id, ' ID DU POPUP ')]
${ID_POPUP_ERREUR_AJOUT_OBJET}   locator=//android.widget.ImageButton[contains(@resource-id, ' ID DU POPUP ')]

# ID Champ de texte
${ID_CHAMP_TEXTE_NOM_OBJET}     locator=android=resourceId(" ID DU CHAMP DE TEXTE ")

# Variable pour le dossier_temporaire ajouter un objet
${NOM_OBJET_PAR_DEFAUT}     nom_objet_par_faut
${NOM_OBJET_SAISI}  nom_objet_saisi
${ID_NOUVEL_OBJET_AJOUTE}   locator=android=resourceId(" ID NOUVEL OBJET ")

${IMAGE}    /home/ubuntu/Documents/ESEO/SE/ProSE/GIT_Redmine/se2024-b1.android/production/CANdroid/app/src/main/res/drawable/rasprgb.png

*** Settings ***

Resource    resources/android/CANdroid_keywords.resource

Library    AppiumLibrary
Library    WatchUI
Library    Screenshot
Library    OperatingSystem

*** Keywords ***

#_______________ dossier_temporaire Ajouter un objet (début) __________________________________#

#       À MODIFIER POUR FAIRE COMME LE dossier_temporaire DÉMARRER LE SAE

Ajouter un objet
    # 1. on lance CANdroid
    L'utilisateur lance l'exécution du composant CANdroid
    # 2. on clique sur le bouton pour ajouter un objet
    Click Element    ${ID_BOUTON_AJOUT_OBJET}
    # 3. on attend de voir si le popup d'ajout d'objet s'affiche
    # TODO mettre le [timeout] dans une variable ?
    Wait Until Page Contains Element    timeout=5s    ${ID_POPUP_AJOUT_OBJET}
    # 4. on vérifie que le champ de texte est rempli par le nom d'objet par defaut
    Element Text Should Be    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_PAR_DEFAUT}
    # 5. on saisit un nom pour le futur objet
    Input Text    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_SAISI}
    # 6. on clique sur le bouton pour valider l'ajout de l'ajoute de l'objet
    Click Element    ${ID_BOUTON_VALIDER_AJOUT_OBJET}
    # 7. on vérifie que l'écran principal est mis à jour
    # TODO modifier l'id du nouvel objet ajouté (c'est le même dans plusieurs SV)
    Element Should Be Visible    ${ID_NOUVEL_OBJET_AJOUTE}

Ajouter un objet [Utilisateur ne souhaite pas saisir de nom]
    # 1. on lance CANdroid
    L'utilisateur lance l'exécution du composant CANdroid
    # 2. on clique sur le bouton pour ajouter un objet
    Click Element    ${ID_BOUTON_AJOUT_OBJET}
    # 3. on attend de voir si le popup s'affiche
    Wait Until Page Contains Element    timeout=5s    ${ID_POPUP_AJOUT_OBJET}
    # 4. on vérifie que le champ de texte est rempli par le nom d'objet par defaut
    Element Text Should Be    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_PAR_DEFAUT}
    # 5. on clique sur le bouton pour valider l'ajout de l'ajoute de l'objet
    Click Element    ${ID_BOUTON_VALIDER_AJOUT_OBJET}
    # 6. on vérifie que l'écran principal est mis à jour
    # TODO modifier l'id du nouvel objet ajouté (c'est le même dans plusieurs SV)
    Element Should Be Visible    ${ID_NOUVEL_OBJET_AJOUTE}

Ajouter un objet [Utilisateur souhaite annuler]
    # 1. on lance CANdroid
    L'utilisateur lance l'exécution du composant CANdroid
    # 2. on clique sur le bouton pour ajouter un objet
    Click Element    ${ID_BOUTON_AJOUT_OBJET}
    # 3. on attend de voir si le popup s'affiche
    Wait Until Page Contains Element    timeout=5s    ${ID_POPUP_AJOUT_OBJET}
    # 4. on vérifie que le champ de texte est rempli par le nom d'objet par defaut
    Element Text Should Be    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_PAR_DEFAUT}
    # 5. on clique sur le bouton pour valider l'ajout de l'ajoute de l'objet
    Click Element    ${ID_BOUTON_ANNULER_AJOUT_OBJET}
    # 6. on vérifie que l'écran principal n'a pas été modifier
    # TODO comment faire pour vérifier que l'écran principal n'a pas été modifié ?

Ajouter un objet [Nombre maximal d'objet atteint & Utilisateur souhaite ajouter un objet]
    # TODO comment faire en sorte que le nombre max est atteint ?
    # TODO modifier la variable globale qui définit le nombre max ?
    # TODO Ajout des objets pour atteindre le nombre max d'objet ?
    # 1. on lance CANdroid
    L'utilisateur lance l'exécution du composant CANdroid
    # 2. on clique sur le bouton pour ajouter un objet
    Click Element    ${ID_BOUTON_AJOUT_OBJET}
    # 3. on attend de voir si le popup d'erreur d'ajout d'objet s'affiche
    Wait Until Page Contains Element    timeout=5s    ${ID_POPUP_ERREUR_NOMBRE_OBJET}
    # 4. on clique sur le bouton pour fermer le popup d'erreur
    Click Element    ${ID_BOUTON_FERMER_POPUP_ERREUR_AJOUT_OBJET}

Ajouter un objet [Utilisateur souhaite saisir un nom qui existe déjà]
    # TODO un objet avec le nom "objet_existant" existe déjà sur l'application
    # 1. on lance CANdroid
    L'utilisateur lance l'exécution du composant CANdroid
    # 2. on clique sur le bouton pour ajouter un objet
    Click Element    ${ID_BOUTON_AJOUT_OBJET}
    # 3. on attend de voir si le popup d'ajout d'objet s'affiche
    Wait Until Page Contains Element    timeout=5s    ${ID_POPUP_AJOUT_OBJET}
    # 4. on vérifie que le champ de texte est rempli par le nom d'objet par defaut
    Element Text Should Be    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_PAR_DEFAUT}
    # 5. on saisit un nom d'objet qui existe déjà
    # TODO modifier ${NOM_OBJET_SAISI} par le nom d'objet déjà créé
    Input Text    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_SAISI}
    # 6. on clique sur le bouton pour valider l'ajout de l'objet
    Click Element    ${ID_BOUTON_VALIDER_AJOUT_OBJET}
    # 7. on attend de voir si le popup d'erreur d'ajout d'objet s'affiche
    Wait Until Page Contains Element    timeout=5s    ${ID_POPUP_ERREUR_AJOUT_OBJET}
    # 8. on saisit un nom pour le futur objet
    # TODO modifier ${NOM_OBJET_SAISI} par un nom d'objet qui n'existe pas
    Input Text    ${ID_CHAMP_TEXTE_NOM_OBJET}    ${NOM_OBJET_SAISI}
    # 9. on clique sur le bouton pour valider l'ajout de l'objet
    Click Element    ${ID_BOUTON_VALIDER_AJOUT_OBJET}
    # 10. on vérifie que l'écran principal est mis à jour
    # TODO modifier l'id du nouvel objet ajouté (c'est le même dans plusieurs SV)
    Element Should Be Visible    ${ID_NOUVEL_OBJET_AJOUTE}

#_______________ dossier_temporaire Ajouter un objet (fin) __________________________________#
