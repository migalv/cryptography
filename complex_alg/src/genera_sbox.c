// Programa que genera Sbox que cumplen los criterios del DES

#include "../include/DES_tables.h"
#include "../include/criterion.h"
#include "../include/linealidad_SBoxes_DES.h"
#include <time.h>
#include <unistd.h>

struct Set_t{
	short* value;
	int size;
	int max_size;
};

typedef struct Set_t Set;

Set* initSet(int size);
int exists(Set* set, short value);
int addToSet(Set* set, short value);
void destroySet(Set* set);
void printSet(Set* set);

#define IT 1000
#define UMBRAL_SAC_I 0.45
#define UMBRAL_SAC_S 0.6
#define UMBRAL_BIC 0.2
#define UMBRAL_LINEAL 0.1

int main(int argc, char* argv[]){

	int i = 0, j = 0, k = 0;
	unsigned short R_BOXES[NUM_S_BOXES][ROWS_PER_SBOX][COLUMNS_PER_SBOX];
	double* p_sac;
	double** p_bic;
	double media_sac = 0.0, media_bic = 0.0, linealidad = 0.0;
	double diferencia = 0.0;
	int randomIndex1 = 0, randomIndex2 = 0;
	short temp;

	i = 0;

	fprintf(stdout, "static const unsigned short R_BOXES[NUM_S_BOXES][ROWS_PER_SBOX][COLUMNS_PER_SBOX] = {\n");

	while(i < NUM_S_BOXES){
		for(j = 0; j < ROWS_PER_SBOX; j++){
			for(k = 0; k < COLUMNS_PER_SBOX; k++){
				R_BOXES[i][j][k] = k;
			}
			for(k = 0; k < IT; k++){
				srand(clock());
				randomIndex1 = rand() % 16;
				randomIndex2 = rand() % 16;

				temp = R_BOXES[i][j][randomIndex1];
				R_BOXES[i][j][randomIndex1] = R_BOXES[i][j][randomIndex2];
				R_BOXES[i][j][randomIndex2] = temp;
			}
		}
		media_sac = 0.0;
		media_bic = 0.0;
		diferencia = 0.0;
		p_sac = SAC(R_BOXES[i]);
		if(p_sac == NULL){
			return -1;
		}
		for(j = 0; j < 4; j++){
			media_sac += p_sac[j];
		}
		media_sac /= 4;
		if(UMBRAL_SAC_I < media_sac && media_sac < UMBRAL_SAC_S){
			p_bic = BIC(R_BOXES[i]);
			if(p_bic == NULL){
				return -1;
			}
			for(j = 0; j < 4; j++){
				for(k = j+1; k < 4; k++){
					diferencia += p_bic[j][k] - p_sac[j]*p_sac[k];
				}
			}
			for(j = 0; j < 4; j++){
				free(p_bic[j]);
			}
			free(p_bic);
			media_bic = diferencia/6;
			if((-UMBRAL_BIC) < media_bic && media_bic < UMBRAL_BIC){
				linealidad = estudio_linealidad(R_BOXES[i]);
				if(linealidad < UMBRAL_LINEAL){
					//fprintf(stdout, "R_Box%d\n", i+1);
					fprintf(stdout, "{\t");
					for(j = 0; j < ROWS_PER_SBOX; j++){
						fprintf(stdout, "{");
						for(k = 0; k < COLUMNS_PER_SBOX; k++){
							if(k == COLUMNS_PER_SBOX-1)
								fprintf(stdout, " %d", R_BOXES[i][j][k]);
							else
								fprintf(stdout, " %d,", R_BOXES[i][j][k]);
						}
						fprintf(stdout, " },\n");
					}
					fprintf(stdout, "\t}\n");
					i++;
				}
			}
		}
		free(p_sac);
	}

	fprintf(stdout, "};");

	return 1;
}

int exists(Set* set, short value){
	int i = 0;

	for(i = 0; i < set->size; i++){
		if(set->value[i] == value)
			return 1;
	}

	return 0;
}

int addToSet(Set* set, short value){

	if(exists(set, value) == 1){
		return 0;
	}

	if(set->size != set->max_size){
		set->value[set->size] = value;
		set->size++;
		return 1;
	}

	return -1;
}

Set* initSet(int size){
	Set* newSet;
	int i = 0;

	newSet = (Set*)malloc(sizeof(Set));
	newSet->value = (short*)malloc(size*sizeof(short));

	for(i = 0; i < size; i++){
		newSet->value[i] = 0;
	}

	newSet->size = 0;
	newSet->max_size = size;

	return newSet;
}

void printSet(Set* set){

	int i = 0;

	printf("Set: ");
	for(i = 0; i < set->size; i++){
		printf("%d ", set->value[i]);
	}
	printf("\n");

	return;
}

void destroySet(Set* set){

	set->size = 0;

	if(set->value != NULL){
		free(set->value);
	}

	return;
}
