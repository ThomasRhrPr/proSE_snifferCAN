/**
*@file example.c
*@brief	Sert de modèle pour le Projet Prose A1
*@author TROVALLET Romain
*@date 10/03/2023
*/

public class ClassExample
{	


	/*
	* VARIABLES LOCALES
	*/
	
	private int exampleValue;
	private int exampleTable [];
	
	
	/*
	* Fontions
	*/	
	
	
	/*
	*@param[in] valueInit Exemple remplissage classe
	*@brief Constructeur de la classe
	*/	
	public ClasseExample (int valueInit)
	{
		exampleValue = valueInit;
		exampleTable = new int [20];	
	}
	
	/*
	*@param[out] exampleValue Retourne la valeur de la classe
	*@brief Getteur de la classe
	*/
	int getValue()
	{
		return exampleValue;
	}
	
	/*
	*@param[in] value Remplie la valeur de la classe
	*@brief Setteur de la classe
	*/
	void setValue(int value)
	{
		exampleValue = value;
	}
	
	/*
	*@param[in] increment Incrémente la valeur de la classe
	*@brief Incrémentation de la valeur de la classe
	*/
	void incrementValue(int increment)
	{
		exampleValue+= increment;
	}
	
	
	String searchTable(int value)
	{
		int i;
		boolean exit = FALSE;
		String returnMessage;
		
		for (i=0; i<20 && !exit; i++)
		{
			if(exampleTable[i] == value)
			{
				exit = TRUE;
			}
			else
			{
				i++;
			}
		}
		if (exit)
		{
			returnMessage = "La valeur " + value +
					 " a été trouvée au rang " + i + 
					 "dans le tableau.";
		}
		else
		{
			returnMessage = "La valeur n'a pas été trouvé dans le tableau.";
		}
			
		return returnMessage;
}





