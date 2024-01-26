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

Resource    CANdroid_keywords.resource

*** Keywords ***

#_______________ dossier_temporaire Démarrer le SàE (début) ___________________________________#
#_______________ Scénario Nominal (début) _____________________________________#
Démarrer le SàE
    # 1.
    Utilisateur met en fonctionnement le Simulateur ICSim
    # 2.
    Utilisateur connecte la Raspberry Pi au bus CAN
    # 3.
    Utilisateur met sous tension la Raspberry Pi
    # 4.
    Le programme CANgateway essaye de se lancer
    # 5.
    La Raspberry Pi informe Utilisateur que le programme CANgateway est lancé
    # 6.
    Utilisateur démarre l’application CANdroid
    # 7.
    L’application CANdroid affiche EcranPrincipal [demarrer]
    # 8.
    L’application CANdroid se connecte au programme CANgateway
    # 9.
    L’application CANdroid met à jour EcranPrincipal [demarrer]

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Démarrer le SàE (fin) _____________________________________#

#_______________ dossier_temporaire Reconnecter l’application CANdroid (début) ________________#
#_______________ Scénario Nominal (début) _____________________________________#

Reconnecter l’application CANdroid
    # 1.
    Utilisateur demande la reconnexion de l’application CANdroid au programme CANgateway
    # 2.
    L’application CANdroid affiche PopupDemandeReconnexion
    # 3.
    Utilisateur valide la reconnexion
    # 4.
    L’application CANdroid affiche EcranPrincipal [reconnecter]
    # 5.
    L’application CANdroid se reconnecte au programme CANgateway
    # 6.
    L’application CANdroid met à jour EcranPrincipal [reconnecter]

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Reconnecter l’application CANdroid (fin) __________________#

#_______________ dossier_temporaire Recevoir des trames (début) _______________________________#
#_______________ Scénario Nominal (début) _____________________________________#

Recevoir des trames
    [Arguments]         ${actionneur}          ${trameActionneur}
    # 1.
    Utilisateur commande un actionneur sur Tableau de Bord      ${actionneur}
    # 2.
    Le SàE récupère les trames du réseau CAN
    # 3.
    L’application CANdroid met à jour EcranPrincipal [recevoir]     ${trameActionneur}
    # 4.
    Utilisateur demande à interagir avec le sniffer

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Recevoir des trames (fin) _________________________________#

#_______________ dossier_temporaire Interagir avec le sniffer (début) _________________________#
#_______________ Scénario Nominal (début) _____________________________________#

Interagir avec le sniffer
    # 1.
    Utilisateur demande de revenir en haut du fil
    # 2.
    L’application CANdroid va en haut du fil
    # 3.
    Utilisateur demande d’effacer les trames
    # 4.
    L’application CANdroid efface les trames aﬀichées
    # 5.
    Utilisateur demande l’arrêt de la réception des trames
    # 6.
    L’application CANdroid stoppe l’aﬀichage de nouvelles trames
    # 7.
    Utilisateur demande d’enregistrer les trames reçues sur le Smartphone
    # 8.
    L’application CANdroid enregistre les trames dans un fichier de log (voir section 3.2)
    # 9.
    Utilisateur demande la reprise de la réception des trames
    # 10.
    L’application CANdroid reprend l’aﬀichage de nouvelles trames

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Interagir avec le sniffer (fin) ___________________________#

#_______________ dossier_temporaire Ajouter un objet (début) __________________________________#
#_______________ Scénario Nominal (début) _____________________________________#

Ajouter un objet
    # 1.
    Utilisateur demande l’ajout d’un objet
    # 2.
    Le SàE choisit un Nom d’objet par défaut
    # 3.
    L’application CANdroid affiche PopupAjoutObjet
    # 4.
    Utilisateur saisit un nom d’objet
    # 5.
    Utilisateur valide l’ajout
    # 6.
    L’application CANdroid met à jour EcranPrincipal [objet]

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Ajouter un objet (fin) ____________________________________#

#_______________ dossier_temporaire Ajouter une trame (début) _________________________________#
#_______________ Scénario Nominal (début) _____________________________________#

# Précondition :
#   L'application CANdroid est démarrée.
#   Un ${objet} est créé.
#   Le nombre maximal de trames n'a pas été atteint dans ${objet}.
#   Aucune trame n'est créée dans ${objet}.
Ajouter une trame
    [Arguments]     ${objet}    ${trame}    ${periodiciteDefaut}
    # 1.
    Utilisateur déroule le menu de l'objet    ${objet}
    # 2.
    L'application CANdroid met à jour EcranPrincipal    ${objet}
    # 3.
    Utilisateur saisit le trame    ${trame}
    # 4.
    Utilisateur valide la saisie de la trame
    # 5.
    L'application CANdroid affiche PopupModeEnvoiTrame    ${trame}    ${periodiciteDefaut}
    # 6.
    Utilisateur choisit le mode d'envoi ponctuel
    # 7.
    Utilisateur valide le mode d'envoi
    # 8.
    L'application CANdroid affiche EcranPrincipal    ${objet}    ${trame}

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Ajouter une trame (fin) ___________________________________#

#_______________ dossier_temporaire Envoyer des trames (début) ________________________________#
#_______________ Scénario Nominal (début) _____________________________________#

Envoyer des trames
    # 1.
    Utilisateur déroule le menu d’un objet
    # 2.
    L’application CANdroid met à jour EcranPrincipal
    # 3.
    Utilisateur sélectionne une trame
    # 4.
    L’application CANdroid met à jour EcranPrincipal
    # 5.
    Utilisateur demande à envoyer la trame
    # 6.
    Le SàE diffuse la trame sur le bus CAN
    # 7.
    L’application CANdroid aﬀiche EcranPrincipal

#_______________ Scénario Nominal (fin) _____________________________________#
#_______________ dossier_temporaire Envoyer des trames (fin) __________________________________#

#_______________ dossier_temporaire Arrêter l’envoi des trames (début) ________________________#
#_______________ Scénario Nominal (début) _____________________________________#

Arrêter l’envoi des trames
    # 1.
    Utilisateur demande à stopper l’envoi des trames
    # 2.
    L’application CANdroid aﬀiche PopupArretEnvoi
    # 3.
    Utilisateur confirme l’arrêt de l’envoi des trames
    # 4.
    Le SàE arrête de diffuser les trames sur le bus CAN
    # 5.
    L’application CANdroid aﬀiche EcranPrincipal

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Arrêter l’envoi des trames (fin) __________________________#

#_______________ dossier_temporaire Supprimer un élément (début) ______________________________#
#_______________ Scénario Nominal (début) _____________________________________#

Supprimer un élément
    # 1.
    Utilisateur sélectionne un objet
    # 2.
    L’application CANdroid met à jour EcranPrincipal
    # 3.
    Utilisateur demande la suppression de la sélection
    # 4.
    L’application CANdroid aﬀiche PopupSuppressionElement
    # 5.
    Utilisateur valide la suppression de la sélection
    # 6.
    L’application CANdroid met à jour EcranPrincipal

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Supprimer un élément (fin) ________________________________#

#_______________ dossier_temporaire Stopper le SàE (début) ____________________________________#
#_______________ Scénario Nominal (début) _____________________________________#

Stopper le SàE
    # 1.
    Utilisateur quitte l’application CANdroid
    # 2.
    Utilisateur met hors tension la Raspberry Pi
    # 3.
    Utilisateur éteint le Simulateur ICSim

#_______________ Scénario Nominal (fin) _______________________________________#
#_______________ dossier_temporaire Stopper le SàE (fin) ______________________________________#
