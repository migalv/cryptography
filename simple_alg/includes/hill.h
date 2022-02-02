/**
 * Modulo encargado de implementar las funciones del cifrado
 * y descifrado de hill
 *
 * Modulo: hill.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 15/10/2018
 */

#ifndef HILL_H
#define HILL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include <wchar.h>/*Para limpiar texto*/
#include <locale.h>/*Para limpiar texto*/

#include "../includes/matriz.h"
#include "../includes/parser.h"

#define PADDING "X"
#define DESPLAZADOR_ASCII 65
#define MAX_TEXTO 501

/****************************************************************
 * Funcion que implementa el algoritmo de cifrado Hill. Recibe
 * un texto plano, que será cifrado, dada la clave alojada en la
 * matrizK.
 *
 * Function: cifrar_hill
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-int --> Tamanio de la matrizK
 *            [2]-int** --> Matriz clave
 * Return: char* Texto cifrado
 ****************************************************************/
char* cifrar_hill(char* texto_plano, int dimensionK, int** matrizK);

/****************************************************************
 * Funcion que implementa el algoritmo de descifrado Hill. Recibe
 * un texto plano, que será descifrado, dada la clave alojada en la
 * matrizK.
 *
 * Function: descifrar_hill
 * Arguments: [0]-char* --> Texto a descifrar
 *            [1]-int --> Tamanio de la matrizK
 *            [2]-int** --> Matriz clave
 * Return: char* Texto descifrado
 ****************************************************************/
char* descifrar_hill(char* texto_cifrado, int dimensionK, int** matrizK);

/****************************************************************
 * Funcion utilizada para introducir "X" al final de un bloque,
 * en caso de que éste no sea múltiplo del tamaño de la matriz.
 *
 * Function: padding
 * Arguments: [0]-char* --> Texto a añadir las "X"
 *            [1]-int --> Tamanio de la matrizK
 * Return: char* El texto con las "X" al final
 ****************************************************************/
char* padding(char* text, int tamanio);

int comprueba_inyectividad(int* permutacion, int tamanio);

#endif
