/* 
 * Modulo encargado de parsear el texto a nuestro alfabeto
 * para el cifrado
 *
 * Modulo: parser.c
 * Autor: Miguel Alvarez Lesmes
 */

#ifndef PARSER_H
#define PARSER_H

#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************
 * Parsea los caracteres de un archivo y los guarda en otro. Las 
 * letras como enie, y vocales con acentos las pasa a su respectiva
 * letra en mayusculas sin el acento. Ademas pasa todas las 
 * minusculas a mayusculas y elimina los espacios en blanco.
 * El resultado es un texto todo en mayusculas sin acentos, sin 
 * enies y sin espacios en blanco.
 *
 * Function: limpiar_archivo
 * Arguments: [0]-char* --> Nombre del archivo a limpiar
 *            [1]-char* --> Nombre del archivo de salida
 * Return: VOID 
 ****************************************************************/
void limpiar_archivo(char* infilename, char* outfilename);

#endif