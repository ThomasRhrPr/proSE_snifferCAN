/**
 * @file example.h
 *
 * @brief Fichier listant l'ensemble des règles de codage pour le projet ProSE S8 de l'ESEO Angers.
 *		 	Ce fichier ne concerne que l'équipe B1 2024 "CANvengers".
 *
 * @version 1.0.0
 *
 * @date 12/04/2023
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

#ifndef EXAMPLE_H_
#define EXAMPLE_H_

/*****************************\
|*	Déclaration des include  *|
\*****************************/

#include <stdio.h>

/****************************\
|*	Déclaration des define  *|
\****************************/

#define EXAMPLE_VALEUR_MIN_COORDONNEES (0)
#define EXAMPLE_VALEUR_MAX_COORDONNEES (50)

/*****************************\
|*	Déclaration des TypeDef  *|
\*****************************/

/*
	Le nom d'un typedef prend le nom du module suivi de "_" comme préfixe.
*/

/*
	Le nom d'une enumération prend "_e" comme suffixe.
*/

/**
 * @typedef example_bool_e
 *
 * @brief Enumération servant de booléen.
*/
typedef enum
{
	FALSE = 0,
	TRUE,
} example_bool_e;

/*
	Le nom d'une structure prend "_t" comme suffixe.
*/

/**
 * @typedef example_coordonnees_t
 *
 * @brief Structure servant de description de coordonnées.
*/
typedef struct
{
	int positionX;
	int positionY;
} example_coordonnees_t;

/*
	Le nom d'une union prend "_u" comme suffixe.
*/

/**
 * @typedef example_coordonnees_u
 *
 * @brief Structure servant de description de coordonnées.
*/
typedef union
{
	int x;
	float y;
	void *z;
} example_union_u;

/*
	Prédéfinition d'une structure détaillée dans le .c.
*/

/**
 * @typedef example_StructTag
 *
 * @brief Structure servant de description de coordonnées.
*/
typedef struct example_StructTag;

/****************************************\
|*	Déclaration des fonctions globales  *|
\****************************************/

void example_Init ();
int example_GetVariableLocale ();
void example_SetVariableLocale (int p_parametre);
void example_ExempleFonction (int *p_parametre_pt);

#endif /* EXAMPLE_H_ */
