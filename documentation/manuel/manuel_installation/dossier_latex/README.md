Ce fichier a pour but de rappeler comment générer le fichier spec_inc1.pdf

Pour cela, il vous suffit simplement d'ouvrir un terminal dans le dossier où se trouve le fichier spec_inc1.tex et de taper :
    xelatex spec_inc1.tex
Il vous génèrera le fichier spec_inc1.pdf
Si vous avez suivi le tuto de Théo, vous avez dû ajouter du contenu au fichier setting.json. Il vous permet de mettre à jour le fichier pdf juste en sauvegardant le fichier latex

Afin de générer les fichiers eps (qui servent à afficher les diagrammes plantuml), vous pouvez compiler le makefile (en tapant "make" dans le terminale)

Vous pouvez nettoyer le dépôt grâce au makefile en tapant "make clean" dans un terminale. Cela supprime tous les fichiers de compilation latex mais également les fichiers eps pour générer les fichier et le fichier pdf. Il faut donc retaper la commande "make" puis de build le fichier latex pour obtenir un nouvelle fois le fichier pdf.

Bugs possibles :
- Il se peut qu'après la première compilation du document pdf, la table des matière n'apparaisse pas. On ne sait pas encore pourquoi mais on sait que recompiler le fichier (ie retaper la commande ci-dessue) suffit à réparer le problème 
- Si la compilation échoue, vérifiez que vous avez bien les fichiers eps présents dans le dossier schemas
