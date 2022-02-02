/**
 * Modulo encargado de las operaciones con matrices cuadradas
 *
 * Modulo: matriz.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 15/10/2018
 */

#include "../includes/matriz.h"

/****************************************************************
 * Lee una matriz cuadrada de un archivo, reserva memoria para
 * ella y la devuelve como int**. El archivo debe estar
 * formateado de la siguiente manera :
 *
 *                          |%d %d %d|
 *                          |%d %d %d|
 *                          |%d %d %d|
 *
 * Function: lee_matriz
 * Arguments: [0]-char* --> Nombre del archivo a leer
 *            [1]-int --> Tamanio de la matriz cuadrada
 * Return: int** La matriz cuadrada formateada
 ****************************************************************/
int** lee_matriz(char* filename, int tamanio){

    int i = 0, j = 0;

    // Archivo que contiene la matriz
    FILE * fmatriz = NULL;
    // Entero recuperado del archivo
    int num = 0;
    // Identificadores para refenciarse a
    // las columnas y filas de la matriz
    int fila = 0, columna = 0;
    // Matriz obtenida del archivo
    int **matriz = NULL;

    if(tamanio < 0){
        fprintf(stderr, "Error: Parametros incorrectos en la funcion 'lee_matriz'.\n");
        return NULL;
    }

    // Matriz cuadrada de tamanio "tamanio"
    matriz = (int **)malloc(tamanio * sizeof(int*));
    if(matriz == NULL){
        fprintf(stderr, "Error[matriz47]: Al reservar memoria para la matriz 'matriz'.\n");
        return NULL;
    }
    for(i = 0; i < tamanio; i++){
        matriz[i] = (int *)malloc(tamanio * sizeof(int));
        if(matriz[i] == NULL){
            fprintf(stderr, "Error[matriz53]: Al reservar memoria para la matriz 'matriz'.\n");
            for(j = 0; j < i; j++)
                free(matriz[j]);
            free(matriz);
            return NULL;
        }
    }

    // Caso de algun problema al abrir el archivo
    if((fmatriz = fopen (filename, "r")) == NULL) {
        fprintf (stderr, "Error: Al abrir el archivo '%s'.\n", filename);
        for(i = 0; i < tamanio; i++)
            free(matriz[i]);
        free(matriz);
        return NULL;
    }

    // Bucle que se encarga de leer el archivo y rellenar la matriz
    while(fila < tamanio){

        fscanf(fmatriz, "%d", &num);

        if((num >= get_modulo()) || (num < 0)){
            fprintf(stderr, "Error: La matriz no está en forma modular.\n");
            return NULL;
        }

        matriz[fila][columna] = num;
        columna++;

        if(columna == tamanio){
            columna = 0;
            fila++;
        }

    }

    // Caso en el que se le ha pasado unas dimensiones de
    // matriz incorrectas. O el archivo está mal formateado
    if(fscanf(fmatriz, "%d", &num) != EOF){
        fprintf (stderr, "Error: Dimensiones de matriz incorrectas o archivo mal formateado.\n");
        return NULL;
    }

    fclose(fmatriz);

    return matriz;

}

/****************************************************************
 * Calcula el determinante en forma modular de una matriz cuadrada.
 *
 * Function: determinante
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 * Return: int el determinante de la matriz
 ****************************************************************/
int determinante(int **matriz, int tamanio){

    // Matriz de tamanio-1
    int** menor = NULL;
    int i=0,j=0,k=0,c1=0,c2=0;
    int determinant=0;
    int O=1;

    if(matriz == NULL || tamanio < 0){
        fprintf(stderr, "Error: Argumentos incorrectos en la función \"det()\".\n");
        return -1;
    }

    determinant = 0;

    menor = (int**)malloc((tamanio) *  sizeof(int*));

    if(menor == NULL){
        fprintf(stderr, "Error[matriz130]: Al reservar memoria para la matriz menor.\n");
        return -1;
    }

    for(i = 0; i < tamanio; i++){
        menor[i] = (int*)malloc((tamanio) * sizeof(int));
        if(menor[i] == NULL){
            fprintf(stderr, "Error[matriz135]: Al reservar memoria para la matriz menor.\n");
            for(j = 0; j < i; j++)
                free(menor[j]);
            free(menor);
            return -1;
        }
    }

    if(tamanio == 1){
        determinant = matriz[0][0];
    }
    if(tamanio == 2){
        determinant = res_mod(mul_mod(matriz[0][0], matriz[1][1]), mul_mod(matriz[0][1], matriz[1][0]));
    }
    else{
        for(i = 0 ; i < tamanio ; i++){
            c1 = 0, c2 = 0;

            for(j = 0 ; j < tamanio ; j++){
                for(k = 0 ; k < tamanio ; k++){
                    if(j != 0 && k != i){

                        menor[c1][c2] = matriz[j][k];
                        c2++;
                        if(c2>tamanio-2){

                            c1++;
                            c2=0;
                        }
                    }
                }
            }
            determinant = sum_mod(determinant, mul_mod(O, (mul_mod(matriz[0][i], determinante(menor, tamanio-1)))));
            O = mul_mod(-1, O);
        }
    }

    for(i = 0; i < tamanio; i++)
        free(menor[i]);

    free(menor);

    return determinant;
}

