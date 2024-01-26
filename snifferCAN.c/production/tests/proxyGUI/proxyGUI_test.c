/**
 * @file : proxyGUI_test.c
 * 
 * @brief Fichier de test du module proxyGUI qui permet de tester le bon fonctionnement de la méthode de décodage de message.
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
#include <assert.h>


#include "proxyGUI/proxyGUI.c"

/****************************************\
|*	Déclaration des fonctions locales   *|
\****************************************/

static int set_up(void **state);

static int tear_down(void **state);

static void test_encodeMessage(void **state);

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/
typedef struct 
{
    SendingState_e sendingState;
    char message[17];
} DonneeTest_t;

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

DonneeTest_t proxyGUI_donneeTest1 = 
{
    .sendingState = SENDING_STATE_ON,
    .message = "~sendingState#0\n"
};

DonneeTest_t proxyGUI_donneeTest2 = 
{
    .sendingState = SENDING_STATE_OFF,
    .message = "~sendingState#1\n"
};

/**
 * @brief Suite de test pour le module.
 *
 * @note compléter si besoin est.
 */
static const struct CMUnitTest tests_param[] = 
{
	cmocka_unit_test_prestate(test_encodeMessage, &proxyGUI_donneeTest1),
    cmocka_unit_test_prestate(test_encodeMessage, &proxyGUI_donneeTest2)
};


/***************************************\
|*	Déclaration des fonctions locales  *|
\***************************************/

/**
 * @fn static int set_up(void **state)
 * 
 * @brief Fonction de set_up du module. Elle est appelée avant chaque méthodes de test.
 * 
 * @param state 
 * 
 * @return succès ou échec du set_up
 */
static int set_up(void **state) 
{
	return 0;
}

/**
 * @fn static int tear_down(void **state)
 * 
 * @brief Fonction de tear_down du module. Elle est appelée après chaque méthodes de test.
 * 
 * @param state 
 * 
 * @return succès ou échec du tear_down
 */
static int tear_down(void **state) 
{
	return 0;
}

/**
 * @fn static void test_encodeMessage(void **state) 
 * 
 * @brief Test de la méthode d'encodage de message.
 * 
 * @param state 
 */
static void test_encodeMessage(void **state) 
{
    /* Récupération des données de tests */
    DonneeTest_t *donneeTest = (DonneeTest_t *) *state;

    char expectedMessage[sizeof(donneeTest->message)];

    for (int i = 0 ; i < sizeof(donneeTest->message) ; i++)
    {
    	expectedMessage[i] = donneeTest->message[i];
    }

    char message[sizeof(expectedMessage)];

    proxyGUI_EncodeMessage(donneeTest->sendingState, message);

    printf("expected : %s", expectedMessage);
    printf("real : %s", message);
    
    for (int i = 0 ; i < sizeof(message) ; i++)
    {
    	assert_int_equal(message[i], expectedMessage[i]);
    }
}

/**
 * @fn int dispatcher_run_tests()
 * 
 * @brief Lancement de la suite de tests pour le module.
 */
int proxyGUI_run_tests() 
{
	return cmocka_run_group_tests_name("Test du module proxyGUI (param)", tests_param, set_up, tear_down);
}