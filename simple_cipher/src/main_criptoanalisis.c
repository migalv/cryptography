/** 
 * Main encargado de mostrar el funcionamiento 
 * del criptoanalisis a un texto cifrado
 * por vignere
 *
 * Autor: Alberto Garcia Redero
 * Version: 1.0
 *
 * Fecha: 18/10/2018 
 */

#include "../includes/criptoanalisis.h"


int main (int argc,char *argv[]) {

	int ngrama=-1;
	int tam_k=-1;

	if(argc!=MAX_ARG_ANALISIS && argc!=AYUDA){
		printf("Introduzca correctamente los argumentos.\n");
		printf("Para ver la ayuda ./criptoanalisis -h.\n");

		return(0);
	}
	if(argc == AYUDA && strcmp(argv[1],"-h") == 0){
	  	printf("Ejemplo:\n\t./criptoanalisis {-l Ngrama} [-i filein] [-o fileout]\n\n");
	  	printf("OPCION\t\t\tDESCRIPCION\n");
	  	printf("-l\t\t\tlongitud de ngrama buscado\n");
	  	printf("-i\t\t\tfichero de entrada\n");
	  	printf("-o\t\t\tfichero de salida\n");
	  	return (0);
	}	

	ngrama = atoi(argv[2]);										/*Obtenemos el tamano de ngrama*/
	if(ngrama==0){
		printf("\nERROR: Introduzca un tamaño de ngrama correcto\n");
		return (0);
	}

	printf("\nComienza test de kasiski...\n");	
	tam_k = kasiski(argv[4], argv[6], ngrama);					/*Empezamos realizando el test de kasiski, el cual nos devuelve el tamaño de la clave*/
	if(tam_k==-1){
		printf("ERROR realizando el test de kasiski\n");
		return (0);
	}

	printf("\nComienza indice de coicidencia...\n");
	indice_coincidencia(argv[4], argv[6], ngrama);				/*Probamos el indice de coincidencia, probamos si el tamaño de clave coincide con*/
																/*el tamano de ngrama introducido inicialmente, comprueba si es cierto o no*/
	printf("\nLa clave es...\n");
	ic_clave(argv[4], argv[6], tam_k);							/*Obtenemos la clave gracias al indice de coincidencia*/

	return (0);
}