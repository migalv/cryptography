#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <gmp.h>

// Generar rsa keys: openssl genrsa -des3 -out private.pem 2048
//                   openssl rsa -in private.pem -outform PEM -pubout -out public.pem
// openssl rsa -text < private.pem

// Define el mejor valor de K-Ary para la exponenciacion modular
#define BEST_K_ARY 5

unsigned long int* digitsof(mpz_t num, mpz_t base, int* num_digits);
void LasVegas(mpz_t e, mpz_t d, mpz_t n, mpz_t mcd);
void modpow(mpz_t base, mpz_t exponente, mpz_t mod, unsigned long int k, mpz_t result);
void hextodec(char* hex, mpz_t num);
char* formathex(char* str);

int main (int argc,char *argv[]) {
    FILE *outfile = stdout;
    int i = 0;
    mpz_t n;
    mpz_t e;
    mpz_t d;
    mpz_t mcd;
    char* str = NULL;
    char* fstr = NULL;

    if(argc >= 2){
        if(!strcmp(argv[1], "-h") && argc == 2){
    		printf("Ejemplo:\n\n%s {-e exp_cifrado -d exp_descifrado -n modulo} \n\n", argv[0]);
            printf("[-o outfile]\n\n");
            printf("ARGUMENTO\t\t\tDESCRIPCION\n\n");
            printf("exp_cifrado\t\t\tExponente utilizado para el cifrado (Clave pública).\n");
            printf(" └> Formato (HEXADECIMAL): AB:CD:FF:09:12:34:45:67:89:09:AB:AF:CD:FA:58:39\n\n");
            printf("exp_descifrado\t\t\tExponente utilizado para el descifrado (Clave privada).\n");
            printf(" └> Formato (HEXADECIMAL): AB:CD:FF:09:12:34:45:67:89:09:AB:AF:CD:FA:58:39\n\n");
            printf("modulo\t\t\tMódulo del RSA (n = p * q)\n");
            printf(" └> Formato (HEXADECIMAL): AB:CD:FF:09:12:34:45:67:89:09:AB:AF:CD:FA:58:39\n\n");
            printf("\nOPCION\t\t\tDESCRIPCION\n");
            printf("-o outfile\t\tFichero de salida. Por defecto es stdout\n");
            return 1;
    	}else if (argc >= 7){
            for (i = 1; i < argc; i++) {
                if (!strcmp(argv[i], "-e") && argc > (i+1)){
                    mpz_init(e);
                    hextodec(argv[++i], e);
                    gmp_printf("\nEXPONENTE DE CIFRADO (CLAVE PUBLICA): %Zd\n", e);
                }else if (!strcmp(argv[i], "-d") && argc > (i+1)){
                    mpz_init(d);
                    hextodec(argv[++i], d);
                    gmp_printf("\nEXPONENTE DE DESCIFRADO (CLAVE PRIVADA): %Zd\n", d);
                }else if (!strcmp(argv[i], "-n") && argc > (i+1)){
                    mpz_init(n);
                    hextodec(argv[++i], n);
                    gmp_printf("\nMODULO (P*N): %Zd\n", n);
                }else if (!strcmp(argv[i], "-o") && argc > (i+1)){
                    outfile = fopen(argv[++i], "w");
                    if(outfile == NULL){
                        printf("Error: Al abrir el archivo de salida.\n");
                        return -1;
                    }
                    printf("Se guardarán en el archivo %s . . .\n", argv[i]);
                }else{
            		printf("Introduzca correctamente los argumentos.\n");
            		printf("Para ver la ayuda %s -h.\n", argv[0]);
            		return -1;
            	}
            }
        }
        else{
    		printf("Introduzca correctamente los argumentos.\n");
    		printf("Para ver la ayuda %s -h.\n", argv[0]);
    		return -1;
    	}
    }
    else{
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda %s -h.\n", argv[0]);
        return -1;
    }

    mpz_init(mcd);
    LasVegas(e, d, n, mcd);

    if(mpz_cmp_ui(mcd, 0) == 0){
        gmp_printf("El algoritmo de Las Vegas no ha encontrado una respuesta\n");
        mpz_clear(n);
        mpz_clear(e);
        mpz_clear(d);
        mpz_clear(mcd);
        if(outfile != stdout || outfile != NULL){
    		fclose(outfile);
    	}
        return 1;
    }

    str = mpz_get_str(NULL, 16, mcd);
    fstr = formathex(str);
    gmp_fprintf(outfile, "\n\np (DECIMAL) = %Zd\n\n", mcd);
    fprintf(outfile, "\n\np (HEXADECIMAL)= %s\n\n", fstr);
    free(str);
    free(fstr);
    str = NULL;

    mpz_div(mcd, n, mcd);
    str = mpz_get_str (NULL, 16, mcd);
    fstr = formathex(str);
    gmp_fprintf(outfile, "\n\nq (DECIMAL) = %Zd\n\n", mcd);
    fprintf(outfile, "\n\nq (HEXADECIMAL)= %s\n\n", fstr);
    free(str);
    free(fstr);

    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(mcd);

    if(outfile != stdout || outfile != NULL){
		fclose(outfile);
	}

    return 1;
}

