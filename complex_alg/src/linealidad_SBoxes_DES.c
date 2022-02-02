
#include "../include/linealidad_SBoxes_DES.h"
#include "../include/DES_tables.h"
#include <inttypes.h>
#include <time.h>

uint8_t get_bit(uint8_t bits, uint8_t pos)
{
   return (bits >> pos) & 0x01;
}

uint8_t obtener_fila(uint8_t num){      //bits 0 y 5

    uint8_t fila = 0;

    fila |= get_bit(num, 0);
    fila <<= 1;
    fila |= get_bit(num, 5);

    return fila;
}

uint8_t obtener_columna(uint8_t num){   //bits 1, 2, 3, 4

    uint8_t columna = 0;

    columna |= get_bit(num, 1);
    columna <<= 1;
    columna |= get_bit(num, 2);
    columna <<= 1;
    columna |= get_bit(num, 3);
    columna <<= 1;
    columna |= get_bit(num, 4);

    return columna;
}


double estudio_linealidad(unsigned short box[4][16]){      //f(A ⊕ B) != f(A) ⊕ f(B)
    int j = 0, n_pruebas = 0;
    double coincidencias = 0.0;
    time_t t;
    uint8_t number1 = 0;
    uint8_t number2 = 0;
    uint8_t xor_n1_n2 = 0;
    uint8_t fila = 0;
    uint8_t columna = 0;
    uint8_t f_a = 0;
    uint8_t f_b = 0;
    uint8_t f_a_xor_b = 0;

    srand((unsigned) time(&t));

    n_pruebas = rand() % 32;    //Ya que en cada sbox hay en total 64 hacemos como minimo la mitad
    n_pruebas = (64 - n_pruebas)*10000;

    for (j = 0, coincidencias = 0.0; j < n_pruebas; j++) {
        number1 = rand() % 63;
        number2 = rand() % 63;

        xor_n1_n2 = number1 ^ number2;

        fila = obtener_fila(number1);
        columna = obtener_columna(number1);

        f_a = (uint8_t)box[fila][columna];

        fila = obtener_fila(number2);
        columna = obtener_columna(number2);

        f_b = (uint8_t)box[fila][columna];

        fila = obtener_fila(xor_n1_n2);
        columna = obtener_columna(xor_n1_n2);

        f_a_xor_b = (uint8_t)box[fila][columna];

        if(f_a_xor_b == (f_a^f_b)){
            coincidencias++;
        }
    }

    return (coincidencias/n_pruebas)*100;
}

