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

*** Settings ***

Resource    resources/keywords/CANdroid_keywords_Demarrer_le_SaE.resource
#Resource    resources/keywords/CANdroid_keywords_temp.resource

Library    AppiumLibrary
Library    WatchUI
Library    Screenshot
Library    OperatingSystem

*** Keywords ***

#_______________ dossier_temporaire Démarrer le SàE (début) ___________________________________#

Démarrer le SàE
    [Arguments]     ${reseauWifi}    ${motDePasse}
    # 1.
    Utilisateur met en fonctionnement le Simulateur ICSim
    # 2.
    Utilisateur connecte la Raspberry Pi au bus CAN
    # 3.
    Utilisateur met sous tension la Raspberry Pi
    # 4.
    Utilisateur lance le programme CANgateway via la connexion SSH
    # 5.
    E_PC informe Utilisateur que le programme CANgateway est lancé
    # 6.
    Utilisateur connecte E_Smartphone au réseau TCP/IP de E_Raspberry   ${reseauWifi}   ${motDePasse}
    # 7.
    E_Smartphone informe Utilisateur qu'il est connecté au réseau TCP/IP de E_Raspberry     ${reseauWifi}
    # 8.
    Utilisateur démarre l’application CANdroid
    # 9.
    L’application CANdroid affiche EcranPrincipal
    # 10.
    L’application CANdroid se connecte au programme CANgateway
    # 11.
    L’application CANdroid met à jour EcranPrincipal

#_______________ dossier_temporaire Démarrer le SàE (fin) _____________________________________#