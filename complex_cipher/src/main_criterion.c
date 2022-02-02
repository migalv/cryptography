// Programa que comprueba el principio de diseño SAC del DES
// Strict Avalanche Criterion

#include "../include/criterion.h"

int main(int argc, char* argv[]){

	double** p = NULL;
	double** pc = NULL;
	int j = 0, k = 0, is = 0, ic = 0;

	p = (double**) malloc(NUM_S_BOXES*sizeof(double*));

	printf("========== ANALIZANDO CRITERIO SAC ==========\n\n");
	// Bucle que recorre las 8 Sbox
	for(is = 0; is < NUM_S_BOXES; is++){
		printf("Para la SBox-%d\n", is+1);
		p[is] = SAC(S_BOXES[is]);
		for(ic = 0; ic < 4; ic++){
			printf("  p(b%d) = %f\n", ic, p[is][ic]);
		}
	}
	printf("========== ANALIZANDO CRITERIO BIC ==========\n\n");
	// Bucle que recorre las 8 Sbox
	for(is = 0; is < NUM_S_BOXES; is++){
		printf("Para la SBox-%d\n", is+1);
		pc = BIC(S_BOXES[is]);
		for(j = 0; j < 4; j++){
			for(k = j+1; k < 4; k++){
				printf("  p(b%d, b%d) = %f --- ", j, k, pc[j][k]);
				printf("p(b%d) * p(b%d) = %f\n", j, k, p[is][j] * p[is][k]);
			}
		}
	}

	for(j = 0; j < NUM_S_BOXES; j++){
		free(p[j]);
	}
	free(p);

	for(j = 0; j < 4; j++){
		free(pc[j]);
	}
	free(pc);

	return 1;
}
