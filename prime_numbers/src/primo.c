#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <gmp.h>

// Define el mejor valor de K-Ary para la exponenciacion modular
#define BEST_K_ARY 5
#define NUM_PRIMES 2000

unsigned long int* digitsof(mpz_t num, mpz_t base, int* num_digits);
void modpow(mpz_t base, mpz_t exponente, mpz_t mod, unsigned long int k, mpz_t result);
char* randnum(int bits);
int divide_by_primes(mpz_t number, int* primes, int num_primes);
int Miller_Rabin(mpz_t number, int tests, int* primes, int num_primes);

int main (int argc,char *argv[]) {
	FILE *outfile = stdout;
	FILE *primesfile;
    int bits = 0;
    int i = 0;
    int tests;
	int temp;
    mpz_t prime;
	int* primes = NULL;
	int is_prime;
    char *number;
    clock_t begin, end;
    double time_spent_GMP;
	double time_spent_M;

	if(argc == 2){
		if(!strcmp(argv[1], "-h")){
			printf("Ejemplo:\n\n%s {-b bits -t tests}", argv[0]);
			printf("[-o outfile]\n\n");
			printf("ARGUMENTO\t\t\tDESCRIPCION\n");
			printf("-b\t\t\tNúmero máximo de bits significativos del número generado.\n");
			printf("-t\t\t\tnúmero de veces que se pasa el test.\n");
			printf("\nOPCION\t\t\tDESCRIPCION\n");
			printf("-o outfile\t\tFichero de salida. Por defecto es stdout\n");
			printf("\n  Ejemplos de uso:\n\n");
			printf("\t%s -b 4096 -t 10 \n", argv[0]);
			printf("\t%s -b 4096 -t 10 -o salida.txt\n", argv[0]);
			printf("\n\nATENCION SERÁ NECESARIO EJECUTAR EL EJECUTABLE DESDE EL DIRECTORIO DONDE SE ENCUENTRA EL ARCHIVO primes.dat \n\n");
			return 1;
		}else{
			printf("Introduzca correctamente los argumentos.\n");
			printf("Para ver la ayuda %s -h.\n", argv[0]);
			return -1;
		}
	}
	if(argc < 5){
		printf("Introduzca correctamente los argumentos.\n");
		printf("Para ver la ayuda %s -h.\n", argv[0]);
		return -1;
	}

    for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-b")){
			if((i + 1) < argc){
				bits = atoi(argv[i+1]);
				i++;
			}else{
				printf("Introduzca correctamente los argumentos.\n");
	            printf("Para ver la ayuda %s -h.\n", argv[0]);
	            return -1;
			}
    	}else if (!strcmp(argv[i], "-t")) {
			if((i + 1) < argc){
				tests = atoi(argv[i+1]);
				i++;
			}
			else{
				printf("Introduzca correctamente los argumentos.\n");
	            printf("Para ver la ayuda %s -h.\n", argv[0]);
	            return -1;
			}
    	}else if (!strcmp(argv[i], "-o")) {
			if((i + 1) < argc){
				 outfile = fopen(argv[i+1], "w");
				 i++;
			}
			else{
				printf("Introduzca correctamente los argumentos.\n");
	            printf("Para ver la ayuda %s -h.\n", argv[0]);
	            return -1;
			}
    	}else{
			printf("Introduzca correctamente los argumentos.\n");
			printf("Para ver la ayuda %s -h.\n", argv[0]);
			return -1;
		}
    }

	// Abrimos el archivo que contiene los NUM_PRIMES primeros primos
	primesfile = fopen("primes.dat", "r");
	if(primesfile == NULL){
        printf("Error: Al abrir el archivo de primos (primes.dat)\n");
        return -1;
    }
	primes = (int*)malloc(sizeof(int)*NUM_PRIMES);
	// Precargamos los NUM_PRIMES primeros primos en memoria
	for(i = 0; i < NUM_PRIMES ; i++){
		fscanf(primesfile, "%d/t", &temp);
		primes[i] = temp;
	}

	mpz_init(prime);
	srand(time(NULL));
    number = randnum(bits);
    mpz_set_str(prime, number, 2);

	begin = clock();
	// Pasamos el test de Miller Rabin hasta que encontremos un número primo
    while(Miller_Rabin(prime, tests, primes, NUM_PRIMES)){
    	mpz_add_ui(prime,prime,2);
    }
    end = clock();

	time_spent_M = (double)(end - begin) / CLOCKS_PER_SEC;

	begin = clock();
	is_prime = mpz_probab_prime_p(prime, tests);
	end = clock();

	time_spent_GMP = (double)(end - begin) / CLOCKS_PER_SEC;

    gmp_printf("Posible número primo encontrado: %Zd\n", prime);
    if(is_prime){
    	printf("Función GMP (mpz_probab_prime_p) considera que es un posible primo\n");
    }else{
    	printf("Función GMP (mpz_probab_prime_p) afirma que es un número compuesto\n");
    }

    printf("Tiempos de ejecución:\n");
	printf("\tMiller Rabin: %lfs\n", time_spent_M);
	printf("\tFunción GMP (mpz_probab_prime_p): %lfs\n", time_spent_GMP);

	if(outfile != stdout || outfile != NULL){
		fclose(outfile);
	}

    mpz_clear(prime);
    free(number);
	fclose(primesfile);

	return 1;
}


