# Compilation croisée

Vous devez cloner sur votre machine le répertoire tools dans le dossier production (vous ne pouvez pas le commit sur le repo): https://github.com/raspberrypi/tools.git

# Makefile récusifs 
Le code dans le dossier production est disposé sous forme de modules (= dossiers) contenant un fichier source et un fichier header. 
Si vous souhaitez ajouter des modules, il faut modifier le Makefile au niveau du dossier "src" et ajouter votre module aux packages à compiler (voir commentaires).
!!!! Afin de compiler, vous devez indiquer le chemin de votre compilateur dans le fichier src/variables.mk !!!!

# Debugger :
Voici me mien du tuto que nous avons suivi : https://nnfw.readthedocs.io/en/stable/howto/how-to-remote-debugging-with-visual-studio-code.html
Pour utiliser le debugger, une tâche dans le fichier launch.json a été créée :

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "gdb",
            "request": "attach",
            "name": "Debug CANgateway",
            "gdbpath": "/usr/bin/gdb-multiarch",
            "executable": "./home/pi/CANgateway.out",
            "target": "10.42.0.42:3333",
            "remote": true,
            "printCalls": true,
            "cwd": "${workspaceRoot}/production/src",
            "valuesFormatting": "parseText"
        }
    ]
}
```

Pour le faire fonctionner, il faut exécuter la commande suivant sur la target :
gdbserver --multi :<PORT> <BINARY_PATH> <EXECUTION_ARGUMENTS>
Ce qui donne, dans notre cas : 
gdbserver --multi :3333 CANgateway.out