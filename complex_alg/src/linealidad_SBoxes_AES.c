
#include "../include/linealidad_SBoxes_AES.h"
#include "../include/AES_tables.h"
#include <inttypes.h>
#include <time.h>

uint8_t obtener_fila(uint8_t num){      //bits 0 y 5

    uint8_t fila = 0;

    fila = num & 240;
    fila >>= 4;

    return fila;
}

uint8_t obtener_columna(uint8_t num){      //bits 0 y 5

    uint8_t fila = 0;

    fila = num & 15;

    return fila;
}

void estudio_linealidad(int estudiar_directa){      //f(A ⊕ B) != f(A) ⊕ f(B)

    time_t t;
    int i = 0, n_pruebas = 0;
    uint8_t number1 = 0;
    uint8_t number2 = 0;
    uint8_t xor_n1_n2 = 0;
    uint8_t fila = 0;
    uint8_t columna = 0;
    double coincidencias = 0.0;
    const char* c_f_a = NULL;
    uint8_t f_a = 0;
    const char* c_f_b = NULL;
    uint8_t f_b = 0;
    const char* c_f_a_xor_b = NULL;
    uint8_t f_a_xor_b = 0;

    srand((unsigned) time(&t));

    if(estudiar_directa){
        //Tabla directa

        n_pruebas = rand() % 130;    //Ya que en cada sbox hay en total 256  y comienza en 0.
        n_pruebas = (255 - n_pruebas)*10000;
        for (i = 0, coincidencias = 0.0; i < n_pruebas; i++) {

            number1 = rand() % 255;
            number2 = rand() % 255;
            xor_n1_n2 = number1 ^ number2;

            fila = obtener_fila(number1);
            columna = obtener_columna(number1);

            c_f_a = DIRECT_SBOX[fila][columna];
            f_a = strtol(c_f_a, NULL, 16);

            fila = obtener_fila(number2);
            columna = obtener_columna(number2);

            c_f_b = DIRECT_SBOX[fila][columna];
            f_b = strtol(c_f_b, NULL, 16);

            fila = obtener_fila(xor_n1_n2);
            columna = obtener_columna(xor_n1_n2);

            c_f_a_xor_b = DIRECT_SBOX[fila][columna];
            f_a_xor_b = strtol(c_f_a_xor_b, NULL, 16);

            if(f_a_xor_b == (f_a^f_b)){
                coincidencias++;
            }
        }
        printf("\nCaja directa:\n");
    } else{
        //Tabla indirecta

        n_pruebas = rand() % 130;    //Ya que en cada sbox hay en total 256  y comienza en 0.
        n_pruebas = (255 - n_pruebas)*10000;
        for (i = 0, coincidencias = 0.0; i < n_pruebas; i++) {

            number1 = rand() % 255;
            number2 = rand() % 255;
            xor_n1_n2 = number1 ^ number2;

            fila = obtener_fila(number1);
            columna = obtener_columna(number1);

            c_f_a = INVERSE_SBOX[fila][columna];
            f_a = strtol(c_f_a, NULL, 16);

            fila = obtener_fila(number2);
            columna = obtener_columna(number2);

            c_f_b = INVERSE_SBOX[fila][columna];
            f_b = strtol(c_f_b, NULL, 16);

            fila = obtener_fila(xor_n1_n2);
            columna = obtener_columna(xor_n1_n2);

            c_f_a_xor_b = INVERSE_SBOX[fila][columna];
            f_a_xor_b = strtol(c_f_a_xor_b, NULL, 16);

            if(f_a_xor_b == (f_a^f_b)){
                coincidencias++;
            }
        }
        printf("\nCaja inversa:\n");
    }

    printf("Pocentaje de coincidencia f(A ⊕ B) == f(A) ⊕ f(B):\n-->%.3lf%%\n", (coincidencias/n_pruebas)*100);
}

