/**
 * Modulo encargado analizar los criterios del DES
 *
 * Modulo: criterion.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 23/11/2018
 */

#include "../include/criterion.h"
#include <inttypes.h>

/****************************************************************
 * Funcion que calcula las probabilidades del criterio SAC sobre
 * las salidas de la SBox pasada como argumento.
 *
 * IMPORTANTE! -> La caja de sustitucion debe ser de 4x16.
 * 			   4 Filas y 16 Columnas.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: SAC
 * Arguments: [0]-const short Caja de sustitucion a analizar
 * Return: double* Array con la probabilidad de cada salida de la Sbox
 ****************************************************************/
double* SAC(unsigned short box[ROWS_PER_SBOX][COLUMNS_PER_SBOX]){
	// Entrada de la SBox complementada
	uint8_t comp = 0x0;
	uint8_t a1 = 0x0, a6 = 0x0;
	uint8_t fila = 0x0;
	uint8_t columna = 0;
	// Resultado de la Sbox aplicada sobre la entrada
	uint8_t r_entrada = 0x0;
	// Resultado de la Sbox aplicada sobre la entrada complementada
	uint8_t r_comp = 0x0;
	// Array que contiene los cambios realizados en cada bit de salida de las Sbox
	double* p = NULL;
	int i = 0, j = 0, ic = 0;

	p = (double*)malloc(4*sizeof(double));
	if(p == NULL){
		fprintf(stderr, "Error[71]: Se realizó mal la reserva de memoria de P\n");
		return NULL;
	}
	for(i = 0; i < 4; i++){
		p[i] = 0.0;
	}

	for(ic = 0; ic < 4; ic++){
		// Bucle que recorre las 64 posibles entradas de una Sbox
		for(i = 0; i < ENTRADAS; i++){
			// Bucle que calcula los 6 complementarios cada entrada
			for(j = 0; j < 6; j++){
				// Calculamos el complementario
				comp = i ^ (1 << j);

				// Recuperamos el bit de más peso de la entrada (a1)
				a1 = (i & 0x20) >> 4;
				// Recuperamos el bit de menor peso de la entrada (a6)
				a6 = i & 0x01;
				columna = (i & 0x1E) >> 1;
				fila = a1 | a6;
				r_entrada = box[fila][columna];

				// Recuperamos el bit de más peso del complementario(a1)
				a1 = (comp & 0x20) >> 4;
				// Recuperamos el bit de menor peso del complementario (a6)
				a6 = comp & 0x01;
				columna = (comp & 0x1E) >> 1;
				fila = a1 | a6;
				r_comp = box[fila][columna];
				// Comparamos cada bit de la salida
				if(((r_entrada >> ic) & 1) != ((r_comp >> ic) & 1))
					p[ic]++;
			}
		}
		// Aque se divide el total de bits de la salida que cambiaron
		// Entre las 64 posibles entradas de la Sbox y sus 6 complementarios
		// Calculando así la probabilidad de cada bit de la salida
		p[ic] = p[ic]/(double)(64*6);
	}

	return p;
}

/****************************************************************
 * Funcion que calcula las probabilidades del criterio BIC sobre
 * las salidas de la SBox pasada como argumento.
 * La salida de la funcion sera una matriz que contiene las posibles
 * combinaciones de probabilidades de las salidas. En total 6.
 * salida[0][1], salida[0][2], salida[0][3]
 * salida[1][2], salida[1][3],
 * salida[2][3].
 *
 * IMPORTANTE! -> La caja de sustitucion debe ser de 4x16.
 * 			   4 Filas y 16 Columnas.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: BIC
 * Arguments: [0]-const short Caja de sustitucion a analizar
 * Return: double** Matriz de probabilidades.
 ****************************************************************/
double** BIC(unsigned short box[ROWS_PER_SBOX][COLUMNS_PER_SBOX]){
	// Entrada de la SBox complementada
	uint8_t comp = 0x0;
	uint8_t a1 = 0x0, a6 = 0x0;
	uint8_t fila = 0x0;
	uint8_t columna = 0;
	// Resultado de la Sbox aplicada sobre la entrada
	uint8_t r_entrada = 0x0;
	// Resultado de la Sbox aplicada sobre la entrada complementada
	uint8_t r_comp = 0x0;
	// Array que contiene los cambios realizados en los bits de las posibles
	// Combinaciones de los bits de salida
	double** pc;
	int i = 0, j = 0, k = 0, ie = 0;

	pc = (double**)malloc(4*sizeof(double*));
	if(pc == NULL){
		fprintf(stderr, "Error[71]: Se realizó mal la reserva de memoria de P\n");
		return NULL;
	}
	for(i = 0; i < 4; i++){
		pc[i] = (double*)malloc(4*sizeof(double));
		if(pc[i] == NULL){
			fprintf(stderr, "Error[71]: Se realizó mal la reserva de memoria de P\n");
			return NULL;
		}
		for(j = 0; j < 4; j++){
			pc[i][j] = 0.0;
		}
	}


	// Bucle que recorre las diferentes combinaciones de los bits de salidas
	for(j = 0; j < 4; j++){
		// Bucle que recorre las 64 posibles entradas de una Sbox
		for(ie = 0; ie < ENTRADAS; ie++){
			// Bucle que calcula los 6 complementarios cada entrada
			for(i = 0; i < 6; i++){
				// Calculamos el complementario
				comp = ie ^ (1 << i);

				// Recuperamos el bit de más peso de la entrada (a1)
				a1 = (ie & 0x20) >> 4;
				// Recuperamos el bit de menor peso de la entrada (a6)
				a6 = ie & 0x01;
				columna = (ie & 0x1E) >> 1;
				fila = a1 | a6;
				r_entrada = box[fila][columna];

				// Recuperamos el bit de más peso del complementario(a1)
				a1 = (comp & 0x20) >> 4;
				// Recuperamos el bit de menor peso del complementario (a6)
				a6 = comp & 0x01;
				columna = (comp & 0x1E) >> 1;
				fila = a1 | a6;
				r_comp = box[fila][columna];

				// Bucle más interno que recorre las diferentes combinaciones de los bits de salidas
				for(k = j+1; k < 4; k++){
					if((((r_entrada >> k) & 1) != ((r_comp >> k) & 1)) && (((r_entrada >> j) & 1) != ((r_comp >> j) & 1)))
						pc[j][k]++;
				}
			}
		}
	}
	for(j = 0; j < 4; j++){
		for(k = j+1; k < 4; k++){
			pc[j][k] = pc[j][k]/(double)(64*6);
		}
	}

	return pc;
}
