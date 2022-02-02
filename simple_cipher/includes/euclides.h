/** 
 * Modulo para el algoritmo de euclides
 *
 * Modulo: euclides.c
 * Autor: Alberto García Redero
 *
 * !ATENCION	Este módulo utiliza gmp para todos los enteros
 *
 * Fecha: 8/09/2018 
 */

// Liberia para uso de números enteros muy grandes
#include <gmp.h>

#ifndef EUCLIDES_H
#define EUCLIDES_H


/****************************************************************
 * Función que implementa el algoritmo de euclides extendido.
 * Se devuelven los valores como parametro.
 * Además utiliza la libreria GMP para el manejo de los enteros
 *
 * Function: euclides_gmp
 * Arguments: [0]-mpz_t a --> Entero a de la funcion euclides
 *			  [1]-mpz_t b --> Entero b de la funcion euclides
 *			  [2]-mpz_t d --> Resultado de mcd(a, b)
 *			  [3]-mpz_t s --> Inverso de a en Zd
 *			  [2]-mpz_t t --> Inverso de b en Zd
 * Return: Devuelve todos los valores por parametro
 ****************************************************************/
void euclides_gmp(mpz_t a, mpz_t b, mpz_t d, mpz_t s, mpz_t t);

/****************************************************************
 * Función que implementa el algoritmo de euclides extendido
 * Se devuelven los valores como parametro.
 *
 * Function: euclides
 * Arguments: [0]-int a --> Entero a de la funcion euclides
 *			  [1]-int b --> Entero b de la funcion euclides
 *			  [2]-int* d --> Resultado de mcd(a, b)
 *			  [3]-int* s --> Inverso de a en Zd
 *			  [2]-int* t --> Inverso de b en Zd
 * Return: Devuelve d, s y t como por parametro
 ****************************************************************/
void euclides(int a, int b, int* d, int* s, int* t);

#endif