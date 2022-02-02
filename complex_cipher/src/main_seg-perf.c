/**
 * Main encargado de comprobar
 * la seguridad perfecta del cifrado
 * por desplazamiento
 *
 * Autor: Alberto Garcia Redero
 * Version: 1.0
 *
 * Fecha: 10/11/2018
 */
#include "../include/seg-perf.h"
#include "../include/parser.h"

int nCaracteres = 0;

int main (int argc,char *argv[]) {

    char buf_entrada[TAM_FICH];
    char entrada[TAM_NOMBRE_FICH];
    char salida[TAM_NOMBRE_FICH];
    char* faux = "f_aux.txt";
    char* faux2 = "f.txt";
    char* buf = NULL;
    char* buf_cifrado = NULL;
    FILE * f_entrada;
    int* prob = NULL;


    if(argc != MAX_ARG && argc!=AYUDA){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./seg-perf -h.\n");
        return (0);
    }

    if(argc == AYUDA && strcmp(argv[1],"-h") == 0){
        printf("Ejemplo:\n\t./seg-perf {-P | -I} [-i filein] [-o fileout]\n\n");
        printf("OPCION\t\t\tDESCRIPCION\n");
        printf("-P\t\t\tSe utiliza el metodo equiprobable\n");
        printf("-I\t\t\tNo se utiliza el metodo equiprobable\n");
        printf("-i\t\t\tfichero de entrada\n");
        printf("-o\t\t\tfichero de salida\n");
        return(0);

    } else if (argc == AYUDA  && (strcmp(argv[1],"-P") == 0 || strcmp(argv[1],"-I") == 0)){
        printf("Introduzca un texto de entrada:\n");
        fgets(buf_entrada, TAM_FICH, stdin);
        printf("El resultado se guardara en salida.txt\n");
        strcpy(salida, "salida.txt");

        strcpy(entrada, faux2);
        f_entrada = fopen (entrada, "w");
        fprintf(f_entrada, "%s", buf_entrada);
        fclose(f_entrada);

    } else if(argc == MAX_ARG){
        strcpy(entrada, argv[3]);
        strcpy(salida, argv[5]);
    }

    limpiar_archivo(entrada, faux);

    f_entrada = fopen (faux, "r");

    fseek(f_entrada,0,SEEK_END);                                            /*Guardamos el tamano del fichero*/
    nCaracteres = ftell(f_entrada);

    buf = (char*)malloc(sizeof(char)*nCaracteres+1);                        /*buf contiene el texto sin cifrar*/
    if(!buf){
        free(prob);
        fclose(f_entrada);
        remove(faux2);
        remove(faux);
        return (1);
    }

    fseek(f_entrada, 0, SEEK_SET);
    if(fread(buf, sizeof(char), nCaracteres, f_entrada)==0){                         /*Guardamos el texto en el buffer*/
        free(prob);
        fclose(f_entrada);
        remove(faux2);
        remove(faux);
        free(buf);
        return (1);
    }

    fclose(f_entrada);

    prob = repeticiones_en_texto(buf);
    if(!prob){
        free(prob);
        free(buf);
        remove(faux2);
        remove(faux);
        return (1);
    }

    if(strcmp(argv[1],"-P") == 0){
        buf_cifrado = cifrado_equiprobable(buf);

    } else if(strcmp(argv[1],"-I") == 0){
        buf_cifrado = cifrado_no_equiprobable(buf);

    } else{
        printf("Introduzca una opción correcta\n");
        free(prob);
        free(buf);
        remove(faux2);
        remove(faux);
        return 0;
    }

    estadistica(buf, buf_cifrado, prob, salida);

    free(buf_cifrado);
    free(prob);
    free(buf);
    remove(faux2);       
    remove(faux);

    return (0);
}

