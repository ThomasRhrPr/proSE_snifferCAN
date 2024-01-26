/**
 * @file example.c
 * 
 * @brief Fichier listant l'ensemble des règles de codage pour le projet ProSE S8 de l'ESEO Angers.
 *		 	Ce fichier ne concerne que l'équipe B1 2024 "CANvengers".
 * 
 * @version 1.0.0
 * 
 * @date 04/04/2023
 * 
 * @author Paul TRÉMOUREUX
 * 
 * @todo Balise license.
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
|*	L'ordre d'apparition est le suivant : Déclaration des include, Déclaration des define, Déclaration des TypeDef,							  *|
|*										  Déclaration des variables locales, Déclaration des fonctions locales,			  					  *|
|*										  Description des fonctions globales, Description des fonctions locales, Description du main.	 	  *|
\**********************************************************************************************************************************************/

/*
	Tout commentaire se fera de la manière suivante. Il ne faut pas faire 4 espaces mais une tabulation.
	Tout commentaire de type double barre oblique est interdit.
	Chaque section de commantaire ne traite que d'un sujet.
*/

/*
	Pour traiter d'une autre sujet, il faut créer une nouvelle section de commantaire.
*/

/*****************************\
|*	Déclaration des include  *|
\*****************************/

/*
	Appel de bibliothèques externes au projet.
*/
#include <stdlib.h>

/*
	Appel de bibliothèques internes au projet.
	Les déclarations de bibliothèques internes doivent être mises après celles de bibliothèques externes.
*/
#include "example.h"

/****************************\
|*	Déclaration des define  *|
\****************************/

/*
	Pour la déclaration d'un define, le nom doit ête mis en majuscules avec chaque mot séparé d'un "_".
	Les valeurs associées aux define sont misent entre parenthèses.
	Ne pas oublier de séparer la parenthèses du nom par un espace.
*/
#define EXAMPLE_DEFINE_TEST (0)

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/

struct example_StructTag {
	char nomTag;
} example_StructTag_t;

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

/*
	Toute variable sera locale.
	Aucune affectation n'est faite lors de la déclaration d'une variable.
	Lors de la déclaration d'une variable static, le prefixe "s_" lui sera appliqué.
	Lors de la déclaration d'une variable static volatile, le prefixe "sv_" lui sera appliqué.
	Lors de la déclaration d'une variable static const, le prefixe "sc_" lui sera appliqué.
	Les différents du nom de la variable sont collés.
	Le premier mot commence par une minuscule, les autres par une majuscule.
*/
static int s_variableLocale;
static volatile int sv_variableLocaleVolatile;
static const int sc_variableLocaleConst;
/*
	Lors de la déclaration d'un pointeur, le suffixe "_pt" lui sera appliqué.
	Lors de l'utilisation d'un pointeur, aucun espace ne doit être mis entre '*' et le nom de la variable.
*/
static int *s_pointeurLocal_pt;

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static int ExampleFonction (int p_parametreInt, example_bool_e p_parametreBool);

/****************************************\
|*	Description des fonctions globales  *|
\****************************************/

/*
	Toute fonction globale possède le nom du module suivi d'un "_" en tant que préfixe.
*/

/*
	Tout programme possède sa fonction d'initialisation.
	Aucune autre fonction d'un module ne doit être décrite dans le fichier avant sa fonction d'initialisation.
	Aucune autre fonction d'un module ne doit être appelée avant sa fonction d'initialisation.
	Toutes variables locales seront affectées pour la première fois dans la fonction d'initialisation.
	Lors de la déclaration d'une fonction, mettre un espace entre le nom et la parenthèse.
*/

/**
 * @fn example_Init
 *
 * @brief Permet d'initialiser les différentes variables locales du module.
*/
void example_Init ()
{
	/*
		Une section de commantaires est indentées au même niveau que les lignes qu'elle commente.
	*/
	s_variableLocale = 0;
	sv_variableLocaleVolatile = 0;
	sc_variableLocaleConst = 0;
	
	s_pointeurLocal_pt = 0;
}

/*
	Les fonctions de get et set sont mises dans l'ordre de déclaration des variables locales.
	Pour une variable locale son get et son set sont mis à la suite avec le get avant le set.
*/

