// Programa que comprueba el efecto avalancha

#include "../include/desCBC.h"
#include <time.h>

uint64_t change_bits(uint64_t bytes, int nbits);
int compare(uint64_t bytes1, uint64_t bytes2, int nbits);

int main(int argc, char* argv[]) {

	uint64_t bytes = 0x0;
	uint64_t c_bytes = 0x0;
	uint64_t clave = 0x0;
	uint64_t* claves = NULL;
	uint64_t c_clave = 0x0;
	uint64_t* c_claves = 0x0;
	uint64_t* p_aux = NULL;
	int modo = -1;
	int nbits = 1;
	int i = 0;

	srand(time(NULL));

	for (i=0; i<64; i++) {
		bytes = (bytes<<1) | rand()%2;
	}

	if(argc < 2){
		printf("Introduzca correctamente los argumentos.\n");
		printf("Para ver la ayuda %s -h.\n", argv[0]);
		return -1;
	}

	for(i=1;i<argc;i++){
		// Modo 0 El programa comprueba cuantos cambios se realizan sobre los datos.
  		if(!strcmp(argv[i],"-D")){
  			modo = 0;
		}
  		else if(!strcmp(argv[i],"-K")) {// Modo 1 El programa comprueba cuantos
			modo = 1; 					//cambios se realizan sobre las claves.
		}
		else if(!strcmp(argv[i],"-n")){ // OPCIÓN que permite escoger cuantos
			if(i+1 == argc){		// bits se van a cambiar
                printf("Introduzca correctamente los argumentos.\n");
                printf("Para ver la ayuda %s -h.\n", argv[0]);
                return -1;
            }
  				nbits = atoi(argv[i+1]);
  		}else if(!strcmp(argv[i],"-h")){
			printf("Ejemplo:\n\n%s {-D | -K} [-n nbits]\n\n", argv[0]);
	        printf("ARGUMENTO\t\t\tDESCRIPCION\n");
	        printf("-D\t\t\tEl programa comprueba cuantos cambios se realizan sobre los datos.\n");
	        printf("-K\t\t\tEl programa comprueba cuantos cambios se realizan sobre la clave.\n");
			printf("\nOPCION\t\t\tDESCRIPCION\n");
	        printf("-n nbits\t\tNúmero de bits a cambiar. (por defecto 1)\n");
			printf("\n  Ejemplos de uso:\n\n");
			printf("\t%s -D \n", argv[0]);
			printf("\t%s -K -n 2\n", argv[0]);
			printf("\t%s -D -n 4\n", argv[0]);

			return 1;
		}
  	}

	if(modo == -1){
		printf("Introduzca correctamente los argumentos.\n");
		printf("Para ver la ayuda %s -h.\n", argv[0]);
		return -1;
	}

	if(modo == 0){
		printf("\nSe ha elegido la opción de cambios sobre los DATOS\n");
		c_bytes = change_bits(bytes, nbits);
		printf("\n");
		printf("Datos binario\t\t->  ");
		print_bits(sizeof(uint64_t), &bytes);
		printf("Datos con %d cambios\t->  ", nbits);
		print_bits(sizeof(uint64_t), &c_bytes);
		printf("\n");

		// Permutacion IP
		p_aux = (uint64_t*)permutar(&bytes, TAM_CLAVE_BITS, IP, BITS_IN_IP);
		if(p_aux == NULL){
			fprintf(stderr, "Error[123]: Al llamar a permutar.\n");
			return -1;;
		}
		bytes = *p_aux;

		// Permutacion IP
		p_aux = (uint64_t*)permutar(&c_bytes, TAM_CLAVE_BITS, IP, BITS_IN_IP);
		if(p_aux == NULL){
			fprintf(stderr, "Error[123]: Al llamar a permutar.\n");
			return -1;;
		}
		c_bytes = *p_aux;
	}
	else{
		printf("\nSe ha elegido la opción de cambios sobre la CLAVE\n");
		// Permutacion IP
		p_aux = (uint64_t*)permutar(&bytes, TAM_CLAVE_BITS, IP, BITS_IN_IP);
		if(p_aux == NULL){
			fprintf(stderr, "Error[123]: Al llamar a permutar.\n");
			return -1;;
		}
		bytes = *p_aux;
		c_bytes = *p_aux;
	}

	printf("Generando clave...\n");
	clave = generar_clave();
	printf("\n");

	printf("Clave en binario\t->  ");
	print_bits(sizeof(uint64_t), &clave);

	if(modo == 1){
		c_clave = change_bits(clave, nbits);
		printf("Clave con %d cambios\t->  ", nbits);
		print_bits(sizeof(uint64_t), &c_clave);
		// Generamos las 16 claves de 56 bits de la clave cambiada
		c_claves = generar_claves(c_clave);
	}

	printf("\n");

	// Generamos las 16 claves de 56 bits de la clave original
	claves = generar_claves(clave);

	if(modo == 0){;
		for(i = 0; i < ROUNDS; i++){
			bytes = ronda(bytes, claves[i]);
			c_bytes = ronda(c_bytes, claves[i]);
			printf("  %d Cambios al cambiar %d bits en la entrada - Ronda %d\n", compare(bytes, c_bytes, TAM_CLAVE_BITS), nbits, i+1);
		}
	}
	if(modo == 1){
		for(i = 0; i < ROUNDS; i++){
			bytes = ronda(bytes, claves[i]);
			c_bytes = ronda(c_bytes, c_claves[i]);
			printf("  %d Cambios al cambiar %d bits en la clave - Ronda %d\n", compare(bytes, c_bytes, TAM_CLAVE_BITS), nbits, i+1);
		}
	}

	return 1;
}

uint64_t change_bits(uint64_t bytes, int nbits){

	// Posicion del bit a cambiar
	int random = 0;
	int i = 0;

	srand(time(NULL));

	for(i = 0; i < nbits; i++){
		// Obtenemos la posicion del bit a cambiar
		random = rand() % TAM_CLAVE_BITS;

		bytes = bytes ^ (1 << random);
	}

	return bytes;
}

int compare(uint64_t bytes1, uint64_t bytes2, int nbits){
	int i = 0, count = 0;

	for(i = 0; i < nbits; i++){
		if(((bytes1 >> i) & 1) != ((bytes2 >> i) & 1))
			count++;
	}

	return count;
}
