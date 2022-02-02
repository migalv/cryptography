/*
 * Modulo encargado de comprobar la linealidad
 * perfecta
 *
 * Modulo: seg-perf.h
 * Autor: Alberto Garcia Redero
 */


#ifndef SEGPERF_H
#define SEGPERF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <wchar.h>/*Para limpiar texto*/
#include <locale.h>/*Para limpiar texto*/

// Maximo de argumentos de entrada
#define ABECEDARIO 26
// Maximo de argumentos de entrada
#define MAX_ARG 6
// Caso de ayuda
#define AYUDA 2
// Tamano buf de lectura por pantalla
#define TAM_FICH 25000
#define TAM_NOMBRE_FICH 250

extern int nCaracteres;

/****************************************************************
 * Funcion que guarda las probabilidades de ocurrencia de los
 * caracteres en el texto plano y las de estas respecto a como
 * se han cifrado
 *
 * Function: afin_cifrar
 * Arguments: [0]-char* --> Texto sin cifrar
 *            [1]-char* --> Texto cifrado
 *            [2]-in*   --> lista con las repeticiones de las
 *							letras en el texto plano
 *            [3]-char* --> fichero donde se guarda el resultado
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void estadistica(char* texto_plano, char* texto_cifrado, int* repeticiones, char* salida);

/****************************************************************
 * Funcion que realiza una función de cifrado sobre el texto texto_plano
 * con una probabilidad de las claves no equiprobable.
 *
 * Function: cifrado_no_equiprobable
 * Arguments: [0]-char* --> Texto sin cifrar
 *
 * Return: cadena de caracteres con el texto cifrado
 ****************************************************************/
char* cifrado_no_equiprobable(char* texto_plano);

/****************************************************************
 * Funcion que realiza una función de cifrado sobre el texto texto_plano
 * con una probabilidad de las claves equiprobable.
 *
 * Function: cifrado_equiprobable
 * Arguments: [0]-char* --> Texto sin cifrar
 *
 * Return: cadena de caracteres con el texto cifrado
 ****************************************************************/
char* cifrado_equiprobable(char* texto_plano);

/****************************************************************
 * Funcion que busca las repeticiones del alfabeto en la cadena de
 * entrada.
 *
 * Function: repeticiones_en_texto
 * Arguments: [0]-char* --> Texto sin cifrar
 *
 * Return: array con las repeticiones de cada caracter en el texto
 ****************************************************************/
int* repeticiones_en_texto(char* entrada);
#endif
