#
# Ressource RobotFramework du dossier_temporaire Ajouter une trame du projet ProSE.
#
# Construction des scénarions de test dans les termes des dossier_temporaire des spécifications.
# Les keywords des points des dossier_temporaire sont définis dans les ressources propres aux technologies
# d'implémentation (java ou android) (cf. resources/).
#
# @version 0.0.3
# @author Paul Trémoureux
#


*** Variables ***

*** Settings ***

Resource    resources/keywords/CANdroid_keywords_Ajouter_une_trame.resource

Library    AppiumLibrary
Library    WatchUI
Library    Screenshot
Library    OperatingSystem

*** Keywords ***

#_______________ dossier_temporaire Ajouter une trame (début) ________________________________#


# Scénario nominal

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

# Variantes

# Précondition :
#   L'application CANdroid est démarrée.
#   Un ${objet} est créé.
#   Le nombre maximal de trames n'a pas été atteint dans ${objet}.
#   Aucune trame n'est créée dans ${objet}.
#Ajouter une trame 1 [L'objet es déjà déplié]
#    [Arguments]    ${trame}
    # 3.
#    Utilisateur saisit le trame    ${trame}
    # 4.
#    Utilisateur valide la saisie de la trame
    # 5.
#    L'application CANdroid affiche PopupModeEnvoiTrame
    # 6.
#    Utilisateur choisit le mode d'envoi ponctuel
    # 7.
#    Utilisateur valide le mode d'envoi
    # 8.
#    L'application CANdroid affiche EcranPrincipal


# Précondition :
#   L'application CANdroid est démarrée.
#   Un ${objet} est créé.
#   Le nombre maximal de trames a été atteint dans ${objet}.
#   Des ${trames} sont créées dans ${objet}.
#Ajouter une trame 5 [Nombre maximal de trames atteint & Utilisateur souhaite ajouter une trame]
#    [Arguments]     ${objet}    ${trame}
    # 1.
#    Utilisateur déroule le menu de l'objet    ${objet}
    # 2.
#    L'application CANdroid met à jour EcranPrincipal
    # 3.
#    Utilisateur saisit le trame    ${trame}
    # 4.
#    Utilisateur valide la saisie de la trame
    # 5.a.1.
#    L'application CANdroid affiche PopupErreurNombreTrame (5.a)
    # 5.a.2.
#    Utilisateur ferme PopupErreurNombreTrame (5.a)
    # 5.a.3.
#    L'application CANdroid affiche EcranPrincipal (5.a)


# Précondition :
#   L'application CANdroid est démarrée.
#   Un ${objet} est créé.
#   Le nombre maximal de trames n'a pas été atteint dans ${objet}.
#   Aucune trame n'est créée dans ${objet}.
#Ajouter une trame 5 [Utilisateur souhaite ajouter une trame qui n'a pas le bon format]
#    [Arguments]     ${objet}    ${trameFausse}    ${trame}
    # 1.
#    Utilisateur déroule le menu de l'objet    ${objet}
    # 2.
#    L'application CANdroid met à jour EcranPrincipal
    # 3.
#    Utilisateur saisit le trame    ${trameFausse}
    # 4.
#    Utilisateur valide la saisie de la trame
    # 5.b.1.
#    L'application CANdroid affiche PopupErreurSaisieTrame (5.b)
    # 5.b.2.
#    Utilisateur ferme PopupErreurSaisieTrame (5.b)
    # 5.b.3.
#    L'application CANdroid affiche EcranPrincipal (5.b)
    # 3.
#    Utilisateur saisit le trame    ${trame}
    # 4.
#    Utilisateur valide la saisie de la trame
    # 5.
#    L'application CANdroid affiche PopupModeEnvoiTrame
    # 6.
#    Utilisateur choisit le mode d'envoi ponctuel
    # 7.
#    Utilisateur valide le mode d'envoi
    # 8.
#    L'application CANdroid affiche EcranPrincipal

# Précondition :
#   L'application CANdroid est démarrée.
#   Un ${objet} est créé.
#   Le nombre maximal de trames n'a pas été atteint dans ${objet}.
#   Aucune trame n'est créée dans ${objet}.
#Ajouter une trame 6 [Utilisateur choisit le mode cyclique & souhaite changer la périodicité]
#    [Arguments]     ${objet}    ${trame}    ${periodicite}
    # 1.
#    Utilisateur déroule le menu de l'objet    ${objet}
    # 2.
#    L'application CANdroid met à jour EcranPrincipal
    # 3.
#    Utilisateur saisit le trame    ${trame}
    # 4.
#    Utilisateur valide la saisie de la trame
    # 5.
#    L'application CANdroid affiche PopupModeEnvoiTrame
    # 6.a.1.
#    Utilisateur choisit le mode d'envoi cyclique (6.a)
    # 6.a.2.
#    Utilisateur saisit la périodicité (6.a)    ${periodicite}
    # 7.
#    Utilisateur valide le mode d'envoi
    # 8.
#    L'application CANdroid affiche EcranPrincipal

# Précondition :
#   L'application CANdroid est démarrée.
#   Un ${objet} est créé.
#   Le nombre maximal de trames n'a pas été atteint dans ${objet}.
#   Aucune trame n'est créée dans ${objet}.
#Ajouter une trame 6 [Utilisateur choisit le mode cyclique & souhaite garder la périodicité par défaut]
#    [Arguments]     ${objet}    ${trame}
    # 1.
#    Utilisateur déroule le menu de l'objet    ${objet}
    # 2.
#    L'application CANdroid met à jour EcranPrincipal
    # 3.
#    Utilisateur saisit le trame    ${trame}
    # 4.
#    Utilisateur valide la saisie de la trame
    # 5.
#    L'application CANdroid affiche PopupModeEnvoiTrame
    # 6.b.1.
#    Utilisateur choisit le mode d'envoi cyclique (6.b)
    # 7.
#    Utilisateur valide le mode d'envoi
    # 8.
#    L'application CANdroid affiche EcranPrincipal

# Extension

# Précondition :
#   L'application CANdroid est démarrée.
#   Un ${objet} est créé.
#   Le nombre maximal de trames n'a pas été atteint dans ${objet}.
#   Aucune trame n'est créée dans ${objet}.
#Ajouter une trame 6-7 [Utilisateur souhaite annuler]
#    [Arguments]     ${objet}    ${trame}
    # 1.
#    Utilisateur déroule le menu de l'objet    ${objet}
    # 2.
#    L'application CANdroid met à jour EcranPrincipal
    # 3.
#    Utilisateur saisit le trame    ${trame}
    # 4.
#    Utilisateur valide la saisie de la trame
    # 5.
#    L'application CANdroid affiche PopupModeEnvoiTrame
    # 6-7.a.1.
#    Utilisateur ferme PopupModeEnvoiTrame (6-7.a)
    # 6-7.a.2.
#    L'application CANdroid affiche EcranPrincipal (6-7.a)


#_______________ dossier_temporaire Ajouter une trame (fin) ________________________________#
