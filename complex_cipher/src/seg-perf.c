
#include "../include/seg-perf.h"

int* repeticiones_en_texto(char* entrada){

    int* prob = NULL;
    int i = 0;


    prob= (int*)malloc(sizeof(int)*26);
    if(!prob){
        return NULL;
    }

    memset (prob,0,sizeof(int)*26);

    for(i = 0; i < nCaracteres; i++){
        prob[entrada[i]-65]++;
    }

    return prob;
}

char* cifrado_equiprobable(char* texto_plano){

    int i = 0;
    time_t t;
    int cif = 0;
    char* texto_cifrado = NULL;

    texto_cifrado = (char*)malloc(sizeof(char)*nCaracteres+1);
    if(!texto_cifrado){
        return NULL;
    }

    srand((unsigned) time(&t));

    for(i = 0; i < nCaracteres; i++){                       /*Probilidad de cifrar con A-Z es 1/26*/
        cif = (rand()%26);
        texto_cifrado[i] = (((texto_plano[i]-65) + cif)%26)+65;
    }


    return texto_cifrado;
}

char* cifrado_no_equiprobable(char* texto_plano){

    int i = 0;
    time_t t;
    int cif = 0;
    int limitador = 0;
    char* texto_cifrado = NULL;

    texto_cifrado = (char*)malloc(sizeof(char)*nCaracteres+1);
    if(!texto_cifrado){
        return NULL;
    }

    srand((unsigned) time(&t));

    for(i = 0; i < nCaracteres; i++){                           /*Para que no sea equiprobable:*/
        limitador = (rand()%12);                                /*Probabilidad de cifrar es 1/(26-x);; siendo x un numero aletorio del rango 1-26*/
        cif = (rand()%(13-limitador));                          /*Empieza en 1 ya que mod 0 daria error*/
        texto_cifrado[i] = (((texto_plano[i]-65) + cif)%26)+65;
    }


    return texto_cifrado;
}

int repeticiones_transformado(char* texto_plano, char* texto_cifrado, char letra_plano, char letra_cifrado){

    int i = 0, repeticiones = 0;

    for(i = 0; i < nCaracteres; i++){ //Bucle que itera en el texto
        if(texto_plano[i] == letra_plano){
            if(texto_cifrado[i] == letra_cifrado){
                repeticiones++;
            }
        }
    }

    return repeticiones;
}

void estadistica(char* texto_plano, char* texto_cifrado, int* repeticiones, char* salida){

    int i = 0, j = 0;
    int* repeticiones_en_cifrado = NULL;
    double prob = 0.0;
    FILE * f_salida;
    int repeticiones_al_cifrar_con = 0;

    f_salida = fopen(salida, "w");
    if(!f_salida){
        return;
    }

    for(i = 0; i < 26; i++){
        prob = (double)repeticiones[i]/nCaracteres;
        printf("Pp(%c) = %.3lf\n", i+65, prob);
        fprintf(f_salida, "Pp(%c) = %.3lf\n", i+65, prob);
    }

    for(i = 0; i < 26; i++){
        for(j = 0; j < 26; j++){
            repeticiones_al_cifrar_con = repeticiones_transformado(texto_plano, texto_cifrado, i + 65, j + 65);
            if(repeticiones_al_cifrar_con == 0){
                printf("Pp(%c|%c) = 0.000\n", i+65, j+65);
                fprintf(f_salida, "Pp(%c|%c) = 0.000\n", i+65, j+65);
            } else{
                prob = (double)repeticiones_al_cifrar_con/(double)repeticiones[i];
                printf("Pp(%c|%c) = %.3lf\n", i+65, j+65, prob);
                fprintf(f_salida, "Pp(%c|%c) = %.3lf\n", i+65, j+65, prob);
            }
        }
    }

    free(repeticiones_en_cifrado);
    fclose(f_salida);
}
