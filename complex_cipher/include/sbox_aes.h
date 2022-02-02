/*
 * Modulo encargado de crear
 * las sboxes del aes
 *
 * Modulo: create sboxes AES.c
 * Autor: Alberto Garcia Redero
 */

#ifndef CREATE_BOXES_AES_H
#define CREATE_BOXES_AES_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

/****************************************************************
 * Funcion que calcula el inverso de los números a y b
 *
 * Function: euclides
 * Arguments: [0]-uint16_t* --> array con la salida, contiene:
 *                  En [0] maximo común divisor
 *                  En [1] inverso de a
 *                  En [2] inverso de b
 *            [1]-uint16_t --> Primer numero
 *            [2]-uint16_t --> Segundo numero
 * Return: Nada, el resultado se guarda en el primer argumento
 ****************************************************************/
void euclides(uint16_t a, uint16_t b, uint16_t* d, uint16_t* s, uint16_t* t);

/****************************************************************
 * Realiza la transformación afín para la tabla de sustitución
 * directa de n
 *
 * Function: transformacion_afin_directa
 * Arguments: [0]-uint16_t --> numero del que creamos su equivalente
 *
 * Return: uint16_t, numero transformado correspondiente en la tabla
 ****************************************************************/
uint16_t transformacion_afin_directa(uint16_t n);

/****************************************************************
 * Realiza la transformación afín para la tabla de sustitución
 * inversa de n
 *
 * Function: transformacion_afin_directa
 * Arguments: [0]-uint16_t --> numero del que creamos su equivalente
 *
 * Return: uint16_t, numero transformado correspondiente en la tabla
 ****************************************************************/
uint16_t transformacion_afin_inversa(uint16_t n);

/****************************************************************
 * Funcion que permite printear los bytes de una variable en
 * hexadecimal.
 *
 * ATENCION! -> Es importante saber que numero de bloques no
 * 			   sobrepase la memoria reservada de la variable.
 *
 * Function: print_hexa
 * Arguments: [0]-void* --> Stream de bytes
 *			  [1]-int   --> Número de bloques a imprimir
 *			  [2]-int   --> Tamaño de los bloques en bytes
 *            [3]-FILE* --> En caso de querer imprimir en un fichero
 *                          es este, en caso contrario NULL;
 * Return: void* Array de bytes que representan cada letra.
 ****************************************************************/
void print_hexa(void* bytes, int nbloques, int tam_bloque, FILE * f_out);

#endif
