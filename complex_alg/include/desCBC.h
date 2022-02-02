/**
 * Modulo encargado de implementar las funciones del cifrado
 * y descifrado DES
 *
 * Modulo: desCBC.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 05/11/2018
 */

#ifndef DES_H
#define DES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>

#include "DES_tables.h"

#define PADDING 0
#define MAX_TEXTO 504+1 // Multiplo de 64
// Tamaño de la clave en bytes
#define TAM_CLAVE_BYTES 8
// Tamaño de la clave en bits
#define TAM_CLAVE_BITS 64
// Numero de bits en un byte
#define BITS_IN_BYTE 8

typedef enum{
	CBC,
	ECB,
	CFB,
	OFB
}MODO;

/****************************************************************
 * Función de cifrado del DES. Se le pasa un stream de datos
 * binarios y ésta los cifra según DES. Tiene 2 diferentes
 * modos de operacion: ECB, CBC.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 * ATENCION! -> Es importante que el stream esté en bloques de 64
 *				bits además de paddeado.
 *
 * Function: cifrar_des
 * Arguments: [0]-uint64_t* stream--> Stream de datos. Datos binarios
 *			  Deben ser bloques de 64bits. Con padding incluido.
 *			  [1]-int nbloques --> Número de bloques de 64Bits
 * 			  que tiene el stream de datos.
 *			  [2]-uint64_t clave--> Clave de 64bits utilizada para
 *			  cifrar. Contiene bits de paridad (8, 16, 24, 32, 40, 48,
 *			  56, 64). Se comprobará si la paridad es correcta.
 *		      [3]-uint64_t IV--> Vector de inicializacion. utilizado
 * 			  para el modo CBC. Es de 64bits.
 *			  [4]-MODO modo--> Modo de cifrado del DES (ECB o CBC)
 * Return: uint64_t* Stream de datos cifrado.
 ****************************************************************/
uint64_t* cifrar_des(uint64_t* stream, int nbytes, uint64_t* claves, uint64_t iv, MODO modo);

uint64_t* descifrar_des(uint64_t* stream, int nbytes, uint64_t* claves, uint64_t iv, MODO modo);
/****************************************************************
 * Función que genera las 16 claves de 56 bits utilizadas en las
 * diferentes rondas del DES.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: generar_claves
 * Arguments: [0]-uint64_t clave--> Clave de 64bits utilizada para
 *			  cifrar. Contiene bits de paridad (8, 16, 24, 32, 40, 48,
 *			  56, 64). Se comprobará si la paridad es correcta.
 ****************************************************************/
uint64_t* generar_claves(uint64_t clave);

/****************************************************************
 * Función que realiza una ronda del DES. Se le pasa el de 64 Bits
 * que se está cifrando y la clave correspondiente a la ronda.
 * Este generara la clave de 48bits con PC-2 y utilizara la funcion
 * feistel.
 *
 * Function: ronda
 *
 * Arguments: [0]-uint64_t bloque64--> Bloque de 64bits
 *			  [1]-uint64_t clave56--> Clave de 56 bits correspondiente
 *			  a la ronda.
 * Return: uint64_t Bloque de 64 bits.
 ****************************************************************/
uint64_t ronda(uint64_t bloque64, uint64_t clave56);

/****************************************************************
 * Funcion Feistel (F). Funcion que se encarga de realizar la
 * confusion de la clave en el DES. Utiliza las SBOX para ello.
 * Recibe los 32bits de menor peso de los datos y la clave (48bits)
 * y devuelve 32 bits.
 *
 * Function: feistel
 * Arguments: [0]-uint32_t --> 32bits de menor peso de los datos
 * 			  [1]-uint64_t --> Clave de 48bits, tras PC-2
 * Return: uint32_t 32 bits con la clave confundida.
 ****************************************************************/
uint32_t feistel(uint32_t r0, uint64_t clave);

/****************************************************************
 * Función que realiza la sustituciones de las Sbox. Utiliza las
 * tablas de sustitución de DES_tables.h. Recibe 48bits de Datos
 * y los transforma en 32 bits.
 *
 * Function: sbox
 * Arguments: [0]-uint64_t --> Recibe 48 bits de datos
 * Return: uint32_t 32 bits de datos.
 ****************************************************************/
