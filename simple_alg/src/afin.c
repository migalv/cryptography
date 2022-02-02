

#include "../includes/afin.h"

/****************************************************************
 * Funcion que implementa el algoritmo de cifrado afin
 *
 * Function: afin_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-mpz_t --> a para ax+b mod m
 *            [2]-mpz_t --> b para ax+b mod m
 *            [2]-mpz_t --> m para ax+b mod m
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void afin_cifrar(char* fin, char* fout, mpz_t a, mpz_t b, mpz_t m){

	FILE * f_entrada;
	FILE * f_salida;
	char caracter_plano;
	unsigned long caracter;
	mpz_t caracter_plano_mpz, caracter_cifrado_mpz, suma, producto, desplazador_ascii;
	mpz_t caracter_cifrado_desplazado, caracter_plano_mpz_desplazado;

	mpz_init (caracter_cifrado_mpz);
	mpz_init (caracter_cifrado_desplazado);
	mpz_init (suma);
	mpz_init (producto);
	mpz_init (desplazador_ascii);
	mpz_init (caracter_plano_mpz);
	mpz_init (caracter_plano_mpz_desplazado);

	mpz_set_str (desplazador_ascii,"65",10);

	f_entrada = fopen (fin, "r");
	f_salida = fopen (fout, "w");


	while((caracter_plano = fgetc(f_entrada)) != EOF){

		mpz_set_ui (caracter_plano_mpz,(unsigned long)caracter_plano);
		mpz_sub(caracter_plano_mpz_desplazado, caracter_plano_mpz, desplazador_ascii);
		mpz_mul(producto, caracter_plano_mpz_desplazado, a);
		mpz_add(suma, producto, b);
		mpz_mod(caracter_cifrado_mpz, suma, m);
		mpz_add(caracter_cifrado_desplazado, caracter_cifrado_mpz, desplazador_ascii);

		caracter = mpz_get_ui(caracter_cifrado_desplazado);

		fprintf(f_salida, "%c", (char)caracter);
		//printf("%c", (char)caracter);
	}


	fclose(f_entrada);
	fclose(f_salida);

	mpz_clear (caracter_plano_mpz);
	mpz_clear (desplazador_ascii);
	mpz_clear (caracter_cifrado_mpz);
	mpz_clear (caracter_cifrado_desplazado);
	mpz_clear (producto);
	mpz_clear (suma);
	mpz_clear (caracter_plano_mpz_desplazado);

	return;
}

/****************************************************************
 * Funcion que implementa el algoritmo de descifrado afin
 *
 * Function: afin_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-mpz_t --> b para (y-b)*(a^-1) mod m
 *            [2]-mpz_t --> s=(a^-1) para (y-b)*(a^-1) mod m
 *            [2]-mpz_t --> m para (y-b)*(a^-1) mod m
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void afin_descifrar(char* fin, char* fout, mpz_t b, mpz_t s, mpz_t m){

	FILE * f_entrada;
	FILE * f_salida;
	char caracter_cifrado;
	unsigned long caracter;
	mpz_t caracter_plano_mpz, caracter_cifrado_mpz, resta, producto, desplazador_ascii;
	mpz_t caracter_plano_desplazado, caracter_cifrado_mpz_desplazado;

	mpz_init (caracter_cifrado_mpz);
	mpz_init (resta);
	mpz_init (producto);
	mpz_init (desplazador_ascii);
	mpz_init (caracter_plano_desplazado);
	mpz_init (caracter_plano_mpz);
	mpz_init (caracter_cifrado_mpz_desplazado);

	mpz_set_str (desplazador_ascii,"65",10);
	
	f_entrada = fopen (fin, "r");
	f_salida = fopen (fout, "w");

	while((caracter_cifrado = fgetc(f_entrada)) != EOF){

		mpz_set_ui (caracter_cifrado_mpz,(unsigned long)caracter_cifrado);
		mpz_sub(caracter_cifrado_mpz_desplazado, caracter_cifrado_mpz, desplazador_ascii);
		mpz_sub(resta, caracter_cifrado_mpz_desplazado, b);
		mpz_mul(producto, resta, s);
		mpz_mod(caracter_plano_mpz, producto, m);
		mpz_add(caracter_plano_desplazado, caracter_plano_mpz, desplazador_ascii);

		caracter = mpz_get_ui(caracter_plano_desplazado);

		fprintf(f_salida, "%c", (char)caracter);
		//printf("%c", (char)caracter);
	}


	fclose(f_entrada);
	fclose(f_salida);

	mpz_clear (caracter_cifrado_mpz);
	mpz_clear (resta);
	mpz_clear (producto);
	mpz_clear (desplazador_ascii);
	mpz_clear (caracter_plano_desplazado);
	mpz_clear (caracter_plano_mpz);
	mpz_clear (caracter_cifrado_mpz_desplazado);

	return;
}