/****************************************************************
 * Calcula la matriz menor de una matriz cuadrada, dada una fila
 * y una columna. Ejemplo:
 *              matriz      ==> matriz menor fila = 1 // col = 1
 *             %d %d %d           X  X  X
 *             %d %d %d           X %d %d
 *             %d %d %d           X %d %d
 *
 * Function: matriz_menor
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 *            [2]-int --> Fila a eliminar
 *            [3]-int --> Columna a eliminar
 * Return: int** La matriz menor, de tamanio = tamanio-1
 ****************************************************************/
int** matriz_menor(int** matriz, int tamanio, int fila, int col){
    int i=0, j=0;
    int** menor = NULL;

    if(matriz == NULL || tamanio < 0){
        fprintf(stderr, "Error: En los argumentos de la función matriz menor.\n");
        return NULL;
    }

    menor = (int**)malloc((tamanio-1) * sizeof(int*));

    if(menor == NULL){
        fprintf(stderr, "Error[matriz207]: Al reservar memoria para la matriz menor.\n");
        return NULL;
    }

    for(i = 0; i < tamanio-1; i++){
        menor[i] = (int*)malloc((tamanio-1) * sizeof(int));
        if(menor[i] == NULL){
            fprintf(stderr, "Error[matriz215]: Al reservar memoria para la matriz menor.\n");
            for(j = 0; j < i; j++)
                free(menor[j]);
            free(menor);
            return NULL;
        }
    }

    for(i = 0; i < tamanio; i++){
        for(j = 0; i != fila && j < tamanio; j++){
            if (j != col){
                if(i < fila){
                    if(j < col)
                        menor[i][j] = matriz[i][j];
                    else
                        menor[i][j-1] = matriz[i][j];
                }
                else{
                    if(j < col)
                        menor[i-1][j] = matriz[i][j];
                    else
                        menor[i-1][j-1] = matriz[i][j];
                }
            }
        }
    }


    return menor;
}

/****************************************************************
 * Calcula en forma modular la matriz inversa, de una matriz
 * cuadrada.
 *
 * Function: matriz_inversa
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 *            [2]-int --> Determinante de la matriz cuadrada
 * Return: int** La matriz inversa en forma modular
 ****************************************************************/
int** matriz_inversa(int** matriz, int tamanio, int determinante){

    int i = 0, j = 0;
    int** mat_inversa = NULL;
    int** mat_cofactor = NULL;

    mat_cofactor = matriz_cofactor(matriz, tamanio);

    mat_inversa = (int**)malloc(tamanio * sizeof(int*));
    if(mat_inversa == NULL){
        fprintf(stderr, "Error[matriz266]: Al reservar memoria para la matriz inversa.\n");
        return NULL;
    }

    for(i = 0; i < tamanio; i++){
        mat_inversa[i] = (int*)malloc(tamanio * sizeof(int));
        if(mat_inversa[i] == NULL){
            fprintf(stderr, "Error[matriz273]: Al reservar memoria para la matriz inversa.\n");
            for(j = 0; j < i; j++)
                free(mat_inversa[j]);
            free(mat_inversa);
            return NULL;
        }

        for (j = 0; j < tamanio; ++j){
            // Hacemos la traspuesta a la vez que dividimos por el determinante
            mat_inversa[i][j] = div_mod(mat_cofactor[j][i], determinante);
        }
    }

    for(i = 0; i < tamanio; i++)
        free(mat_cofactor[i]);
    free(mat_cofactor);

    return mat_inversa;
}

