/**
 * @file 	example.h
 * @brief	Sert de modèle pour le Projet Prose A1
 * @author 	TROVALLET Romain & BOUY Hugo
 * @license 	MIT License
 * @date 	10/03/2023
 *
 * Copyright (c) 2023 ProSE-A1-2024
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 */

#ifndef EXAMPLE_H
#define EXAMPLE_H

/*
 * INCLUDES
 */
 #include <stdint.h>

/*
 * DEFINES
 */

// Des fonctions de debug
 #define TRACE(fmt, ...) do { \
		fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		fflush (stderr); \
	} while(0);

#define STOP_ON_ERROR(error_condition) do { \
	if(error_condition) { \
		fprintf (stderr, "*** Error (%s) at \
		%s:%d\nExiting\n", \
		#error_condition, __FILE__, __LINE__); \
		perror (""); \
		exit (EXIT_FAILURE); }                     \
    } while (0);

/*
 * TYPEDEF
 */

// Définition d'un type structure suivant le PascalCase et finissant par _t dont les attributs sont protégés
/**
* @brief Type pour instencier un objet Model_t 
*/
typedef struct Model_tag Model_t;

// Définition d'une structure suivant le PascalCase et finissant par _t
/**
 * @brief Structure de données représentant une zone mémoire
 */
typedef struct{
	char * pData;
	int id;
}StructExample_t;

//Définition d'un type énumération suivant le PascalCase et finissant par _t
/**
 * @brief Énumération pour une machine à état
 */
typedef enum 
{
	INIT =0,
	IDLE,
	RUNNING,
	ENUM_SIZE
}EnumExample_t;

/*
 * PUBLIC FUNCTIONS
 */

/**
 * @fn exampleFuction 
 *
 * @brief Copie de données vers une zone mémoire à un index prédéfini
 * @param [in,out] pMemory La zone mémoire recevant les données
 * @param [in] index L'index de la zone mémoire recevant les données
 * @param [in] pData Les données copiées dans la zone mémoire
 * @pre sizeof(pMemory.pData)>= sizeof (pData) / INDEX_MIN < index < INDEX_MAX
 * @post pMemory à reçu les données pData à l'emplacement index
 * @ret index L'index de la zone mémoire recevant les données 
 */
int exampleFunction(StructExample_t * pMemory, uint8_t index, char * pData);

/**
 * @fn modelFree
 *
 * @brief destruct the object Model from dynamic memory
 * @param [in] Model_t*: pointer to the Model structure in the heap
 */
void modelFree(Model_t pModel);

/**
 * @fn modelNew
 *
 * @brief initialize in dynamic memory an object model_t
 * @ret Model_t*: pointer to the Model structure in the heap
 */
Model_t modelNew(void);

#endif /* EXAMPLE_H */
