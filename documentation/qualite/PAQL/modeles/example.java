/**
 * @file example.java
 * 
 * @brief Fichier listant l'ensemble des règles de codage pour le projet ProSE S8 de l'ESEO Angers.
 *		  Ce fichier ne concerne que l'équipe B1 2024 "CANvengers".
 * 
 * @version 1.0.0
 * 
 * @date 04/04/2023
 * 
 * @author Paul TRÉMOUREUX
 * 
 * @todo 
 * 
 * @copyright © Copyright Équipe ProSE B1 2024 CANvengers
 * 
 * \b License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**********************************************************************************************************************************************\
|*	Toute déclaration de sous-partie sera fera de cette manière. Entre les barres et le texte il faut faire une tabulation et non un espace.  *|
|*	Les sous-parties doivent être disposées de la même manière dans tous les fichiers de code.												  *|
|*	L'ordre d'apparition est le suivant : Déclaration des packages, Déclaration des imports, Déclaration des variables globales,			  *|
|*										  Déclaration des attributs, Déclaration des abstracts, Déclaration des constructeurs,				  *|
|*										  Déclaration des get/set, Description des fonctions publiques, Description des fonctions protégées,  *|
|*										  Description des fonctions privées.																  *|
\**********************************************************************************************************************************************/

/*
	Tout commentaire se fera de la manière suivante. Il ne faut pas faire 4 espaces mais une tabulation.
	Tout commentaire de type double barre oblique est interdit.
	Chaque section de commantaire ne traite que d'un sujet.
*/

/*
	Pour traiter d'une autre sujet, il faut créer une nouvelle section de commantaire.
*/

/******************************\
|*	Déclaration des packages  *|
\******************************/

package fr.eseo.b12024.candroid;

/*****************************\
|*	Déclaration des imports  *|
\*****************************/

import android.os.Bundle;

/************\
|*	Classe  *|
\************/
public class Example extends Test
{
	/****************************************\
	|*	Déclaration des variables globales  *|
	\****************************************/
	
	/*
		Aucune affectation n'est faite lors de la déclaration d'une variable.
		Lors de la déclaration d'une variable globale, le prefixe "G_" lui sera appliqué.
		Les différents du nom de la variable sont collés.
		Le nom d'une variable globale est entièrement écrit en majuscules.
	*/
	public static final String G_TEST = "test";
	
	/*******************************\
	|*	Déclaration des attributs  *|
	\*******************************/
	
	/*
		Aucune affectation n'est faite lors de la déclaration d'un attribut.
		Lors de la déclaration d'un attribut, le prefixe "a_" lui sera appliqué.
		Les différents du nom de la variable sont collés.
		Le premier mot commence par une minuscule, les autres par une majuscule.
	*/
	private int a_attributClasse;
	
	
	/*******************************\
	|*	Déclaration des abstracts  *|
	\*******************************/
	
	/*
		Exemple de déclaration d'un abstract
	*/
	public abstract void fonctionAbstract();
	
	/***********************************\
	|*	Déclaration des constructeurs  *|
	\***********************************/

	/*
		Toute classe possède ses constructeurs.
		Aucune autre fonction d'une classe ne doit être décrite dans le fichier avant ses constructeurs.
		Aucune autre fonction d'une classe ne doit être appelée avant ses constructeurs.
		Toutes variables locales seront affectées pour la première fois dans les constructeurs.
	*/

	/**
	 * @fn Example
	 *
	 * @brief Permet d'initialiser les différentes variables locales de la classe sans préciser son attribut.
	*/
	public Example()
	{
		/*
			Une section de commantaires est indentées au même niveau que les lignes qu'elle commente.
		*/
		a_attributClasse = 0;
	}

	/**
	 * @fn Example
	 *
	 * @brief Permet d'initialiser les différentes variables locales de la classe en précisant son attribut.
	 *
	 * @param int p_parametre - Valeur que l'on veut attribuer à a_attributClasse
	*/
	public Example(int p_parametre)
	{
		a_attributClasse = int p_parametre;
	}
	
	/*****************************\
	|*	Déclaration des get/set  *|
	\*****************************/
	
