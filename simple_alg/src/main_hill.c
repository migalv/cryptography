/**
 * Main encargado de mostrar el funcionamiento
 * del cifrado de hill
 *
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 18/10/2018
 */

#include "../includes/hill.h"

// Caso de ayuda
#define AYUDA 2
// Maximo de argumentos de entrada
#define MAX_ARG 9
// Minimo de argumentos de entrada
#define MIN_ARG 5
// Una opcion
#define ONEOPTION 7

int main(int argc, char* argv[]) {

    /* Nombre del archivo que contiene la matriz de la clave K */
    char fileK[20] = "" ;
    // Tamanio de la matriz
    int dimensionK = 0;
    // Flag para saber si cifrar o descifrar (Cifrar = 1 // Descifrar = 0)
    int flag_cifrar = 1;
    // Archivo de entrada
    FILE * infile = NULL;
    // Archivo de salida
    FILE * outfile = NULL;
    int i = 0;
    // Matriz obtenida del archivo
    int** matriz = NULL;
    // Cadena de caracteres
    char* texto = NULL;
    // Estructura que permite conocer el tamaño de los archivos
    struct stat st;
    // Tamaño de los archivos
    int size = 0;
    // Variables que recogen el nombre de los archivos de entrada y salida
    char* infilename = NULL; char* outfilename = NULL;

    // Caso de que el numero de argumentos no sea el correcto
    if((argc < MIN_ARG || argc > MAX_ARG) && argc != AYUDA){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./hill -h.\n");
        return -1;
    }

    // Mostrar ayuda
    if(argc == AYUDA && strcmp(argv[1],"-h") == 0){
        printf("Ejemplo:\n\nhill {-C|-D} {-m} {-n} {-k}");
        printf("[-i filein] [-o fileout]\n\n");
        printf("OPCION\t\t\tDESCRIPCION\n");
        printf("-C\t\t\tEl programa cifra\n");
        printf("-D\t\t\tEl programa descifra\n");
        printf("-m\t\t\tCardinalidad de Zm\n");
        printf("-n\t\t\tDimension de la matriz de transformacion\n");
        printf("-k\t\t\tFichero que contiene la matriz de transformacion\n");
        printf("-i filein\t\tFichero de entrada. En caso de no especificarse se pedirá por pantalla el texto a cifrar\n");
        printf("-o fileout\t\tFichero de salida. En caso de no especificarse se pedirá por pantalla el texto a descifrar\n\n");
        printf("  Ejemplos de uso:\n\n");
        printf("\thill -C 26 3 matriz_transformacion.txt\n");
        printf("\thill -D 26 3 matriz_transformacion.txt -o fichero_descifrado.txt\n");
        printf("\thill -C 26 3 matriz_transformacion.txt -i fichero_a_cifrar.txt -o fichero_cifrado.txt\n");
        return -1;
    }

    // Comprobamos si queremos cifrar o descifrar
    if(!strcmp(argv[1], "-C")){
        flag_cifrar = 1;
    }else if(!strcmp(argv[1], "-D")){
        flag_cifrar = 0;
    }else{
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./hill -h.\n");
        return -1;
    }

    // Recuperamos los parametros introducidos por terminal
    if(argv[2] == NULL || argv[3] == NULL || argv[4] == NULL){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./hill -h.\n");
        return -1;
    }else{
        if(atoi(argv[2]) < 0 || atoi(argv[3]) < 0){
            printf("Introduzca correctamente los argumentos.\n");
            printf("Para ver la ayuda ./hill -h.\n");
            return -1;
        }
        set_modulo(atoi(argv[2]));
        dimensionK = atoi(argv[3]);
        strcpy(fileK, argv[4]);
    }

    // Leemos la matriz que contiene la clave para cifrar
    if((matriz = lee_matriz(fileK, dimensionK)) == NULL){
        fprintf (stderr, "Error: Al leer la matriz del archivo %s.\n", fileK);
        return -1;
    }

    // Comprobamos si se le pasa un archivo o leemos de terminal
    if((argc == ONEOPTION) || (argc == MAX_ARG))
        if(!strcmp(argv[5], "-i"))
            infilename = argv[6];

    if((argc == MAX_ARG))
        if(!strcmp(argv[7], "-i"))
            infilename = argv[8];

    if(infilename){

        limpiar_archivo(infilename, "texto_limpio.txt");

        if((infile = fopen("texto_limpio.txt", "r")) == NULL){
            fprintf (stderr, "Error: Al leer el archivo texto_limpio.txt.\n");
            for(i = 0; i < dimensionK; i++)
                free(matriz[i]);
            free(matriz);
            return -1;
        }

        // Obtenemos el tamaño del archivo
        stat(infilename, &st);
        size = (int)st.st_size;

        i = 0;
        texto = (char*)malloc(size * sizeof(char) + 1);
        if(texto == NULL){
            fprintf(stderr, "Error[133]: Al reservar memoria para el texto.\n");
            fclose(infile);
            for(i = 0; i < dimensionK; i++)
                free(matriz[i]);
            free(matriz);
            return -1;
        }

        // Recopilamos los datos del archivo
        while((texto[i] = fgetc(infile)) != EOF)
            i++;

        texto[i] = '\0';
        fclose(infile);

        remove("texto_limpio.txt");
    }

    else{
        // Caso en el que se ha elegido cifrar el texto por pantalla
        if(flag_cifrar)
            printf("\nIntroduzca el texto que desea cifrar (hasta %d caracteres)\n > ", MAX_TEXTO-1);
        // Caso en el que se ha elegido descifrar el texto por pantalla
        else
            printf("\nIntroduzca el texto que desea descifrar (hasta %d caracteres)\n > ", MAX_TEXTO-1);

        texto = (char*)malloc(MAX_TEXTO * sizeof(char));
        if(texto == NULL){
            fprintf(stderr, "Error[133]: Al reservar memoria para el texto.\n");
            for(i = 0; i < dimensionK; i++)
                free(matriz[i]);
            free(matriz);
            return -1;
        }

        // Lees el texto de pantalla
        fgets(texto, MAX_TEXTO, stdin);

        // Guardas el texto en un archivo para poder limpiarlo
        infile = fopen("texto_sucio.txt", "w");
        fprintf(infile, "%s", texto);
        fclose(infile);

        // Limpias el archivo y creas el archivo texto_limpio.txt
        limpiar_archivo("texto_sucio.txt", "texto_limpio.txt");

        // Abres el archivo texto_limpio.txt y lo lees para cifrarlo.
        infile = fopen("texto_limpio.txt", "r");

        // Recopilamos los datos del archivo
        while((texto[i] = fgetc(infile)) != EOF)
            i++;

        texto[i] = '\0';
        fclose(infile);
        remove("texto_sucio.txt");
        remove("texto_limpio.txt");
    }

    if(flag_cifrar){
        texto = cifrar_hill(texto, dimensionK, matriz);
        if(texto == NULL){
            fprintf(stderr, "Error: Al cifrar el texto.\n");
            for(i = 0; i < dimensionK; i++)
                free(matriz[i]);
            free(matriz);
            return -1;
        }
    }else{
        texto = descifrar_hill(texto, dimensionK, matriz);
        if(texto == NULL){
            fprintf(stderr, "Error: Al descifrar el texto.\n");
            for(i = 0; i < dimensionK; i++)
                free(matriz[i]);
            free(matriz);
            return -1;
        }
    }

    // Comprobamos si se le pasa un archivo o leemos de terminal
    if((argc == ONEOPTION) || (argc == MAX_ARG))
        if(!strcmp(argv[5], "-o"))
            outfilename = argv[6];

    if(argc == MAX_ARG)
        if(!strcmp(argv[7], "-o"))
            outfilename = argv[8];

    if(outfilename){
        outfile = fopen(outfilename, "w");
        fprintf(outfile, "%s", texto);
        fclose(outfile);
    }
    else{
        if(flag_cifrar)
            printf("\nTexto cifrado\n > %s\n", texto);
        else
            printf("\nTexto descifrado\n > %s\n", texto);
    }

    free(texto);

    for(i = 0; i < dimensionK; i++)
        free(matriz[i]);
     free(matriz);

    return 1;
}
