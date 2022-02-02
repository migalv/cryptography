
#include "../includes/afin.h"
#include "../includes/afin_mejorado.h"

/****************************************************************
 * Funcion que implementa el algoritmo de cifrado afin mejorado
 * en este se cifra dos veces modificando en la segunda el modulo,
 * el cual es b*m
 *
 * Function: afin_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-mpz_t --> a para ax+b mod m
 *            [3]-mpz_t --> b para ax+b mod m
 *            [4]-mpz_t --> m para ax+b mod m, modulo del 
 *											   primer cifrado
 *            [5]-mpz_t --> m2 para ax+b mod m2, modulo del 
 *											   segundo cifrado
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void afin_mejorado_cifrar(char* fin, char* fout, mpz_t a, mpz_t b, mpz_t m, mpz_t m2){

	char faux[] = "cif_aux.txt";

	afin_cifrar(fin, faux, a, b, m);		/*Ciframos una vez el texto*/

	afin_cifrar(faux, fout, a, b, m2);		/*Ciframos el texto cifrado otra vez modificando el modulo, m2 = m*b*/

	remove(faux);
}

/****************************************************************
 * Funcion que implementa el algoritmo de descifrado afin mejorado
 *
 * Function: afin_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-mpz_t --> b para (y-b)*(a^-1) mod m
 *            [3]-mpz_t --> s=(a^-1) para (y-b)*(a^-1) mod m
 *            [4]-mpz_t --> s2=(a^-1) para (y-b)*(a^-1) mod m2
 *            [5]-mpz_t --> m para (y-b)*(a^-1) mod m
 *            [6]-mpz_t --> m2 para (y-b)*(a^-1) mod m2
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void afin_mejorado_descifrar(char* fin, char* fout, mpz_t b, mpz_t s, mpz_t s2, mpz_t m, mpz_t m2){

	char faux[] = "cif_aux.txt";

	afin_descifrar(fin, faux, b, s2, m2);		/*Desciframos el segundo cifrado, donde s2=(a^-1) para (y-b)*(a^-1) mod m2*/

	afin_descifrar(faux, fout, b, s, m);		/*Desciframos el primer cifrado*/

	remove(faux);
}