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

#include "../includes/modular.h"

int modulo = 0;

/****************************************************************
 * Realiza la multiplicacion entre dos enteros en forma modular
 * con modulo "modulo". Entero1 * Entero2
 *
 * Function: mul_mod
 * Arguments: [0]-int --> Primer entero a multiplicar
 *            [1]-int --> Cociente por el que multiplicar
 * Return: int Resultado de la multiplicacion en forma modular
 ****************************************************************/
int mul_mod(int entero1, int entero2){
    // Resultado de la operación
    int resultado = 0;

    resultado = entero1 * entero2;
    int modulo = get_modulo();

    if(resultado >= modulo){
        resultado = resultado % modulo;
    }else if(resultado < 0){
        resultado *= -1;
        resultado = modulo - (resultado % modulo);
    }

    return resultado;
}

/****************************************************************
 * Realiza la resta entre dos enteros en forma modular
 * con modulo "modulo". Entero1 - Entero2
 *
 * Function: res_mod
 * Arguments: [0]-int --> Entero1
 *            [1]-int --> Entero2
 * Return: int Resultado de la resta en forma modular
 ****************************************************************/
int res_mod(int entero1, int entero2){
    // Resultado de la operación
    int resultado = 0;
    int modulo = get_modulo();

    resultado = entero1 - entero2;

    if(resultado >= modulo){
        resultado = resultado % modulo;
    }else if(resultado < 0){
        resultado *= -1;
        resultado = modulo - (resultado % modulo);
    }

    return resultado;
}

/****************************************************************
 * Realiza la suma entre dos enteros en forma modular
 * con modulo "modulo". Entero1 + Entero2
 *
 * Function: sum_mod
 * Arguments: [0]-int --> Entero1
 *            [1]-int --> Entero2
 * Return: int Resultado de la suma en forma modular
 ****************************************************************/
int sum_mod(int entero1, int entero2){
    // Resultado de la operación
    int resultado = 0;
    int modulo = get_modulo();

    resultado = entero1 + entero2;

    if(resultado >= modulo){
        resultado = resultado % modulo;
    }else if(resultado < 0){
        resultado *= -1;
        resultado = modulo - (resultado % modulo);
    }

    return resultado;
}

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
int div_mod(int entero1, int entero2){

    // Resultado de la operación
    int resultado = 0;
    int modulo = get_modulo();
    int mcd = 0; int inverso1 = 0; int inverso2 = 0;

    euclides(entero2, modulo, &mcd, &inverso1, &inverso2);

    // entero 2 no tiene inverso multiplicativo
    if(mcd != 1){
        return -1;
    }
    else{
        resultado = entero1 * inverso1;
    }

    if(resultado >= modulo){
        resultado = resultado % modulo;
    }else if(resultado < 0){
        resultado *= -1;
        resultado = modulo - (resultado % modulo);
    }

    return resultado;
}

/****************************************************************
 * Un seter del modulo, para realizar las operaciones modulares.
 *
 * Function: set_modulo
 * Arguments: [0]-int --> modulo
 * Return: VOID
 ****************************************************************/
void set_modulo(int mod){
    modulo = mod;
    return;
}

/****************************************************************
 * Te devuelve el modulo que se seteo con la funcion set
 *
 * Function: get_modulo
 * Arguments: NONE
 * Return: int modulo
 ****************************************************************/
int get_modulo(){
    return modulo;
}