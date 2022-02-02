

#include "../include/linealidad_SBoxes_DES.h"
#include "../include/DES_tables.h"

int main (int argc,char *argv[]) {

    int i = 0;
    double probabilidad = 0.0;

    printf("Comienza el estudio de linealidad de las SBoxes...\n");

    for(i = 0; i<8; i++){
        probabilidad = estudio_linealidad(S_BOXES[i]);
        printf("\nCaja %d:\n", i+1);
        printf("Pocentaje de coincidencia f(A ⊕ B) == f(A) ⊕ f(B):\n-->%.3lf%%\n", probabilidad);

    }


    return (0);
}
