#!/bin/bash

# Chemin d'accès aux fichiers générés du modèle du point de vue du Makefile du dossier de conception
model=../animUML/$1
objects=$2
interactions=$3
classes=$4
enumerations=$5
activities=$6

declare -A stateMachinesPerClass
. generateLaTeX.config

figure() {
	local file="$1"
	local caption="$2"
	local label="$3"
	echo '\begin{figure}[H]'
	echo '	\centering'
	echo "	\\includegraphics$args{$file}"
	echo "	\\caption{$caption}"
	echo "	\\label{fig:$label}"
	echo '\end{figure}'
}

args='[max width=\textwidth,max height=.9\textheight]'

(
	echo '% WARNING: automatically generated file that may be overwritten or removed at any time'
	echo
    echo '\section{Conception générale}'
	echo
	echo '\newcommand\macroSuffix{}' 
	echo "\\input{${model}-macros}"
	echo
	echo
	echo '\subsection{Architecture candidate}'
	echo
	figure "$model-context" "Architecture candidate" "archiCand"
	echo "Le diagramme de la \\autoref{fig:archiCand} représente l'architecture candidate du système."
    # On génère le fichier de description de l'architecture candidate (s'il n'existe pas)
    touch "../dossier_latex/sections/2_Conception_generale/Description_Arch_Candidate.tex"
    # On insère le fichier de description de l'architecture candidate sous le diagramme
    echo "\input{sections/2_Conception_generale/Description_Arch_Candidate.tex}"
	echo
	echo '\subsection{Diagrammes de séquence}'
	echo
	for inter in $interactions ; do
		echo "\\subsubsection{\\emph{${inter//_/ }}}"
		figure "$model-sequence-$inter" "Diagramme de séquence du \\emph{${inter//_/ }}" "inter-$inter"
		echo "Le diagramme de la \\autoref{fig:inter-$inter} représente le diagramme de séquence du \\emph{${inter//_/ }}."
        # On génère le fichier de description du diagramme de séquence (s'il n'existe pas)
        touch "../dossier_latex/sections/2_Conception_generale/Description_sequences/$inter.tex"
        # On insère le fichier de description du diagramme de séquence sous le diagramme
        echo "\input{sections/2_Conception_generale/Description_sequences/$inter.tex}"
		echo
	done
	echo
	echo '\subsection{Types de données}'
	echo
	figure "$model-datatypes" "Diagramme des types de données" "datatypes"
	echo "Le diagramme de la \\autoref{fig:datatypes} représente les types de données utilisés."
	echo "\newline"
	for enum in $enumerations ; do
		touch "../dossier_latex/sections/2_Conception_generale/Description_enums/$enum.tex"
		echo "\input{sections/2_Conception_generale/Description_enums/$enum.tex}"
		echo
		echo 'En voici le détail des énumérations:'
		echo "\\enum${enum}LiteralDescriptions"
		echo
	done
	touch "../dossier_latex/sections/2_Conception_generale/Description_types.tex"
	echo "\input{sections/2_Conception_generale/Description_types.tex}"
	echo
	echo '\subsection{Classes}'
	echo
	echo '\subsubsection{Vue générale}'
	echo
	figure "$model-classes" "Diagramme de classes" "classes"
	echo "Le diagramme de la \\autoref{fig:classes} représente les classes du système."
	echo
	for class in $classes ; do
		echo "\\subsubsection{La classe $class}"
		echo
		echo "Le diagramme de la \\autoref{fig:class-${class}} représente la classe $class."
		figure "$model-class-$class" "Diagramme de la classe $class" "class-$class"
            # On génère le fichier de description du diagramme de classe (s'il n'existe pas)
        touch "../dossier_latex/sections/2_Conception_generale/Description_classes/$class.tex"
        # On insère le fichier de description du diagramme de classe sous le diagramme
        echo "\input{sections/2_Conception_generale/Description_classes/$class.tex}"
		echo
		echo '\paragraph{Attributs}'
		echo "\\class${class}Properties"
		echo '\paragraph{Services offerts}'
		echo "\\class${class}Operations"

		if [ "${stateMachinesPerClass[$class]}" != "" ] ; then
			echo '\paragraph{Description comportementale}'
			for stateMachine in ${stateMachinesPerClass[$class]} ; do
				stateName=${stateMachine#*.}
				if [ "$stateMachine" = "$stateName" ] ; then
					objectName="$stateMachine"
					figure "$model-$stateMachine-SM" "Machine à états de \\emph{${class//_/ }}" "sm-$stateMachine"
					echo "Le diagramme de la \\autoref{fig:sm-$stateMachine} représente la machine à états de \\emph{${class//_/ }}".
    			    # On génère le fichier de description de la MAE parente (s'il n'existe pas)
    			    touch "../dossier_latex/sections/2_Conception_generale/Description_MAE/$stateMachine.tex"
					# On insère le fichier de description de la MAE parente sous le diagramme
					echo "\input{sections/2_Conception_generale/Description_MAE/$stateMachine.tex}"
				else
					figure "$model-$objectName-$stateMachine-SM" "Sous-état \\emph{${stateName}} de \\emph{${class//_/ }}" "sm-$stateMachine"
					echo "Le diagramme de la \\autoref{fig:sm-$stateMachine} représente le sous-état \\emph{$stateName} de la machine à états de \\emph{${class//_/ }}".
    			    # On génère le fichier de description de la MAE parente (s'il n'existe pas)
    			    touch "../dossier_latex/sections/2_Conception_generale/Description_MAE/$objectName-$stateMachine-SM.tex"
					# On insère le fichier de description de la MAE parente sous le diagramme
					echo "\input{sections/2_Conception_generale/Description_MAE/$objectName-$stateMachine-SM.tex}"
				fi
			done
		fi
	done
	# Si diagramme d'activité : décommenter et déplacer dans la section associée à la classe
	# echo
	# echo "\\subsection{Diagrammes d'activité}"
	# echo
	# for activity in $activities ; do
	# 	op=${activity#*.}
	# 	op=${op//./::}
	# 	figure "{{$model-activity-$activity}}" "Diagramme d'activité de l'opération \\emph{$op}" "activity-$activity"
	# 	echo "Le diagramme de la \\autoref{fig:activity-$activity} représente le comportement de l'opération \\emph{$op}."
    #     # On génère le fichier de description du diagramme d'activité (s'il n'existe pas)
    #     touch "../dossier_latex/sections/2_Conception_generale/Description_activities/$activity.tex"
    #     # On insère le fichier de description du diagramme d'activité sous le diagramme
    #     echo "\input{sections/2_Conception_generale/Description_activities/$activity.tex}"
	# 	echo
	# done
) > ../dossier_latex/sections/2_Conception_generale/2_Conception_generale.tex