uint32_t sbox(uint64_t xor);

/****************************************************************
 * Funcion que realiza las permutaciones del DES. Dado un bloque,
 * de tamaño entre 1 bit y 64 bits realiza las permutaciones de
 * la permutacion pasada como parametro. Puede realizar permutaciones
 * conjutamente con reducciones o con expansiones. Retorna un void*
 * por lo que es posible darle el tipo más deseado al usarla.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: permutar
 * Arguments: [0]-uint64_t* --> Bloque de 64 bits a permutar
 * 			  [1]-int --> Numero de bits del bloque a permutar
 * 			  [2]-const unsigned short* --> Array que contiene las
 *			  permutaciones a realizar sobre el bloque.
 *			  [3]-int --> Tamaño de la permutacion. (Indica el
 * 			  numero de bits de salida de la función.)
 * Return: void* Bits permutados. Puede ser tratado de la forma
 * 		   mas conveniente.
 ****************************************************************/
void* permutar(uint64_t* bloque, int nbits, const unsigned short* permutacion,	int p_size);

/****************************************************************
 * Funcion que genera una clave aleatoria de 64 bits. La clave
 * tendra 56 bits de datos mas 8 bits de paridad impar. Los bits
 * de paridad ocuparan las posiciones 8, 16, 24, 32, 40, 48, 56
 * y 64.
 *
 * Function: generar_clave
 * Arguments: NONE
 * Return: uint64_t Clave de 64bits.
 ****************************************************************/
uint64_t generar_clave();

/****************************************************************
 * Funcion que genera un vector de inicializacion aleatorio de 64
 * bits. Éste se basa en la clave para generar más incertidumbre.
 *
 * Function: generar_iv
 * Arguments: [0]-uint64_t --> Clave utilizada para generar el iv
 * Return: uint64_t Vector de inicializacion.
 ****************************************************************/
uint64_t generar_iv(uint64_t clave);

/****************************************************************
 * Funcion que transforma un array de bytes en bloques de 64 Bits
 * y que realiza un padding en caso necesario. Se le pasa la
 * longitud en bytes como segundo argumento. Y la nueva longitud
 * será devuelta por el mismo parametro.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: strtob
 * Arguments: [0]-void* --> Bytes a transformar
 *			  [1]-int* --> Numero de bytes antes y despues de
 *			  hacer el padding.
 * Return: uint64_t* Array de bloques de 64 bits.
 ****************************************************************/
uint64_t* tandpad(void* bytes, int* nbytes);

/****************************************************************
 * Funcion que permite printear los bytes de una variable en
 * hexadecimal.
 *
 * ATENCION! -> Es importante saber que numero de bloques no
 * 			   sobrepase la memoria reservada de la variable.
 *
 * Function: print_hexa
 * Arguments: [0]-void* --> Stream de bytes
 *			  [1]-int --> Número de bloques a imprimir
 *			  [2]-int --> Tamaño de los bloques en bytes
 * Return: void* Array de bytes que representan cada letra.
 ****************************************************************/
void print_hexa(void* bytes, int nbloques, int tam_bloque);

/****************************************************************
 * Funcion que permite printear por stdout bits de un stream.
 *
 * Function: print_bits
 * Arguments: [0]-size_t* --> Sizeof(Del tipo de la variable)
 *			  [1]-int --> Stream a convertir a bits
 * Return: NONE
 ****************************************************************/
void print_bits(size_t const size, void const * const ptr);

/****************************************************************
 * Funcion que comprueba si la paridad impar de la clave se cumple
 * Esto permite saber si la clave fue modificada o se ha recibido
 * mal.
 *
 * Function: check_parity
 * Arguments: [0]-uint64_t Clave de 64 bits con 8 bits de paridad
 *			  impar, en las posiciones (8, 16 , 24, 32, 40, 48, 56, 64)
 * Return: int Retona 0 si no coincide 1 si todo está OK.
 ****************************************************************/
int check_parity(uint64_t clave);

void set_seed(unsigned int s);

uint64_t* tripleDES(uint64_t* stream, int nbytes, uint64_t** claves, uint64_t iv,
	MODO modo, int flag_cifrar);

#endif
