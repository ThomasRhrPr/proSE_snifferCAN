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

Library    Process
Library    AppiumLibrary
Library    WatchUI
Library    Screenshot
Library    OperatingSystem
Library    Dialogs

*** Variables ***

${CANDROID_PACKAGE}        fr.eseo.b12024.candroid
${CANDROID_ACTIVITY}       fr.eseo.b12024.candroid.MainActivity
${CANDROID_APK}           ${EXECDIR}/SUT/CANdroid.apk

# démarrer le SàE
${SCREENSHOT_NAME}   screenshot.png
${TEMP_FOLDER}  temp_folder

*** Keywords ***

L'utilisateur lance l'exécution du composant CANdroid
    switch application    CANdroid
    activate application    ${CANDROID_PACKAGE}

Arrêt CANdroid
    switch application    CANdroid
    stop application    ${CANDROID_PACKAGE}
    log to console    Arrêt CANdroid ok