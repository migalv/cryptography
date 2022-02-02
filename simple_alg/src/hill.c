/*
 * Modulo dedicado al cifrado de Hill
 *
 * Modulo: hill.c
 * Autor: Miguel Alvarez Lesmes
 */

#include "../includes/hill.h"



/****************************************************************
 * Funcion que implementa el algoritmo de cifrado Hill. Recibe
 * un texto plano, que será cifrado, dada la clave alojada en la
 * matrizK.
 *
 * Function: cifrar_hill
 * Arguments: [0]-char* --> Texto a cifrar
 *            [1]-int --> Tamanio de la matrizK
 *            [2]-int** --> Matriz clave
 * Return: char* Texto cifrado
 ****************************************************************/
char* cifrar_hill(char* texto_plano, int dimensionK, int** matrizK){

    int i = 0, j = 0, k = 0;
    // Mcd de det y modulo
    int mcd = 0;
    // Modulo
    int modulo = 0;
    // Inverso en Zm de los 2 primeros parametros de euclides
    int inverso1 = 0 , inverso2 = 0;
    // Determinante de la matrizK
    int det = 0;
    // Texto cifrado representado en enteros (Mayor facilidad)
    int* pTexto_cifrado = NULL;
    // Texto cifrado definitivo
    char* texto_cifrado = NULL;
    // Longitud del texto_plano
    int length = 0;

    if(texto_plano == NULL || dimensionK < 0 || matrizK == NULL){
        fprintf(stderr, "Error: Parametros incorrectos de la función cifrar.\n");
        return NULL;
    }

    det = determinante(matrizK, dimensionK);

    modulo = get_modulo();

    euclides(det, modulo , &mcd, &inverso1, &inverso2);

    if(mcd != 1){
        printf("La matriz no es inyectiva.\nNo se puede realizar el cifrado.\n");
        return NULL;
    }

    texto_plano = padding(texto_plano, dimensionK);

    if(texto_plano == NULL)
      return NULL;

    length = strlen(texto_plano);

    pTexto_cifrado = (int*)malloc(sizeof(int)*length);
    if(pTexto_cifrado == NULL){
      fprintf(stderr, "Error[645]: Al reservar memoria para el texto cifrado.\n");
      return NULL;
    }

    // Aqui se realiza el cifrado
    for(i = 0; i < length-1; i+=dimensionK){
      for(j = 0; j < dimensionK; j++){
          pTexto_cifrado[i+j] = 0;
          for(k = 0; k < dimensionK; k++){
              pTexto_cifrado[i+j] = pTexto_cifrado[i+j] + ((int)texto_plano[i+k]-DESPLAZADOR_ASCII) * matrizK[j][k];
          }
          pTexto_cifrado[i+j] %= get_modulo();
          pTexto_cifrado[i+j] += DESPLAZADOR_ASCII;
      }
    }

    texto_cifrado = (char*)malloc(sizeof(char)*length+1);
    if(texto_cifrado == NULL){
        fprintf(stderr, "Error[645]: Al reservar memoria para el texto cifrado.\n");
        free(pTexto_cifrado);
        return NULL;
    }
    // Aqui se pasan los caracteres en forma ASCII (int) a char
    for (i = 0 ; i < length ; i++){
        texto_cifrado[i] = pTexto_cifrado[i];
    }

    texto_cifrado[i] = '\0';

    free(pTexto_cifrado);
    free(texto_plano);

    return texto_cifrado;
}

/****************************************************************
 * Funcion que implementa el algoritmo de descifrado Hill. Recibe
 * un texto plano, que será descifrado, dada la clave alojada en la
 * matrizK.
 *
 * Function: descifrar_hill
 * Arguments: [0]-char* --> Texto a descifrar
 *            [1]-int --> Tamanio de la matrizK
 *            [2]-int** --> Matriz clave
 * Return: char* Texto descifrado
 ****************************************************************/
