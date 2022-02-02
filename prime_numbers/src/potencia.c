/**
 * Módulo encargado de realizar la potenciación modular
 *
 * Modulo: potencia.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 16/12/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <math.h>
#include <time.h>

unsigned long int* digitsof(mpz_t num, mpz_t base, int* num_digits);
void modpow(mpz_t base, mpz_t exponente, mpz_t mod, unsigned long int k, mpz_t result);

int main(int argc,char *argv[]) {

    // Base del exponente (a)
    mpz_t base;
    // Exponente (b)
    mpz_t exponente;
    // Modulo (m)
    mpz_t mod;
    // Resultado de la potenciación
    mpz_t result;
    // K-ary Value (Por defecto k = 5)
    unsigned long int k = 5;
    // Variables para calcular los tiempos de ejecucion
    clock_t begin, end;
    double time_spent;

    // Mostrar ayuda
    if(argc < 4){
        printf("Ejemplo:\n\n%s {base exponente modulo}", argv[0]);
        printf("[-k valor_k]\n\n");
        printf("ARGUMENTO\t\t\tDESCRIPCION\n");
        printf("base\t\t\tEl valor de la base a ser elevada\n");
        printf("exponente\t\t\tEl valor del exponente.\n");
        printf("modulo\t\t\tEl valor del Modulo.\n");
        printf("\nOPCION\t\t\tDESCRIPCION\n");
        printf("-k infile\t\tEl valor de k para el metodo K-Ary (Por defecto = 5) [Max = 20]\n");
        printf("\n  Ejemplos de uso:\n\n");
        printf("\t%s 1198976451239874 964745813 9874789645132 \n", argv[0]);
        printf("\t%s 1198976451239874 964745813 9874789645132 -k 3\n", argv[0]);
        return 1;
    }

    // Se ha introducido la opción -k
    if(argc == 6 && !strcmp(argv[4], "-k")){
        k = atoi(argv[5]);
        if(k > 20){
            printf("El valor de k no puede ser mayor que 20.\n");
            return -1;
        }
    }

    mpz_init_set_str(base, argv[1], 10);
    mpz_init_set_str(exponente, argv[2], 10);
    mpz_init_set_str(mod, argv[3], 10);
    mpz_init(result);

    begin = clock();
    mpz_powm(result, base, exponente, mod);
    end = clock();

    printf("Tiempos de ejecucion:\n");
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\tFunción de GMP (gmp_powm): %lfs\n", time_spent);

    begin = clock();
    modpow(base, exponente, mod, k, result);
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\tFunción propia (modpow): %lfs\n", time_spent);

    gmp_printf("%Zd ^ %Zd mod %Zd = %Zd\n", base, exponente, mod, result);

    // Liberamos memoria
    mpz_clear(base);
    mpz_clear(exponente);
    mpz_clear(mod);
    mpz_clear(result);

    return 1;
}

void modpow(mpz_t base, mpz_t exponente, mpz_t mod, unsigned long int k, mpz_t result){
    mpz_t m_base;
    mpz_t* tabla;
    unsigned long int* digits = NULL;
    int num_digits = 0;
    unsigned long int im_base = 0;
    int i = 0, j = 0;

    if(k > 20){
        printf("El valor de k no puede ser mayor que 20.\n");
        return;
    }

    // Inicializamos la base 'm' a 2
    mpz_init_set_ui(m_base, 2);
    // Shift left (k-1) veces.
    mpz_mul_2exp(m_base, m_base, k-1);

    // Reservamos memoria para la tabla de exponentes
    im_base = mpz_get_ui(m_base);
    tabla = (mpz_t*)malloc(im_base*sizeof(mpz_t));
    // Inicializamos todos los valores de la tabla a 1
    for(i = 0; i < im_base; i++){
        mpz_init_set_ui(tabla[i], 1);
    }
    // Precalculamos la tabla de exponentes
    for(i = 1; i < im_base; i++){
        mpz_mul(tabla[i], tabla[i-1], base);
        mpz_mod(tabla[i], tabla[i], mod);
    }

    // Inicializamos el resultado a 1
    mpz_set_ui(result, 1);
    // Recuperamos los digitos del exponente
    digits = digitsof(exponente, m_base, &num_digits);
    // Bucle que realiza los calculos sobre cada digito del exponente
    for(i = num_digits-1; i >= 0; i--){
        for(j = 0; j < k; j++){
            mpz_mul(result, result, result);
            mpz_mod(result, result, mod);
        }
        if(digits[i]){
            mpz_mul(result, result, tabla[digits[i]]);
            mpz_mod(result, result, mod);
        }
    }

    // Liberamos memoria
    free(digits);
    mpz_clear(m_base);
    for(i = 0; i < im_base; i++){
    	mpz_clear(tabla[i]);
    }
    free(tabla);
}

unsigned long int* digitsof(mpz_t num, mpz_t base, int* num_digits){

    // Array con los digitos de num colocados alreves
     unsigned long int* digits = NULL;
     // Variable auxiliar para la operaciones con gmp
     mpz_t aux;
     // Variable auxiliar para recuperar el varlo de num antes de la llamada
     mpz_t aux_num;

     mpz_init(aux);
     mpz_init(aux_num);

     // Guardamos el valor de num
     mpz_set(aux_num, num);

     *num_digits = 0;
     // Reservamos memoria para el primer digito
     digits = (unsigned long int*)malloc(sizeof(unsigned long int));
     digits[0] = 0;
     // Bucle que va guardando los digitos en el array
     while(mpz_cmp_ui(num, 0) != 0){
         mpz_mod(aux, num, base);
         digits[*num_digits] = mpz_get_ui(aux);
         mpz_div(num, num, base);
         *num_digits += 1;
         digits = (unsigned long int*)realloc(digits, sizeof(unsigned long int)*(*num_digits+1));
     }

     // Recuperamos el antiguo valor de num
     mpz_set(num, aux_num);
     mpz_clear(aux);
     mpz_clear(aux_num);

     return digits;
 }
