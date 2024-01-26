/**
 * @file : main_test.c
 *
 * @brieffichier main de test qui permet de lancer les tests.
 *
 * @version : 0.0.0
 *
 * @date : 05/06/2023
 *
 * @author : Elisa DECLERCK
 * 
 * @copyright © Copyright Équipe ProSE B1 2024 CANvengers
 * 
 * \b License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*****************************\
|*	Déclaration des include  *|
\*****************************/

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/****************************\
|*	Déclaration des define  *|
\****************************/

/** 
 * @brief Nombre de suites de tests à exécuter. 
 * 
 * @note A modifier si besoin est.
 */
#define NB_SUITE_TESTS 3

/************************************************\
|*	Déclaration des fonctions globales externes *|
\************************************************/

extern int dispatcher_run_tests();

extern int proxyLogger_run_tests();

extern int proxyGUI_run_tests();

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

/** 
 * @brief Liste des suites de tests à exécuter. 
 * 
 * @note A compléter/modifier si besoin est.
 */
int (* suite_tests[])(void) = 
{ 
	dispatcher_run_tests,
	proxyLogger_run_tests,
	proxyGUI_run_tests
};

/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

static void test_runner();
static void affiche_aide(char * nom);


/***************************************\
|*	Description des fonctions locales  *|
\***************************************/

/**
 * @fn static void test_runner()
 * 
 * @brief Lancement de chacune des suites de test avec CMocka.
 */
static void test_runner()
{
	int i;
	
	for (i = 0; i < NB_SUITE_TESTS; i++)
	{
		suite_tests[i]();
	}
}


/**
 * @fn static void affiche_aide(char * nom)
 * 
 * @brief Affichage d'une aide sur la sortie standard pour utiliser le programme de test.
 * 
 * @param[in] nom nom du programme de test
 */
static void affiche_aide(char * nom)
{
	printf("Suite de tests pour le programme à tester\n");
	printf("Usage : %s [-text] [-subunit] [-tap] [-xml]\n", nom);

	exit(0);
}

/*************************\
|*	Description du main  *|
\*************************/

/**
 * @fn int main(int argc, char *argv[])
 *
 * @brief Fonction principale du module
 * 
 * @param[in] argc nombre d'arguments
 * 
 * @param[in] argv arguments
*/
int main(int argc, char * argv[])
{
	if ( (argc > 1) )
	{
		if (! strcmp(argv[1], "-text")) {
			cmocka_set_message_output(CM_OUTPUT_STDOUT);
		} else 
		if (! strcmp(argv[1], "-subunit")) {
			cmocka_set_message_output(CM_OUTPUT_SUBUNIT);
		} else 
		if (! strcmp(argv[1], "-tap")) {
			cmocka_set_message_output(CM_OUTPUT_TAP);
		} else
		if (! strcmp(argv[1], "-xml")) {
			cmocka_set_message_output(CM_OUTPUT_XML);
		} else {
			affiche_aide(argv[0]);
		}
	}

	test_runner();

	return 0;
}
