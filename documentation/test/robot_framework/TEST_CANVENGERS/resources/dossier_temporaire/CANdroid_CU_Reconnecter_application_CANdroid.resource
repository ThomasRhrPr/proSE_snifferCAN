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

Resource    /keywords/CANdroid_keywords_Reconnecter_application_CANdroid.resource

Library    AppiumLibrary
Library    WatchUI
Library    Screenshot
Library    OperatingSystem

*** Keywords ***

#_______________ dossier_temporaire Reconnecter l’application CANdroid (début) ________________________________#

Reconnecter l’application CANdroid
    # 1.
    Utilisateur demande la reconnexion de l’application CANdroid au programme CANgateway
    # 2.
    L’application CANdroid aﬀiche PopupDemandeReconnexion
    # 3.
    Utilisateur valide la reconnexion
    # 4.
    L’application CANdroid affiche EcranPrincipal
    # 5.
    L’application CANdroid se reconnecte au programme CANgateway
    # 6.
    L’application CANdroid met à jour EcranPrincipal

#_______________ dossier_temporaire Reconnecter l’application CANdroid (fin) ________________________________#
