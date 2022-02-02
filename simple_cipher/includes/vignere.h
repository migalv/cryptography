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

#ifndef VIGNERE_H
#define VIGNERE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <wchar.h>/*Para limpiar texto*/
#include <locale.h>/*Para limpiar texto*/

#include "../includes/matriz.h"
#include "../includes/parser.h"

// Maximo de argumentos de entrada
#define MAX_ARG_VIGNERE 8
// Minimo de argumentos de entrada
#define MIN_ARG_VIGNERE 5
// Caso de ayuda
#define AYUDA 2
// Una opcion
#define ONEOPTION 7
#define PADDING "X"
#define DESPLAZADOR_ASCII 65
#define MAX_TEXTO 501

/****************************************************************
 * Funcion que implementa el algoritmo de cifrado de Vignere. Recibe
 * un texto plano, que será cifrado, dada la clave k.
 *
 * Function: vignere_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-char* --> Clave
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void vignere_cifrar(char* fin, char* fout, char* k);

/****************************************************************
 * Funcion que implementa el algoritmo de descifrado de Vignere. Recibe
 * un texto cifrado, que será descifrado, dada la clave k.
 *
 * Function: vignere_descifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-char* --> Clave
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void vignere_descifrar(char* fin, char* fout, char* k);

#endif