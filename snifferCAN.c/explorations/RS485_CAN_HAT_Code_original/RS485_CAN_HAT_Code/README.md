# Tips et commandes pour exécuter les exemples

## Tips :
Pour exécuter les exemples send et receive, il faut qu'il y ait un device CAN connecté au bus. Cela peut être le simulateur avec l'adaptateur PEAK CAN-USB :
- Pour cela, il faut connecter le fil vert au CANL, le fil bleu au CANH et le fil noir à une masse. 
Mais aussi le banc de test. ATTENTION, si vous souhaitez connecter le shiel CAN au banc de test, il faut qu'il soit dépourvu de résistance de terminaison (HAT avec noté sur la boite "Sans résistance").

Sans device, les tests échoueront. Mais on peut tout de même tester le fonctionnement duu code grâce au mode "loopback". Ainsi, les trames ne seront pas envoyées sur le bus CAN physique et le bus ne partira pas en erreur.

## Commandes :
- vérifier que la socket CAN est active : sudo ip -details -statistics link show can0
    Lorsque le bus CAN est en fonctionnement normal, il est en "ERROR-ACTIVE", sinon, il est "STOPPED" ou "ERROR-PASSIVE"

- activier la socket can : sudo ifconfig can0 up
- désactivier la socket can : sudo ifconfig can0 down

- aide : ip link help [ TYPE ]

- Activer ou désactiver le mode loopback :  sudo ip link set can0 up type can bitrate 125000 loopback on/off
    - Cette commande fait bcp plus que juste activer/désactiver le mode loopback