/****************************************************************
 * Printea una matriz por pantalla
 *
 * Function: print_matriz
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Numero de filas de la matriz
 *            [2]-int --> Numero de columnas de la matriz
 * Return: VOID
 ****************************************************************/
void print_matriz(int** matriz, int columnas, int filas){
    int i = 0, j = 0;
    for(i = 0; i < filas; i++){
        for (j = 0; j < columnas; j++){
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

/****************************************************************
 * Calcula la matriz cofactor de una matriz cuadrada
 *
 * Function: matriz_cofactor
 * Arguments: [0]-int** --> Matriz cuadrada
 *            [1]-int --> Tamanio de la matriz cuadrada
 * Return: int** La matriz cofactor en forma modular
 ****************************************************************/
int** matriz_cofactor(int** matriz, int tamanio){

    int fila = 0, col = 0, i = 0, j = 0;
    int** matriz_cofactor = NULL;
    int** menor = NULL;
    if(matriz == NULL || tamanio < 0){
        fprintf(stderr, "Error: En los argumentos de la función matriz de cofactores.\n");
        return NULL;
    }

    matriz_cofactor = (int**)malloc((tamanio) * sizeof(int*));

    if(matriz_cofactor == NULL){
        fprintf(stderr, "Error[matriz332]: Al reservar memoria para la matriz matriz de cofactores.\n");
        return NULL;
    }

    for(i = 0; i < tamanio; i++){
        matriz_cofactor[i] = (int*)malloc((tamanio) * sizeof(int));
        if(matriz_cofactor[i] == NULL){
            fprintf(stderr, "Error[matriz340]: Al reservar memoria para la matriz matriz de cofactores.\n");
            for(j = 0; j < i; j++)
                free(matriz_cofactor[j]);
            free(matriz_cofactor);
            return NULL;
        }
    }

    for(fila = 0; fila < tamanio; fila++){
        for(col = 0; col < tamanio; col++){
            menor = matriz_menor(matriz, tamanio, fila, col);

            if(menor == NULL){
                fprintf(stderr, "Error: Al coger la matriz menor.\n");
                for(i = 0; i < tamanio; i++)
                    free(matriz_cofactor[i]);
                free(matriz_cofactor);
                return NULL;
            }

            matriz_cofactor[fila][col] = determinante(menor, tamanio-1);

            for(i = 0; i < tamanio-1; i++)
                free(menor[i]);
            free(menor);

            // Aqui se multiplican los elemento de la matriz por - donde la suma de fila + columna
            // es impar
            if((fila +  col) % 2)
                matriz_cofactor[fila][col] = mul_mod(matriz_cofactor[fila][col], -1);
        }
    }

    return matriz_cofactor;
}

/****************************************************************
 * Dada una permutacion, te genera una matriz identidad que la
 * representa.
 *
 * Function: matriz_identidad
 * Arguments: [0]-int* --> Permutacion
 *            [1]-int --> Tamanio de la permutacion
 * Return: int** La matriz identidad de la permutacion
 ****************************************************************/
int** matriz_identidad(int* permutacion, int num){

    int i = 0, j = 0;
    int** mat_identidad = NULL;

    if(permutacion == NULL || num < 0){
        fprintf(stderr, "Error: Parametros incorrectos de la función matriz_identidad.\n");
        return NULL;
    }

    mat_identidad = (int**)malloc(sizeof(int*) * num);
    if(mat_identidad == NULL){
        fprintf(stderr, "Error[645]: Al reservar memoria para la matriz_identidad.\n");
        return NULL;
    }

    for(i = 0; i < num; i++){
        mat_identidad[i] = (int *)malloc(num * sizeof(int));
        if(mat_identidad[i] == NULL){
            fprintf(stderr, "Error[matriz53]: Al reservar memoria para la mat_identidad.\n");
            for(j = 0; j < i; j++)
                free(mat_identidad[j]);
            free(mat_identidad);
            return NULL;
        }
    }

    for(i = 0; i < num; i++){
        for(j = 0; j < num; j++){
            if(j == (permutacion[i]-1))
                mat_identidad[j][i] = 1;
            else
                mat_identidad[j][i] = 0;
        }
    }

    return mat_identidad;
}
