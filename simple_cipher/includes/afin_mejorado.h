/** 
 * Modulo encargado de implementar las funciones del cifrado 
 * y descifrado afin mejorado
 *
 * Modulo: afin.h
 * Autor: Alberto Garcia Redero y Miguel Alvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 7/10/2018 
 */

#ifndef AFIN_M_H
#define AFIN_M_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <math.h>

#include <wchar.h>/*Para limpiar texto*/
#include <locale.h>/*Para limpiar texto*/

// Maximo de argumentos de entrada
#define MAX_ARG_AFIN_M 8
// Minimo de argumentos de entrada
#define MIN_ARG_AFIN_M 5
// Caso de ayuda
#define AYUDA 2
// Tamano buf de lectura por pantalla
#define BUF_TAM 25000
#define TAM_FICH 250


/****************************************************************
 * Funcion que implementa el algoritmo de cifrado afin mejorado
 * en este se cifra dos veces modificando en la segunda el modulo,
 * el cual es b*m
 *
 * Function: afin_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-mpz_t --> a para ax+b mod m
 *            [3]-mpz_t --> b para ax+b mod m
 *            [4]-mpz_t --> m para ax+b mod m, modulo del 
 *											   primer cifrado
 *            [5]-mpz_t --> m2 para ax+b mod m2, modulo del 
 *											   segundo cifrado
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void afin_mejorado_cifrar(char* fin, char* fout, mpz_t a, mpz_t b, mpz_t m, mpz_t m2);

/****************************************************************
 * Funcion que implementa el algoritmo de descifrado afin mejorado
 *
 * Function: afin_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-mpz_t --> b para (y-b)*(a^-1) mod m
 *            [3]-mpz_t --> s=(a^-1) para (y-b)*(a^-1) mod m
 *            [4]-mpz_t --> s=(a^-1) para (y-b)*(a^-1) mod m2
 *            [5]-mpz_t --> m para (y-b)*(a^-1) mod m
 *            [6]-mpz_t --> m2 para (y-b)*(a^-1) mod m2
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void afin_mejorado_descifrar(char* fin, char* fout, mpz_t b, mpz_t s, mpz_t s2, mpz_t m, mpz_t m2);

#endif