/*
	Pour modifier ou récuperer valeur d'une variable locale on utilise des fonctions set et get
	Une fonction publique possède le nom du module suivi d'un "_" en tant que préfixe.
*/

/**
 * @fn example_GetVariableLocale
 *
 * @brief Permet à un autre module de récuperer la valeur de s_variableLocale.
 *
 * @return int s_variableLocale
*/
int example_GetVariableLocale ()
{
	/*
		Une fonction ne retourne pas une valeur mais une variable.
		Toute variable retournée sera mise entre parenthèses.
	*/
	return (s_variableLocale);
}

/*
	Tout paramètre prend le préfixe "p_" avant son nom.
*/	
	
/**
 * @fn example_SetVariableLocale
 *
 * @brief Permet à un autre module de modifier la valeur de s_variableLocale.
 *
 * @param int p_parametre
*/
void example_SetVariableLocale (int p_parametre)
{
	s_variableLocale = p_parametre;
}

/**
 * @fn example_ExempleFonction
 *
 * @brief Fonction d'exemple.
 *
 * @param int *p_parametre_pt
*/
void example_ExempleFonction (int *p_parametre_pt)
{
	int variableCalcul1;
	int variableCalcul2;
	int valeurA = 1;
	int valeurB = 2;
	int valeurC = 3;
	variableCalcul1 = (valeurA + valeurB) / (valeurC);
	variableCalcul2 = variableCalcul1 * *p_parametre_pt;
	variableCalcul1 += variableCalcul2;
	/*
		L'affectation de la valeur au pointeur n'est affecté qu'à la fin de la fonction.
	*/
	*p_parametre_pt = variableCalcul1;
	
}

/***************************************\
|*	Description des fonctions locales  *|
\***************************************/

/*
	Toute fonction locale doit être définie en static
*/

/**
 * @fn ExampleFonction
 *
 * @brief Fonction d'exemple.
 *
 * @param int p_parametreInt - description
 * @param bool_e p_parametreBool - description
 *
 * @return int variableRetour
*/
static int ExempleFonction (int p_parametreInt, example_bool_e p_parametreBool)
{
	/*
		La variable utilisée pour le retour doit être déclarée au début de la fonction.
		La(les) variable(s) utilisée(s) pour le calcul doit(doivent) être déclarée(s) au début de la fonction.
		Aucun calcul ne sera fait directement sur les variables en paramètre ou sur la variable de retour.
	*/
	int variableRetour;
	int variableCalcul1;
	int variableCalcul2;
	/*
		Toute valeur utilisée dans un calcul doit être préalablement déclarée sous forme de variable.
	*/
	int valeurA = 3;
	int valeurB = 2;
	int valeurC = 1;
	/*
		Tout signe de calcul doit être espacé des autres caractères.
	*/
	if (p_parametreBool == TRUE)
	{
		variableCalcul1 = (valeurA + valeurB) / (valeurC);
	}
	else
	{
		variableCalcul1 = (valeurA - valeurB) / (valeurC);
	}
	variableCalcul2 = variableCalcul1 * p_parametreInt;
	variableCalcul1 += variableCalcul2;
	variableRetour = variableCalcul1;
	return (variableRetour);
}

/*************************\
|*	Description du main  *|
\*************************/

/**
 * @fn main
 *
 * @brief Fonction principale du module.
*/
void main ()
{
	example_bool_e booleenX;
	example_bool_e booleenY;
	example_coordonnees_t coordonneesTest;
	
	/*
		Exemple de desactivation d'une partie de code.
	*/
#if 0
	booleenX = TRUE;
	booleenY = FALSE;
#endif
	
	/*
		Exemple d'activation d'une partie de code.
	*/
#if 1
	booleenX = FALSE;
	booleenY = TRUE;
#endif

	/*
		Lors de l'appel d'une fonction, ne pas mettre d'espace avant la parenthèse.
	*/
	coordonneesTest.positionX = ExempleFonction(s_variableLocale, booleenX);
	coordonneesTest.positionY = ExempleFonction(s_variableLocale, booleenY);
}
