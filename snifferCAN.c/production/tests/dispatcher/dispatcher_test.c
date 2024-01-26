/**
 * @file : dispatcher_test.c
 * 
 * @brief Fichier de test du module dispatcher qui permet de tester le bon fonctionnement de la méthode de décodage de message.
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

#include "dispatcher/dispatcher.c"

/****************************************\
|*	Déclaration des fonctions locales   *|
\****************************************/

static int set_up(void **state);

static int tear_down(void **state);

static void test_decodeMessage(void **state);

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/

typedef struct 
{
    char message[25];
    Frame_t frame;
} DonneeTest_t;

/***************************************\
|*	Déclaration des variables locales  *|
\***************************************/

DonneeTest_t donneeTest1 = 
{
    .message = "frame#19B$6@00000E/1000\n",
    .frame = 
    {
        .frame = 
        {
            .can_id = 0x19B,
            .can_dlc = 6,
            .data = {0x00, 0x00, 0x0E, 0x00, 0x00, 0x00}
        },
        .periodicity = 1000,
        .senderMode = SENDER_MODE_CYCLIC
    }
};

DonneeTest_t donneeTest2 = 
{
    .message = "frame#123$2@AA/0\n",
    .frame = 
    {
        .frame = 
        {
            .can_id = 0x123,
            .can_dlc = 2,
            .data = {0xAA, 0x00}
        },
        .periodicity = 0,
        .senderMode = SENDER_MODE_PUNCTUAL
    }
};

/**
 * @brief Suite de test pour le module.
 *
 * @note compléter si besoin est.
 */
static const struct CMUnitTest tests_param[] = 
{
	cmocka_unit_test_prestate(test_decodeMessage, &donneeTest1),
    cmocka_unit_test_prestate(test_decodeMessage, &donneeTest2),
};

/***************************************\
|*	Description des fonctions locales  *|
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
 * @fn static void test_decodeMessage(void **state)
 * 
 * @brief Test de la méthode de décodage de message.
 * 
 * @param state 
 */
static void test_decodeMessage(void **state) 
{
    /* Récupération des données de tests */
	DonneeTest_t donneeTest = *((DonneeTest_t*) *state);

    char* message = donneeTest.message;

    Frame_t frame;

    /* Appel de la méthode à tester */
    dispatcher_DecodeMessage(message, &frame);

    /* Vérifications */
    assert_int_equal(frame.periodicity, donneeTest.frame.periodicity);

    assert_int_equal(frame.senderMode, donneeTest.frame.senderMode);

    assert_int_equal(frame.frame.can_id, donneeTest.frame.frame.can_id);

    assert_int_equal(frame.frame.can_dlc, donneeTest.frame.frame.can_dlc);

    for (int i = 0; i < frame.frame.can_dlc; i++)
    {
        assert_int_equal(frame.frame.data[i], donneeTest.frame.frame.data[i]);
    }
}

/**
 * @fn int dispatcher_run_tests()
 * 
 * @brief Lancement de la suite de tests pour le module.
 */
int dispatcher_run_tests() 
{
	return cmocka_run_group_tests_name("Test du module dispatcher(param)", tests_param, set_up, tear_down);
}