/*
 * Modulo encargado de parsear el texto a nuestro alfabeto
 * para el cifrado
 *
 * Modulo: parser.c
 * Autor: Miguel Alvarez Lesmes
 */

#ifndef LINEALIDAD_AES_H
#define LINEALIDAD_AES_H

#include <stdio.h>
#include <stdlib.h>


/****************************************************************
 * Funcion que realiza un estudio de linealidad de las sBoxes
 * del DES. Muestra por pantalla el resultado
 *
 * Function: estudio_linealidad
 * Arguments: [0]-bool --> flag para saber si estudiar la tabla
 *                         directa(true) o la indirecta(false).
 * Return: VOID
 ****************************************************************/
void estudio_linealidad(int estudiar_directa);

#endif