char* descifrar_hill(char* texto_cifrado, int dimensionK, int** matrizK){

    int i = 0, j = 0, k = 0;
    // Mcd de det y modulo
    int mcd = 0;
    // Inverso en Zm de los 2 primeros parametros de euclides
    int inverso1 = 0 , inverso2 = 0;
    // Determinante de la matrizK
    int det = 0;
    // Texto descifrado representado en enteros (Mayor facilidad)
    int* pTexto_descifrado = NULL;
    // Texto descifrado definitivo
    char* texto_descifrado = NULL;
    // Longitud del texto_plano
    int length = 0;
    // Matriz inversa, utilizada para descifrar
    int** mat_inversa = NULL;
    // Tamaño del alfabeto (representa el modulo)
    int modulo = 0;

    if(texto_cifrado == NULL || dimensionK < 0 || matrizK == NULL){
        fprintf(stderr, "Error: Parametros incorrectos de la función descifrar.\n");
        return NULL;
    }

    det = determinante(matrizK, dimensionK);

    modulo = get_modulo();

    euclides(det, modulo , &mcd, &inverso1, &inverso2);

    if(mcd != 1){
        printf("La matriz no es inyectiva.\nNo se puede realizar el descifrado.\n");
        return NULL;
    }else{

        texto_cifrado = padding(texto_cifrado, dimensionK);

        length = strlen(texto_cifrado);

        pTexto_descifrado = (int*)malloc(sizeof(int)*length);
        if(pTexto_descifrado == NULL){
            fprintf(stderr, "Error[645]: Al reservar memoria para el texto descifrado.\n");
            return NULL;
        }

        mat_inversa = matriz_inversa(matrizK, dimensionK, det);

        // Aqui se realiza el descifrado
        for(i = 0; i < length; i+=dimensionK){
            for(j = 0; j < dimensionK; j++){
                pTexto_descifrado[i+j] = 0;
                for(k = 0; k < dimensionK; k++){
                    pTexto_descifrado[i+j] = pTexto_descifrado[i+j] + ((int)texto_cifrado[i+k]-DESPLAZADOR_ASCII) * mat_inversa[j][k];
                }
                pTexto_descifrado[i+j] %= get_modulo();
                pTexto_descifrado[i+j] += DESPLAZADOR_ASCII;
            }
        }
    }

    texto_descifrado = (char*)malloc(sizeof(char)*length+1);
    if(texto_descifrado == NULL){
        fprintf(stderr, "Error[645]: Al reservar memoria para el texto cifrado.\n");
        free(pTexto_descifrado);
        return NULL;
    }
    // Aqui se pasan los caracteres en forma ASCII (int) a char
    for (i = 0 ; i < length ; ++i)    {
        texto_descifrado[i] = pTexto_descifrado[i];
    }

    texto_descifrado[i] = '\0';

    free(pTexto_descifrado);
    free(texto_cifrado);

    for(i = 0; i < dimensionK; i++){
        free(mat_inversa[i]);
    }
    free(mat_inversa);

    return texto_descifrado;
}

/****************************************************************
 * Funcion utilizada para introducir "X" al final de un bloque,
 * en caso de que éste no sea múltiplo del tamaño de la matriz.
 *
 * Function: padding
 * Arguments: [0]-char* --> Texto a añadir las "X"
 *            [1]-int --> Tamanio de la matrizK
 * Return: char* El texto con las "X" al final
 ****************************************************************/
char* padding(char* text, int tamanio){

    int i = 0, padding = 0;
    char* tmp = NULL;

    int length = (int)strlen(text);

    if((length % tamanio) != 0){
        padding = tamanio - (length % tamanio);

        tmp = (char*)realloc(text, (length+padding+1)*sizeof(char));
        if(tmp == NULL){
            fprintf(stderr, "Error: Al reallocar memoria para el padding.\n");
            return NULL;
        }
        text = tmp;
        for(i = 0; i < padding; i++){
            strcat(text, PADDING);
        }
    }

    return text;
}

int comprueba_inyectividad(int* permutacion, int tamanio){

    int i = 0, j = 0;
    int* permutacion_iny = NULL;
    int* permutacion_aux = NULL;

    if(permutacion == NULL){
        fprintf(stderr, "Error: Parametros incorrectos en la funcion comprueba_inyectividad.\n");
        return -1;
    }

    permutacion_iny = (int*)malloc(sizeof(int)*tamanio);
    permutacion_aux = (int*)malloc(sizeof(int)*tamanio);

    for(i = 0; i < tamanio; i++){
        permutacion_iny[i] = i+1;
    }

    for(i = 0; i < tamanio; i++){
        if(permutacion[i] > tamanio || permutacion[i] <= 0){
            free(permutacion_iny);
            free(permutacion_aux);
            return -1;
        }
        for(j = 0; j < tamanio; j++){
            if(permutacion_iny[i] == permutacion[j]){
                permutacion_aux[i] = permutacion[j];
                break;
            }
        }
        if(permutacion_iny[i] != permutacion_aux[i]){
            free(permutacion_iny);
            free(permutacion_aux);
            return -1;
        }
    }

    free(permutacion_iny);
    free(permutacion_aux);
    return 1;
}
