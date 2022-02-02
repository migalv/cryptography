/*
 * Modulo encargado de parsear el texto a nuestro alfabeto
 * para el cifrado
 *
 * Modulo: parser.c
 * Autor: Miguel Alvarez Lesmes
 */

#ifndef LINEALIDAD_DES_H
#define LINEALIDAD_DES_H

#include <stdio.h>
#include <stdlib.h>


/****************************************************************
 * Funcion que realiza un estudio de linealidad de las sBoxes
 * del DES. Muestra por pantalla el resultado
 *
 * Function: estudio_linealidad
 * Arguments:
 * Return: VOID
 ****************************************************************/
double estudio_linealidad(unsigned short box[4][16]);

#endif