void LasVegas(mpz_t e, mpz_t d, mpz_t n, mpz_t mcd){

    int i;
    // k y m donde e*d-1 = 2^k * m
    mpz_t k, m;
    // Numero aleatorio "a" entre (1 y n-1)
	mpz_t random;
    // x = a^m mod n
    mpz_t x;
    // Antiguo x
    mpz_t y;
    unsigned long int iaux;
    mpz_t aux;
    mpz_t mod;
    gmp_randstate_t state;

    mpz_init(k);
    mpz_init(m);
    mpz_init(random);
    mpz_init(x);
    mpz_init(y);
    mpz_init(aux);
    mpz_init(mod);

    // Calculamos k y m donde e*d-1 = 2^k * m
	mpz_mul(aux, e, d);
    mpz_sub_ui(aux, aux, 1);
	while(!mpz_sgn(mod)){
		mpz_div_ui(aux, aux, 2);
		mpz_add_ui(k, k, 1);
		mpz_mod_ui(mod, aux, 2);
	}
	mpz_set(m, aux);

    // Inicializamos la semilla de generación aleatoria de GMP
  	gmp_randinit_mt(state);
  	gmp_randseed_ui(state, (unsigned long)rand());

    // Generamos un número entre 1 y n
    mpz_sub_ui(aux, n, 1);
    mpz_urandomm(random, state, aux);
    mpz_add_ui(random, random, 1);

    mpz_set_ui(random, 3);

    // Si mcd(a, n) > 1
    mpz_gcd(mcd, random, n);
    if(mpz_cmp_ui(mcd, 1) > 1){
        mpz_clear(k);
    	mpz_clear(m);
    	mpz_clear(random);
    	mpz_clear(x);
    	mpz_clear(y);
    	mpz_clear(aux);
    	mpz_clear(mod);
        gmp_randclear (state);
        return;
    }

    // a^m mod n = x
    modpow(random, m, n, BEST_K_ARY, x);

    // Si x == 1
    if(mpz_cmp_ui(x, 1) == 0){
        mpz_clear(k);
    	mpz_clear(m);
    	mpz_clear(random);
    	mpz_clear(x);
    	mpz_clear(y);
    	mpz_clear(mod);
        gmp_randclear (state);
        // Return 0 (No responde)
        mpz_set_ui(mcd, 0);
        return;
    }
    // Si x == n-1
    mpz_sub_ui(aux, n, 1);
    if(mpz_cmp(x, aux) == 0){
        mpz_clear(k);
    	mpz_clear(m);
    	mpz_clear(random);
    	mpz_clear(x);
    	mpz_clear(y);
    	mpz_clear(mod);
        gmp_randclear (state);
        // Return 0 (No responde)
        mpz_set_ui(mcd, 0);
        return;
    }

    // Bucle de 1 a k-1
    mpz_sub_ui(k, k, 1);
    iaux = mpz_get_ui(k);
    for(i = 1; i < iaux; i++){
        // Guardamos el antiguo valor de x en y
        mpz_set(y, x);
        // Hacemos x^2 mod n = x
        mpz_set_ui(aux, 2);
        modpow(x, aux, n, BEST_K_ARY, x);
        // Si x == 1
        if(mpz_cmp_ui(x, 1) == 0){

            // Return mcd(y+1, n)
            mpz_add_ui(y, y, 1);
            mpz_gcd(mcd, y, n);

            // Liberar memoria
            mpz_clear(k);
        	mpz_clear(m);
        	mpz_clear(random);
        	mpz_clear(x);
        	mpz_clear(y);
        	mpz_clear(aux);
        	mpz_clear(mod);
            gmp_randclear (state);

            return;
        }
        // Si x == -1
        if(mpz_cmp_si(x, -1) == 0){

            // Liberar memoria
            mpz_clear(k);
        	mpz_clear(m);
        	mpz_clear(random);
        	mpz_clear(x);
        	mpz_clear(y);
        	mpz_clear(aux);
        	mpz_clear(mod);
            gmp_randclear (state);

            // Return 0 (No responde)
            mpz_set_ui(mcd, 0);
            return;
        }
    }

    // Return mcd(x+1, n)
    mpz_add_ui(x, x, 1);
    mpz_gcd(mcd, x, n);

    // Liberar memoria
    mpz_clear(k);
    mpz_clear(m);
    mpz_clear(random);
    mpz_clear(x);
    mpz_clear(y);
    mpz_clear(aux);
    mpz_clear(mod);
    gmp_randclear (state);

    return;
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

void hextodec(char* hex, mpz_t num){

    int size = 0;
    char* token = NULL;
    char* str = NULL;

    /* Recuperamos el exponente de cifrado */
    // Los dos primeros caracteres más \0
    size = 2+1;
    str = (char*)malloc(sizeof(char)*size);
    str[0] = '\0';
    token = strtok(hex, ":");
    while (token != NULL) {
        strncat(str, token, 2);
        size += 2;
        str = (char *)realloc(str, sizeof(char) * size);
        token = strtok(NULL, ":"); // Get the next token
    }
    str[size-3] = '\0';
    mpz_set_str(num, str, 16);

    free(str);

    return;
}

char* formathex(char* str){
    int i = 0;
    int size = 0;
    int new_size = 0;
    char* formatted = NULL;
    int formattedcharacters = 0;

    size = strlen(str);
    new_size = size + ceil(size/2.0) - 1;
    formatted = (char*)malloc(sizeof(char)*new_size+1);
    formatted[0] = '\0';

    if(formatted == NULL){
        printf("Error en la reserva de memoria de formatted en formathex\n");
        return NULL;
    }

    if(size % 2){
        formatted[0] = '0';
        formatted[1] = '\0';
        formattedcharacters++;
        new_size++;
        formatted = (char *)realloc(formatted, sizeof(char) * new_size+1);
        if(formatted == NULL){
            printf("Error en realloc de formatted en formathex\n");
            return NULL;
        }
    }

    for(i = 0; i < size; i++){
        strncat(formatted, str+i, 1);
        formattedcharacters++;
        if(formattedcharacters % 2 == 0){
            if(i != size-1){
                strncat(formatted, ":", 1);
            }
        }
    }

    return formatted;
}
