/*
 * Modulo dedicado al cifrado de doble permutacion
 *
 * Modulo: permutacion.c
 * Autor: Miguel Alvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 22/10/2018
 */

#include "../includes/permutacion.h"

char* cifrar_doble(char* texto_plano, int* permutacionK1, int* permutacionK2, int num_k1, int num_k2){

	int i = 0, j = 0, k = 0;
	// Bloque de num_k2 letras para formar la matriz bloque
	char* bloque = NULL;
	// Matriz bloque, donde se permutaran las filas y las columnas
	char** matriz_bloque = NULL;
	// Punteros auxiliares que permiten las permutaciones
	char* foobar1 = NULL;
	char* foobar2 = NULL;
	// Texto una vez cifrado
	char* texto_cifrado = NULL;
	// Tamaño del texto_plano
	int length = 0;
	// Tamanio de la matriz_bloque
	int tam_matriz = 0;

	if(texto_plano == NULL || permutacionK1 == NULL || permutacionK2 == NULL || num_k1 < 0 || num_k2 < 0){
		fprintf(stderr, "Error: Parametros incorrectos de la función cifrar_doble.\n");
        return NULL;
	}

	// Matriz bloque (Tamanio num_k1*num_k2)
	matriz_bloque = (char**)malloc(sizeof(char*)*num_k1);
	if(matriz_bloque == NULL){
        fprintf(stderr, "Error[645]: Al reservar memoria para la matriz_bloque.\n");
        return NULL;
    }

	tam_matriz = num_k1*num_k2;
	length = strlen(texto_plano);

    // Reservamos memoria para el texto cifrado
    texto_cifrado = (char*)malloc(sizeof(char)*length+1);
    if(texto_cifrado == NULL){
        fprintf(stderr, "Error[645]: Al reservar memoria para el texto cifrado.\n");
        return NULL;
    }
	texto_cifrado[0] = '\0';

	for(i = 0; i < length; i+=tam_matriz){
		// Creamos la matriz con los bloques
		for(j = 0; j < num_k1; j++){
			bloque = strndup(&texto_plano[i+(j*(num_k2))], num_k2);
			matriz_bloque[j] = bloque;
		}
		// Bucle de permutacion de filas
		for(j = 0; j < num_k1; j++){
			// Recuperamos el bloque que vamos a cambiar de lugar
			foobar1 = matriz_bloque[j];
			// Recuperamos el bloque donde se va a insertar
			foobar2 = matriz_bloque[(permutacionK1[j]-1)];

			// Cambiamos de sitio segun lo que mande la permutacion K1
			matriz_bloque[(permutacionK1[j]-1)] = foobar1;
			// Guardamos el bloque sobreescrito donde el bloque permutado
			matriz_bloque[j] = foobar2;
		}
		foobar1 = NULL;
		foobar2 = NULL;
		// Bucle de permutacion de columnas
		for(j = 0; j < num_k2; j++){
			for(k = 0; k < num_k1; k++){
				foobar1 = (char*)malloc(sizeof(char));
				foobar2 = (char*)malloc(sizeof(char));

				// Recuperamos la letra que vamos a permutar
				*foobar1 = matriz_bloque[k][j];
				// Recuperamos la letra donde se va a insertar
				*foobar2 = matriz_bloque[k][(permutacionK2[j]-1)];
				// Cambiamos de sitio segun lo que mande la permutacion K2
				matriz_bloque[k][(permutacionK2[j]-1)] = *foobar1;
				// Guardamos la letra sobreescrita donde la letra permutada
				matriz_bloque[k][j] = *foobar2;

				free(foobar1);
				free(foobar2);
			}
		}

		for(j = 0; j < num_k1; j++){
			strcat(texto_cifrado, matriz_bloque[j]);
			free(matriz_bloque[j]);
		}
	}

	free(matriz_bloque);
	free(texto_plano);

	return texto_cifrado;
}

char* descifrar_doble(char* texto_cifrado, int* permutacionK1, int* permutacionK2, int num_k1, int num_k2){
	int i = 0, j = 0, k = 0;
	// Bloque de num_k2 letras para formar la matriz bloque
	char* bloque = NULL;
	// Matriz bloque, donde se permutaran las filas y las columnas
	char** matriz_bloque = NULL;
	// Punteros auxiliares que permiten las permutaciones
	char* foobar1 = NULL;
	char* foobar2 = NULL;
	// Texto una vez cifrado
	char* texto_descifrado = NULL;
	// Tamaño del
	int length = 0;
	// Tamanio de la matriz_bloque
	int tam_matriz = 0;

	if(texto_cifrado == NULL || permutacionK1 == NULL || permutacionK2 == NULL || num_k1 < 0 || num_k2 < 0){
		fprintf(stderr, "Error: Parametros incorrectos de la función cifrar_doble.\n");
        return NULL;
	}

	// Matriz bloque (Tamanio num_k1*num_k2)
	matriz_bloque = (char**)malloc(sizeof(char*)*num_k1);
	if(matriz_bloque == NULL){
        fprintf(stderr, "Error[645]: Al reservar memoria para la matriz_bloque.\n");
        return NULL;
    }

	tam_matriz = num_k1*num_k2;
	length = strlen(texto_cifrado);

    // Reservamos memoria para el texto cifrado
    texto_descifrado = (char*)malloc(sizeof(char)*length+1);
    if(texto_descifrado == NULL){
        fprintf(stderr, "Error[645]: Al reservar memoria para el texto cifrado.\n");
        return NULL;
    }
	texto_descifrado[0] = '\0';

	for(i = 0; i < length; i+=tam_matriz){
		// Creamos la matriz con los bloques
		for(j = 0; j < num_k1; j++){
			bloque = strndup(&texto_cifrado[i+(j*(num_k2))], num_k2);
			matriz_bloque[j] = bloque;
		}
		foobar1 = NULL;
		foobar2 = NULL;
		// Bucle de permutacion de columnas
		for(j = 0; j < num_k2; j++){
			for(k = 0; k < num_k1; k++){
				foobar1 = (char*)malloc(sizeof(char));
				foobar2 = (char*)malloc(sizeof(char));

				// Recuperamos la letra que vamos a permutar
				*foobar1 = matriz_bloque[k][j];
				// Recuperamos la letra donde se va a insertar
				*foobar2 = matriz_bloque[k][(permutacionK2[j]-1)];
				// Cambiamos de sitio segun lo que mande la permutacion K2
				matriz_bloque[k][(permutacionK2[j]-1)] = *foobar1;
				// Guardamos la letra sobreescrita donde la letra permutada
				matriz_bloque[k][j] = *foobar2;

				free(foobar1);
				free(foobar2);
			}
		}
		// Bucle de permutacion de filas
		for(j = 0; j < num_k1; j++){
			// Recuperamos el bloque que vamos a cambiar de lugar
			foobar1 = matriz_bloque[j];
			// Recuperamos el bloque donde se va a insertar
			foobar2 = matriz_bloque[(permutacionK1[j]-1)];

			// Cambiamos de sitio segun lo que mande la permutacion K1
			matriz_bloque[(permutacionK1[j]-1)] = foobar1;
			// Guardamos el bloque sobreescrito donde el bloque permutado
			matriz_bloque[j] = foobar2;
		}

		for(j = 0; j < num_k1; j++){
			strcat(texto_descifrado, matriz_bloque[j]);
			free(matriz_bloque[j]);
		}
	}

	free(matriz_bloque);
	free(texto_cifrado);

	return texto_descifrado;
}