	/**
	 * @fn getVariableLocale
	 *
	 * @brief Permet à un autre module de récuperer la valeur de v_variableLocale.
	 *
	 * @return int a_attributClasse - Attribut de la classe
	*/
	public int getVariableLocale()
	{
		return(a_attributClasse);
	}
	
	/*
	Tout paramètre prend le préfixe "p_" avant son nom.
	*/	
		
	/**
	 * @fn setVariableLocale
	 *
	 * @brief Permet à un autre module de modifier la valeur de v_variableLocale.
	 *
	 * @param int p_parametre - Valeur que l'on veut attribuer à a_attributClasse
	*/
	public void setVariableLocale(int p_parametre)
	{
		a_attributClasse = p_parametre;
	}
	
	/*****************************************\
	|*	Description des fonctions publiques  *|
	\*****************************************/

	/*
		Toute fonction globale doit être définie en public
	*/
	
	/**
	 * @fn fonctionCalcul
	 *
	 * @brief Exemple de fonction de calcul.
	 *
	 * @param int p_parametre - Valeur de paramètre
	 *
	 * @return int valeurRetour - Valeur de retour
	*/
	public int fonctionCalcul(int p_parametre)
	{
		/*
			La variable utilisée pour le retour doit être déclarée au début de la fonction.
			La(les) variable(s) utilisée(s) pour le calcul doit(doivent) être déclarée(s) au début de la fonction.
			Aucun calcul ne sera fait directement sur les variables en paramètre ou sur la variable de retour.
		*/
		int valeurRetour;
		valeurRetour = ExampleFonction(int p_parametre);
		return(valeurRetour);
		
	}
	
	/**
	 * @fn fonctionCalculSuper
	 *
	 * @brief Exemple de fonction de calcul utilisant la fonctionnalité "super".
	 *
	 * @param int p_parametre - Valeur de paramètre
	 *
	 * @return int valeurRetour - Valeur de retour
	*/
	public int fonctionCalculSuper(int p_parametre)
	{
		/*
			"super" doit être mis avant toute autre ligne dans la fonction.
		*/
		super.FonctionCalcul(p_parametre);
		int valeurRetour;
		valeurRetour = ExampleFonction(int p_parametre);
		return(valeurRetour);
		
	}
	
	/*****************************************\
	|*	Description des fonctions protégées  *|
	\*****************************************/
	
	/**
	 * @fn fonctionTest
	 *
	 * @brief Exemple de fonction de calcul.
	 *
	 * @param int p_parametre - Valeur de paramètre
	 *
	 * @return int valeurRetour - Valeur de retour
	*/
	protected int fonctionTest(int p_parametre)
	{
		int valeurRetour;
		valeurRetour = 0;
		if(p_parametre == a_attributClasse)
		{
			valeurRetour = 1;
		}
		return(valeurRetour);
	}
	
	/***************************************\
	|*	Description des fonctions privées  *|
	\***************************************/

	/*
		Toute fonction locale doit être définie en private.
	*/

	/**
	 * @fn exampleFonction
	 *
	 * @brief Fonction d'exemple.
	 *
	 * @param int p_parametreInt - Valeur de paramètre
	 *
	 * @return int valeurRetour - Valeur de retour
	*/
	private int exampleFonction(int p_parametre)
	{
		int variableRetour;
		int variableCalcul1;
		int variableCalcul2;
		/*
			Toute valeur utilisée dans un calcul doit être préalablement déclarée sous forme de variable.
		*/
		int valeurA = 6;
		int valeurB = 2;
		int valeurC = 1;
		/*
			Tout signe de calcul doit être espacé des autres caractères.
		*/
		variableCalcul1 = p_parametre + valeurTest;
		variableCalcul2 = p_parametre + (valeurA / (valeurB + valeurC));
		variableRetour = variableCalcul1 + variableCalcul2;
		return(variableRetour);
	}
	
	/*
		Toute fonction de gestion de l'affichage doit commencer par une miniscule.
	*/
	/**
	 * @fn exampleFonctionAffichage
	 *
	 * @brief Fonction d'exemple de fonction d'affichage.
	*/
	private void exampleFonctionAffichage()
	{
		/*
			...
		*/
	}
}
