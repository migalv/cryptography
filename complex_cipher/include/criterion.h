/**
 * Modulo encargado analizar los criterios del DES
 *
 * Modulo: criterion.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 23/11/2018
 */
#ifndef CRITERION_H
#define CRITERION_H


#include "DES_tables.h"
#include <stdio.h>
#include <stdlib.h>

// Total de entras posibles a una SBox
#define ENTRADAS 64

/****************************************************************
* Funcion que calcula las probabilidades del criterio SAC sobre
* las salidas de la SBox pasada como argumento.
*
* IMPORTANTE! -> La caja de sustitucion debe ser de 4x16.
* 			   4 Filas y 16 Columnas.
*
* ATENCION! -> Sera necesario liberar memoria del return de la
*				funcion.
*
* Function: SAC
* Arguments: [0]-const short Caja de sustitucion a analizar
* Return: double* Array con la probabilidad de cada salida de la Sbox
****************************************************************/
double* SAC(unsigned short box[ROWS_PER_SBOX][COLUMNS_PER_SBOX]);

/****************************************************************
* Funcion que calcula las probabilidades del criterio BIC sobre
* las salidas de la SBox pasada como argumento.
* La salida de la funcion sera una matriz que contiene las posibles
* combinaciones de probabilidades de las salidas. En total 6.
* salida[0][1], salida[0][2], salida[0][3]
* salida[1][2], salida[1][3],
* salida[2][3].
*
* IMPORTANTE! -> La caja de sustitucion debe ser de 4x16.
* 			   4 Filas y 16 Columnas.
*
* ATENCION! -> Sera necesario liberar memoria del return de la
*				funcion.
*
* Function: BIC
* Arguments: [0]-const short Caja de sustitucion a analizar
* Return: double** Matriz de probabilidades.
****************************************************************/
double** BIC(unsigned short box[ROWS_PER_SBOX][COLUMNS_PER_SBOX]);

#endif
