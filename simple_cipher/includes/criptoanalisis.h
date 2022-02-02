
/** 
 * Modulo encargado de implementar las funciones del cifrado 
 * y descifrado de vignere
 *
 * Modulo: vignere.c
 * Autor: Alberto Garcia Redero
 * Version: 1.0
 *
 * Fecha: 15/10/2018 
 */

#ifndef CRIPTOANALISIS_H
#define CRIPTOANALISIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Maximo de argumentos de entrada
#define MAX_ARG_ANALISIS 7
// Minimo de argumentos de entrada
#define MIN_ARG_ANALISIS 5
// Caso de ayuda
#define AYUDA 2
#define BUFFER_1MB 1048576
#define BUF_1000K 1000000


/****************************************************************
 * Funcion que implementa el criptoanalisis por el metodo de 
 * kasiski.
 *
 * Function: kasiski
 * Arguments: [0]-char* --> Fichero con texto cifrado
 *            [1]-char* --> Fichero donde se guardara el informe 
 *							del criptoanalisis
 *            [2]-char* --> Tamano del ngrama
 * Return: int, tamano de la clave con la que se cifrado el texto
 ****************************************************************/
int kasiski(char* fin, char* fout, int tam_ngrama);


/****************************************************************
 * Funcion que implementa el criptoanalisis por el metodo del
 * indice de coincidencia.
 *
 * Function: indice_coincidencia
 * Arguments: [0]-char* --> Fichero con texto cifrado
 *            [1]-char* --> Fichero donde se guardara el informe 
 *							del criptoanalisis
 *            [2]-char* --> Tamano del ngrama, se comprueba si este
 *							es el tamano real de la clave
 * Return: Nada, se guarda en el informe
 ****************************************************************/
void indice_coincidencia(char* fin, char* fout, int tam_ngrama);

/****************************************************************
 * Funcion que implementa el criptoanalisis por el metodo del
 * indice de coincidencia, obtiene la clave con la que se ha cifrado.
 *
 * Function: ic_clave
 * Arguments: [0]-char* --> Fichero con texto cifrado
 *            [1]-char* --> Fichero donde se guardara el informe 
 *							del criptoanalisis
 *            [2]-int   --> Tamano de la clave
 * Return: Nada, se guarda en el informe
 ****************************************************************/
void ic_clave(char* fin, char* fout, int tam_k);

/****************************************************************
 * Funcion que implementa el Algoritmo Knuth-Morris-Pratt, el cual
 * nos permite encontrar una subcadena en una cadena a un coste 
 * bajo, lo que ayuda a reducir los tiempos de busqueda.
 *
 * Function: kmp
 * Arguments: [0]-char* --> Cadena con el texto
 *            [1]-char* --> Subcadena a abuscar
 * Return: int, posicion donde se encuentra el inicio de la subcadena
 ****************************************************************/
int kmp(char* t, char* p);

/****************************************************************
 * Funcion que devuelve el elemento mas popular en un array de enteros,
 * es util para eliminar basura en kasiski
 *
 * Function: euclides
 * Arguments: [0]-int* --> Array de enteros
 *            [1]-int  --> Numero de elementos en el array
 *			  
 * Return: int, numero mas popular
 ****************************************************************/
int elemento_mas_popular(int* numeros, int n_ele);


#endif