int divide_by_primes(mpz_t number, int* primes, int num_primes){
	int i;
	mpz_t prime, mod;
	mpz_init(prime);
	mpz_init(mod);

	// Recorremos los num_primes primeros primos del array
	for(i = 0; i < num_primes; i++){
		// Recuperamos el primo
		mpz_set_ui(prime, primes[i]);
		// Comprobamos si el primo no es igual a nuestro numero aleatorio
		if(mpz_cmp(number, prime) <= 0){
			mpz_clear(prime);
			mpz_clear(mod);
			return 1;
		}
		// Comprobamos si no es múltiplo del primo
		mpz_mod(mod,number,prime);
		if(!mpz_sgn(mod)){
			mpz_clear(prime);
			mpz_clear(mod);
			return 1;
		}
	}
	mpz_clear(prime);
	mpz_clear(mod);
	// No es divisible por ninguno de los 2000 primeros primos
	return 0;
}

int Miller_Rabin(mpz_t number, int tests, int* primes, int num_primes){
	int i,j;
	// k y m donde (number-1) = 2^k * m
	mpz_t k, m;
	// Numero aleatorio "a" entre (2 y number-1)
	mpz_t random;
	mpz_t two;
	mpz_t aux;
	mpz_t mod;
	// (number - 1)
	mpz_t n;
	gmp_randstate_t state;

	// Inicialización de las variables gmp
	mpz_init(m);
	mpz_init(k);
	mpz_init(random);
	mpz_init(two);
	mpz_init(aux);
	mpz_init(mod);
	mpz_init(n);

	// Calculamos k y m donde (number-1) = 2^k * m
	// n = number - 1
	mpz_sub_ui(n, number, 1);
	mpz_set(aux, n);
	while(!mpz_sgn(mod)){
		mpz_div_ui(aux, aux, 2);
		mpz_add_ui(k, k, 1);
		mpz_mod_ui(mod, aux, 2);
	}
	mpz_set(m, aux);

	// Comprobamos si el número elegido es divisible por los 2000 primeros primos
  	if(divide_by_primes(number, primes, num_primes)){
  		mpz_clear(m);
		mpz_clear(k);
		mpz_clear(random);
		mpz_clear(two);
		mpz_clear(aux);
		mpz_clear(mod);
		mpz_clear(n);
		return -1;
	}

	// Inicializamos la semilla de generación aleatoria de GMP
  	gmp_randinit_mt(state);
  	gmp_randseed_ui(state, (unsigned long)rand());

	mpz_set_ui(two, 2);
	for(i = 0; i < tests; i++){
		// Generamos un número entre 2 y n
		mpz_sub_ui(aux, n, 2);
		mpz_urandomm(random, state, aux);
		mpz_add_ui(random,random,2);

		// Elevamos random^m mod number = aux
		modpow(random, m, number, BEST_K_ARY, aux);
		// Comparamos el resultado de la exponenciación con 1 y con "n"
		if(mpz_cmp_ui(aux, 1) && mpz_cmp(aux, n)){
			// Bucle k-1 veces
			for(j = 0; mpz_cmp_ui(k, j); j++){
				// aux^2 mod number = aux
				modpow(aux, two, number, BEST_K_ARY, aux);
				// aux == 1? --> Es compuesto
				if(!mpz_cmp_ui(aux, 1)){
					mpz_clear(m);
					mpz_clear(k);
					mpz_clear(random);
					mpz_clear(two);
					mpz_clear(aux);
					mpz_clear(mod);
					mpz_clear(n);
					gmp_randclear (state);
					// Número compuesto
					return 1;
				}
				// aux == (number - 1)? --> Seguimos con otro test
				if(!mpz_cmp(aux, n)){
					break;
				}
			}
			// Hemos llegado al final del bucle? --> Es compuesto
			if(!mpz_cmp_ui(k,j)){
				mpz_clear(m);
				mpz_clear(k);
				mpz_clear(random);
				mpz_clear(two);
				mpz_clear(aux);
				mpz_clear(mod);
				mpz_clear(n);
				gmp_randclear (state);
				// Número compuesto
				return 1;
			}
		}
	}

	mpz_clear(m);
	mpz_clear(k);
	mpz_clear(random);
	mpz_clear(two);
	mpz_clear(aux);
	mpz_clear(mod);
	mpz_clear(n);
	gmp_randclear (state);
	// Si ha llegado hasta aquí es un posible primo (No es compuesto)
	return 0;


}

char* randnum(int bits){
	int i = 0;
    char* num = NULL;
    int aux = 0;

    if(bits < 0){
        printf("Error: Los parametros de la función randnum no son correctos\n");
        return NULL;
    }

    num = (char*)malloc(sizeof(char) * bits+1);
    if(num == NULL){
        printf("Error: Al realizar la reserva de memoria de num en randnum\n");
        return NULL;
    }

    // Queremos el bit de mayor peso a 1
    num[0] = '1';

	for(i = 1; i < (bits-1); i++) {
		aux = rand()%2;
		num[i] = '0'+aux;
	}
	num[i] = '1';
	num[i+1] = '\0';

    return num;
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
