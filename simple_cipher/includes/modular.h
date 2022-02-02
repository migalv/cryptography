/** 
 * Modulo encargado de las operaciones modulares 
 *
 * Las operaciones se hace segun el modulo guardado 
 * en la variable global "modulo".
 *
 * Modulo: modular.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 15/10/2018 
 */
#ifndef MODULAR_H
#define MODULAR_H

#include "euclides.h"

/****************************************************************
 * Realiza la multiplicacion entre dos enteros en forma modular
 * con modulo "modulo". Entero1 * Entero2
 *
 * Function: mul_mod
 * Arguments: [0]-int --> Primer entero a multiplicar
 *            [1]-int --> Cociente por el que multiplicar
 * Return: int Resultado de la multiplicacion en forma modular
 ****************************************************************/
int mul_mod(int entero1, int entero2);

/****************************************************************
 * Realiza la suma entre dos enteros en forma modular
 * con modulo "modulo". Entero1 + Entero2
 *
 * Function: sum_mod
 * Arguments: [0]-int --> Entero1
 *            [1]-int --> Entero2
 * Return: int Resultado de la suma en forma modular
 ****************************************************************/
int sum_mod(int entero1, int entero2);

/****************************************************************
 * Realiza la resta entre dos enteros en forma modular
 * con modulo "modulo". Entero1 - Entero2
 *
 * Function: res_mod
 * Arguments: [0]-int --> Entero1
 *            [1]-int --> Entero2
 * Return: int Resultado de la resta en forma modular
 ****************************************************************/
int res_mod(int entero1, int entero2);

/****************************************************************
 * Realiza la division entre dos enteros en forma modular
 * con modulo "modulo". Entero1 / Entero2. Calcula el inverso de
 * Entero2 y lo multiplica por Entero1. Utiliza Euclides extendido
 * para calcular el inverso
 *
 * Function: res_mod
 * Arguments: [0]-int --> Entero1
 *            [1]-int --> Entero2
 * Return: int Resultado de la division en forma modular
 ****************************************************************/
int div_mod(int entero1, int entero2);

/****************************************************************
 * Un seter del modulo, para realizar las operaciones modulares.
 *
 * Function: set_modulo
 * Arguments: [0]-int --> modulo
 * Return: VOID
 ****************************************************************/
int get_modulo();

/****************************************************************
 * Te devuelve el modulo que se seteo con la funcion set
 *
 * Function: get_modulo
 * Arguments: NONE
 * Return: int modulo
 ****************************************************************/
void set_modulo(int mod);

#endif