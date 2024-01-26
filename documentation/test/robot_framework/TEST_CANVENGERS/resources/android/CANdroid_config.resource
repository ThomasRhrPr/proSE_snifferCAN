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

Library    AppiumLibrary
Library    Process
Library    OperatingSystem

*** Variables ***

${APPIUM}                /home/ubuntu/Appium/node_modules/.bin/appium     # Localisation de appium
${ANDROID_HOME}          /home/ubuntu/Android/Sdk/                        # Localisation du SDK Android

# Configuration des variables pour l'application CANdroid
${CANDROID_PACKAGE}      fr.eseo.b12024.candroid                          # Nom du package de l'application CANdroid
${CANDROID_ACTIVITY}     .MainActivity                                    # Nom de l'activité principale de l'application CANdroid
${CANDROID_APK}          ${EXECDIR}/SUT/CANdroid.apk                      # Chemin vers le fichier APK de l'application CANdroid

# Configuration des variables pour l'application Paramètre
${SETTINGS_PACKAGE}      com.android.settings
${SETTINGS_ACTVITY}      .Settings

*** Keywords ***

# Suite Setup    :  Exécution SUT (sur cible)
# Suite Teardown :  Arrêt SUT (sur cible)

Exécution SUT (sur cible)
    Exécution Appium
    log to console    Exécution Appium ok
    Déployer CANdroid (sur cible)
    log to console    Exécution CANdroid ok

Arrêt SUT (sur cible)
    Fermer CANdroid (sur cible)
    log to console  Arrêt CANdroid ok
    Arrêt Appium
    log to console  Arrêt Appium ok

Exécution Appium
    start process       ${APPIUM}
    ...                 -p    4723    -a    127.0.0.1    -pa    /wd/hub    --allow-insecure\=adb_shell
    ...                 env:ANDROID_HOME=${ANDROID_HOME}
    ...                 alias=Appium
    sleep               3s

Arrêt Appium
    terminate process    Appium

Déployer CANdroid (sur cible)
    open application    http://127.0.0.1:4723/wd/hub    allowTestPackages=true    automationName=UIAutomator2    platformName=Android
    ...                 app=${CANDROID_APK}    appPackage=${CANDROID_PACKAGE}    appActivity=${CANDROID_ACTIVITY}
    ...                 alias=CANdroid

Retour sur CANdroid (sur cible) (workaround)
    open application    http://127.0.0.1:4723/wd/hub    allowTestPackages=true    automationName=UIAutomator2    platformName=Android
    ...                 appPackage=${CANDROID_PACKAGE}    appActivity=${CANDROID_ACTIVITY}
    ...                 alias=CANdroid

Fermer CANdroid (sur cible)
    switch application    CANdroid
    close application

#_______________ AJOUTER DANS UN AUTRE FICHIER ______________________#
# Todo ajouter toutes les fonctions ci dessous dans un fichier "tools"
# cela va permettre d'avoir dans le fichier CANdroid_keywords_TEST.ressource uniquement les fonctions des dossier_temporaire

Utilisateur lance l'exécution du composant CANdroid
    switch application      CANdroid
    activate application    ${CANDROID_PACKAGE}

Arrêt CANdroid
    switch application    CANdroid
    stop application      ${CANDROID_PACKAGE}
    log to console        Arrêt CANdroid ok

L’application CANdroid affiche EcranPrincipal non connecté
    Vérifier l'état de l'icône grâce à l'attribut contentDescription       connexion       Non Connecté
    Vérifier l'état de l'icône grâce à l'attribut contentDescription       envoi           Mode Envoi impossible
    Vérifier l'état de l'icône grâce à l'attribut contentDescription       play            Application non connecté

L’application CANdroid affiche EcranPrincipal connecté
    Vérifier l'état de l'icône grâce à l'attribut contentDescription       connexion       Connecté
    Vérifier l'état de l'icône grâce à l'attribut contentDescription       envoi           Mode Envoi Inactif
    Vérifier l'état de l'icône grâce à l'attribut contentDescription       play            Pause

Vérifier l'état de l'icône grâce à l'attribut contentDescription
    [Arguments]         ${id}           ${resultat_attendu}

    wait until element is visible    id=${id}       timeout=10
    ${content_description}    Get Element Attribute    id=${id}    contentDescription
    Run Keyword If    '${content_description}' != '${resultat_attendu}'    FAIL    Expected: '${resultat_attendu}', Actual: '${content_description}'

Le Smartphone se connecte au hotspot wifi de la Raspberry Pi
    [Arguments]    ${hotspot_wifi}

    # Ouvrir application paramètre
    start activity      ${SETTINGS_PACKAGE}     ${SETTINGS_ACTVITY}

    # Accéder aux paramètres Wi-Fi
    wait until element is visible    android=new UiSelector().text("Connexions")    timeout=10
    click element                    android=new UiScrollable(new UiSelector().scrollable(true).instance(0)).scrollIntoView(new UiSelector().textContains("Connexions"))

    # Activer le Wi-Fi
    wait until element is visible    android=new UiSelector().text("Wi-Fi")    timeout=10
    click element                    android=new UiScrollable(new UiSelector().scrollable(true).instance(0)).scrollIntoView(new UiSelector().textContains("Wi-Fi"))

    # Vérifier si l'élément 'Activé' est visible
    ${ELEMENT_VISIBLE}  run keyword and return status    element should be visible    android=new UiSelector().text("Activé")

    # Cliquer sur l'élément 'Désactivé' si 'Activé' n'est pas visible pour activé le wifi sur le téléphone
    run keyword if    '${ELEMENT_VISIBLE}' == 'False'    click element    android=new UiSelector().textContains("Désactivé")

    # Sélectionner le réseau Wi-Fi spécifique
    wait until element is visible    android=new UiSelector().text("${hotspot_wifi}")
    click element                    android=new UiSelector().text("${hotspot_wifi}")

    # Vérifier si la connexion Wi-Fi est établie
    wait until element is visible    android=new UiSelector().text("Vérification de la qualité de votre connexion Internet…")    timeout=10
    wait until element is visible    android=new UiSelector().text("Intern. peut ne pas être disp.")    timeout=15

Le Smartphone désactive le wifi

    # Ouvrir application paramètre
    start activity      ${SETTINGS_PACKAGE}     ${SETTINGS_ACTVITY}

    # Accéder aux paramètres Wi-Fi
    wait until element is visible    android=new UiSelector().text("Connexions")    timeout=10
    Click Element    android=new UiScrollable(new UiSelector().scrollable(true).instance(0)).scrollIntoView(new UiSelector().textContains("Connexions"))

    # Activer le Wi-Fi
    wait until element is visible    android=new UiSelector().text("Wi-Fi")    timeout=10
    Click Element    android=new UiScrollable(new UiSelector().scrollable(true).instance(0)).scrollIntoView(new UiSelector().textContains("Wi-Fi"))

    # Cliquer sur l'élément 'Activé' pour désactiver le wifi
    wait until element is visible    android=new UiSelector().text("Activé")    timeout=10
    Click Element    android=new UiSelector().textContains("Activé")

    close application