/**
 * @file 	example.c
 * @brief 	Sert de modèle pour le Projet Prose A1
 * @author 	TROVALLET Romain & BOUY Hugo
 * @license 	MIT License
 * @date 	10/03/2023
 *
 * Copyright (c) 2023 ProSE-A1-2024
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * INCLUDES
 */

// Bibliothèques standardes donc utilisation de chevrons
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Fichier .h associé et interne au projet donc utilisation de guillemets
#include "example.h"

/*
 * DEFINES
 */

// Les defines sont en majuscule et séparés par des underscores. Les valeurs associées sont protégées par des parenthèses.
#define INDEX_MIN (2)
#define INDEX_MAX (4096)

/*
 * TYPEDEF
 */

/**
 * @brief Modèle tag
 */
struct Model_tag{
	uint8_t attribute;
};

/*
 * LOCAL FUNCTIONS
 */
 static void uneFonctionLocale(void);

/*
 * LOCAL VARIABLES
 */

//Déclaration d'une variable locale au module suivant le camelCase
static int staticVariableExample = 20;

 /*******************************************************************************************/

/**
* @fn uneFonctionLocale
* @brief Fonction d'exemple qui ne fait rien
*/
static void uneFonctionLocale(void)
{

}

int exampleFunction(StructExample_t * pMemory, uint8_t index, char * pData)
{
	// Exemple d'un commentaire sur 1 ligne
	if (index <= INDEX_MAX && index > INDEX_MIN)
	{
		pMemory.pData = pData
		pMemory.id = index;
		index--;
	}
#if 0 // Exemple d'une partie de code désactivée
	else if (index > INDEX_MAX)
	{
		index = INDEX_MAX;
	}
#endif
	else
	{
		index = INDEX_MIN;
	}
	return index;	
}

Model_t modelNew(void)
{
    Model_t * pModel = NULL;
    pModel = (Model_t) malloc(sizeof(Model_t));

    if(pModel == NULL)
    {
        printf("Malloc failed for Model");
        while(1);
    }
    return pModel;
}

void modelFree(Model_t pModel)
{
    free(pModel);
}
