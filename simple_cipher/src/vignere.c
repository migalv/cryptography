
#include "../includes/vignere.h"


/****************************************************************
 * Funcion que implementa el algoritmo de cifrado de Vignere. Recibe
 * un texto plano, que será cifrado, dada la clave k.
 *
 * Function: vignere_cifrar
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-char* --> Texto descifrado
 *            [2]-char* --> Clave
 * Return: Nada, el resultado se guarda en el fichero de salida
 ****************************************************************/
void vignere_cifrar(char* fin, char* fout, char* k){

	int i=0;
	FILE * f_entrada;
	FILE * f_salida;
	char caracter_plano;
	char caracter;

	f_entrada = fopen (fin, "r");
	if(!f_entrada){
		printf("Error abriendo el fichero\n");
		return;
	}
	f_salida = fopen (fout, "w");
	if(!f_salida){
		printf("Error abriendo el fichero\n");
		return;
	}

	while((caracter_plano = fgetc(f_entrada)) != EOF){		/*Obtenemos el texto caracter a caracter */

		caracter = (((caracter_plano-65)+(k[i]-65))%26)+65;	/*Ciframos cada caracter con la clave que le correcponde, debemos*/
															/* tener en cuenta que todo son operaciones modulares*/
		fprintf(f_salida, "%c", (char)caracter);			/*Guardamos el resultado*/
		i++;
		i%=strlen(k);										/*Realicamos el modulo para que itere en la clave*/
	}

	fclose(f_entrada);
	fclose(f_salida);
	
	return;
}

void vignere_descifrar(char* fin, char* fout, char* k){

	int i=0;
	FILE * f_entrada = NULL;
	FILE * f_salida = NULL;
	char caracter_cifrado;
	char caracter;

	f_entrada = fopen (fin, "r");
	if(!f_entrada){
		printf("Error abriendo el fichero\n");
		return;
	}
	f_salida = fopen (fout, "w");
	if(!f_salida){
		printf("Error abriendo el fichero\n");
		return;
	}

	while((caracter_cifrado = fgetc(f_entrada)) != EOF){			/*Desciframos el texto cifrado haciendo la operacion contraria al cifrado*/
		caracter = (caracter_cifrado-65)-(k[i]-65);					/*Restamos a cada caracter la clave correspondiente*/
		caracter = (caracter >= 0 ? caracter : caracter+26) + 65; 	/*Si el resultado es negativo hacemod el modulo 26*/
		fprintf(f_salida, "%c", (char)caracter);
		i++;
		i%=strlen(k);
	}

	fclose(f_entrada);
	fclose(f_salida);
	return;
}

