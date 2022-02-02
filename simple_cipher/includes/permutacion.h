/**
 * Modulo encargado de implementar las funciones del cifrado
 * y descifrado de doble permutacion
 *
 * Modulo: permutacion.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 22/10/2018
 */

#ifndef PERMUTACION_H
#define PERMUTACION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hill.h"

// Tamanio maximo de la permutacion
#define MAX_ELEMS 10

#define new_max(x,y) ((x) >= (y)) ? (x) : (y)
#define new_min(x,y) ((x) <= (y)) ? (x) : (y)

char* cifrar_doble(char* texto_plano, int* permutacionK1, int* permutacionK2, int num_k1, int num_k2);
char* descifrar_doble(char* texto_cifrado, int* permutacionK1, int* permutacionK2, int num_k1, int num_k2);

#endif
