

#include "../include/linealidad_SBoxes_AES.h"


int main (int argc,char *argv[]) {

    int estudiar_tabla_directa = 1; //Directa

    printf("Comienza el estudio de linealidad de las SBoxes...\n");

    estudio_linealidad(estudiar_tabla_directa);

    estudiar_tabla_directa = 0;     //Indirecta

    estudio_linealidad(estudiar_tabla_directa);

    return (0);
}
