/**
 * Main encargado de mostrar el funcionamiento
 * del cifrado de doble permutación
 *
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 22/10/2018
 */

#include "../includes/permutacion.h"
#include "../includes/modular.h"
#include "../includes/parser.h"

// Maximo de argumentos de entrada
#define MAX_ARG 10
// Minimo de argumentos de entrada
#define MIN_ARG 6
// Permite elegir el tamanio del idioma (26 ingles o espaniol sin enie)
#define IDIOMA 26
// Caso de ayuda
#define AYUDA 2
// Una opcion
#define ONEOPTION 8

int main(int argc, char* argv[]) {

    int i = 0;
    // Flag para saber si cifrar o descifrar (Cifrar = 1 // Descifrar = 0)
    int flag_cifrar = 1;
    // Archivo de entrada
    FILE * infile = NULL;
    // Archivo de salida
    FILE * outfile = NULL;
    // Variables utilizadas para saber el tamaño de los vectores de K1 y K2
    int num_k1 = 0, num_k2 = 0;
    // Vectores que guardan las permutaciones de K1 y K2
    int permutacionK1[MAX_ELEMS], permutacionK2[MAX_ELEMS];
    // Cadena de caracteres
    char* texto = NULL;
    // Estructura que permite conocer el tamaño de los archivos
    struct stat st;
    // Tamaño de los archivos
    int size = 0;
    // Variables que recogen el nombre de los archivos de entrada y salida
    char* infilename = NULL; char* outfilename = NULL;
    // Variable auxiliar utilizada para tokenizar la entrada de la permutacion
    char* token = NULL, *token2 = NULL;
    // Tamanio de la matriz de cifrado
    int tam_matriz = 0;

    // Caso de que el numero de argumentos no sea el correcto
    if((argc < MIN_ARG || argc > MAX_ARG) && argc != AYUDA){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./permutacion -h.\n");
        return -1;
    }

    // Mostrar ayuda
    if(argc == AYUDA && strcmp(argv[1],"-h") == 0){
        printf("Ejemplo:\n\npermutacion {-C|-D} {-k1 K1 -k2 K2}");
        printf("[-i filein] [-o fileout]\n\n");
        printf("OPCION\t\t\tDESCRIPCION\n");
        printf("-C\t\t\tEl programa cifra\n");
        printf("-D\t\t\tEl programa descifra\n");
        printf("-k1\t\t\tClave para cifrar las filas. Viene dada por una permutacion de M elementos.\n");
        printf("-k2\t\t\tClave para cifrar las columnas. Viene dada por una permutacion de N elementos.\n");
        printf("-i filein\t\tFichero de entrada. En caso de no especificarse se pedirá por pantalla el texto a cifrar\n");
        printf("-o fileout\t\tFichero de salida. En caso de no especificarse se pedirá por pantalla el texto a descifrar\n\n");
        printf("  Ejemplos de uso:\n\n");
        printf("\n\tpermutacion -C -k1 \"4 2 3 5 1\" -k2 \"3 1 4 2\"\n");
        printf("\tpermutacion -D -k1 \"4 2 3 5 1\" -k2 \"3 1 4 2\" -o fichero_descifrado.txt\n");
        printf("\tpermutacion -C -k1 \"4 2 3 5 1\" -k2 \"3 1 4 2\" -i fichero_a_cifrar.txt -o fichero_cifrado.txt\n");
        return -1;
    }

    // Comprobamos si queremos cifrar o descifrar
    if(!strcmp(argv[1], "-C")){
        flag_cifrar = 1;
    }else if(!strcmp(argv[1], "-D")){
        flag_cifrar = 0;
    }else{
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./permutacion -h.\n");
        return -1;
    }

    // Recuperamos los parametros introducidos por terminal
    if(argv[2] == NULL || argv[3] == NULL || argv[4] == NULL || argv[5] == NULL){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./permutacion -h.\n");
        return -1;
    }else{
        if(!strcmp(argv[2], "-k1") && !strcmp(argv[4], "-k2")){
            // Le decimos que idioma se va a utilizar
            set_modulo(IDIOMA);

            token = strtok(argv[3], " ");
            permutacionK1[i] = atoi(token);

            if(token != NULL){
                for(i = 1; (token = strtok(NULL, " ")) != NULL; i++){
                    permutacionK1[i] = atoi(token);
                }
                num_k1 = i;
                if(comprueba_inyectividad(permutacionK1, num_k1) == -1){
                    printf("Error: La permutacion K1 no es inyectiva.\n");
                    return -1;
                }
            }else{
                printf("Introduzca correctamente los argumentos.\n");
                printf("Para ver la ayuda ./transposicion -h.\n");
                return -1;
            }

            i = 0;
            token2 = strtok(argv[5], " ");
            permutacionK2[i] = atoi(token2);

            if(token2 != NULL){
                for(i = 1; (token2 = strtok(NULL, " ")) != NULL; i++){
                    permutacionK2[i] = atoi(token2);
                }
                num_k2 = i;
                if(comprueba_inyectividad(permutacionK2, num_k2) == -1){
                    printf("Error: La permutacion K2 no es inyectiva.\n");
                    return -1;
                }
            }else{
                printf("Introduzca correctamente los argumentos.\n");
                printf("Para ver la ayuda ./transposicion -h.\n");
                return -1;
            }
        }
        else{
            printf("Introduzca correctamente los argumentos.\n");
            printf("Para ver la ayuda ./permutacion -h.\n");
            return -1;
        }
    }
    tam_matriz = num_k1*num_k2;
    // Comprobamos si se le pasa un archivo o leemos de terminal
    if((argc == ONEOPTION) || (argc == MAX_ARG))
        if(!strcmp(argv[MIN_ARG], "-i"))
            infilename = argv[MIN_ARG+1];

    if((argc == MAX_ARG))
        if(!strcmp(argv[MAX_ARG-2], "-i"))
            infilename = argv[MAX_ARG-1];

    // En caso de existir un archivo lo limpiamos
    if(infilename){
        limpiar_archivo(infilename, "texto_limpio.txt");

        if((infile = fopen("texto_limpio.txt", "r")) == NULL){
            fprintf (stderr, "Error: Al leer el archivo texto_limpio.txt.\n");
            return -1;
        }

        // Obtenemos el tamaño del archivo
        stat(infilename, &st);
        size = (int)st.st_size;

		size = size + (tam_matriz-size%tam_matriz);

        texto = (char*)malloc(size * sizeof(char) + 1);
        if(texto == NULL){
            fprintf(stderr, "Error[133]: Al reservar memoria para el texto.\n");
            fclose(infile);
            return -1;
        }
        i = 0;
        // Recopilamos los datos del archivo
        while((texto[i] = fgetc(infile)) != EOF)
            i++;

        texto[i] = '\0';
        fclose(infile);

        remove("texto_limpio.txt");
    }
    else{
        // Aqui se recalcula el tamaño que se va a reservar, respecto al tamaño
        // de la matriz, para que sea multiplo
        size = MAX_TEXTO + (tam_matriz-MAX_TEXTO%tam_matriz);
        // Caso en el que se ha elegido cifrar el texto por pantalla
        if(flag_cifrar)
            printf("\nIntroduzca el texto que desea cifrar (hasta %d caracteres)\n > ", size-1);
        // Caso en el que se ha elegido descifrar el texto por pantalla
        else
            printf("\nIntroduzca el texto que desea descifrar (hasta %d caracteres)\n > ", size-1);

        texto = (char*)malloc(size * sizeof(char));
        if(texto == NULL){
            fprintf(stderr, "Error[133]: Al reservar memoria para el texto.\n");
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

        i = 0;
        // Recopilamos los datos del archivo
        while((texto[i] = fgetc(infile)) != EOF)
            i++;

        texto[i] = '\0';
        fclose(infile);
        remove("texto_sucio.txt");
        remove("texto_limpio.txt");
    }

    // Hacemos padding a nuestro texto para que sea multiplo del tamanio de la
    // matriz
    texto = padding(texto, tam_matriz);

    if(flag_cifrar){
        texto = cifrar_doble(texto, permutacionK1, permutacionK2, num_k1, num_k2);
        if(texto == NULL){
            fprintf(stderr, "Error: Al cifrar el texto.\n");
            return -1;
        }
    }else{
        texto = descifrar_doble(texto, permutacionK1, permutacionK2, num_k1, num_k2);
        if(texto == NULL){
            fprintf(stderr, "Error: Al descifrar el texto.\n");
            return -1;
        }
    }

    // Comprobamos si se le pasa un archivo o leemos de terminal
    if((argc == ONEOPTION) || (argc == MAX_ARG))
        if(!strcmp(argv[MIN_ARG], "-o"))
            outfilename = argv[MIN_ARG+1];

    if(argc == MAX_ARG)
        if(!strcmp(argv[MAX_ARG-2], "-o"))
            outfilename = argv[MAX_ARG-1];

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

    return 1;
}
