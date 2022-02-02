/** 
 * Modulo encargado de las operaciones con matrices
 *
 * Modulo: matriz.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 15/10/2018 
 */

#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdio.h>
#include <stdlib.h>
#include "modular.h"

/****************************************************************
 * Lee una matriz cuadrada de un archivo, reserva memoria para 
 * ella y la devuelve como int**. El archivo debe estar 
 * formateado de la siguiente manera :
 *
 *                          |%d %d %d|
 *                          |%d %d %d|
 *                          |%d %d %d|
 *
 * Function: lee_matriz
 * Arguments: [0]-char* --> Nombre del archivo a leer
 *            [1]-int --> Tamanio de la matriz cuadrada
 * Return: int** La matriz cuadrada formateada
 ****************************************************************/
int determinante(int **matriz, int tamanio);

/****************************************************************
 * Calcula el determinante en forma modular de una matriz cuadrada.
 *
 * Function: determinante
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 * Return: int el determinante de la matriz
 ****************************************************************/
int** lee_matriz(char* filename, int tamanio);

/****************************************************************
 * Calcula la matriz menor de una matriz cuadrada, dada una fila
 * y una columna. Ejemplo:
 *              matriz      ==> matriz menor fila = 1 // col = 1
 *             %d %d %d           X  X  X
 *             %d %d %d           X %d %d
 *             %d %d %d           X %d %d
 *
 * Function: matriz_menor
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 *            [2]-int --> Fila a eliminar
 *            [3]-int --> Columna a eliminar
 * Return: int** La matriz menor, de tamanio = tamanio-1
 ****************************************************************/
int** matriz_menor(int** matriz, int tamanio, int fila, int col);


/****************************************************************
 * Calcula la matriz cofactor de una matriz cuadrada
 *
 * Function: matriz_cofactor
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 * Return: int** La matriz cofactor en forma modular
 ****************************************************************/
int** matriz_cofactor(int** matriz, int tamanio);


/****************************************************************
 * Calcula en forma modular la matriz inversa, de una matriz 
 * cuadrada.
 *
 * Function: matriz_inversa
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 *            [2]-int --> Determinante de la matriz cuadrada
 * Return: int** La matriz inversa en forma modular
 ****************************************************************/
int** matriz_inversa(int** matriz, int tamanio, int determinante);

/****************************************************************
 * Printea una matriz por pantalla
 *
 * Function: print_matriz
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Numero de filas de la matriz
 *            [2]-int --> Numero de columnas de la matriz
 * Return: VOID
 ****************************************************************/
void print_matriz(int** matriz, int columnas, int filas);

/****************************************************************
 * Dada una permutacion, te genera una matriz identidad que la
 * representa.
 *
 * Function: matriz_identidad
 * Arguments: [0]-int* --> Permutacion
 *            [1]-int --> Tamanio de la permutacion
 * Return: int** La matriz identidad de la permutacion
 ****************************************************************/
 int** matriz_identidad(int* permutacion, int num);

#endif