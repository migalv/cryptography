/*
 * Modulo dedicado al cifrado por transposicion
 * basado en el cifrado de hill de forma general
 *
 * Modulo: transposicion.c
 * Autor: Miguel Alvarez Lesmes
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>

#include "../includes/hill.h"


// Maximo de argumentos de entrada
#define MAX_ARG 7
// Minimo de argumentos de entrada
#define MIN_ARG 3
// Caso de ayuda
#define AYUDA 2
// Una opcion
#define ONEOPTION 5
// Tamanio maximo de la permutacion
#define MAX_ELEMS 2000
// Permite elegir el tamanio del idioma (26 ingles o espaniol sin enie)
#define IDIOMA 26

int main(int argc, char* argv[]) {

	int i = 0;
    // Flag para saber si cifrar o descifrar (Cifrar = 1 // Descifrar = 0)
    int flag_cifrar = 1;
    // Archivo de entrada
    FILE * infile = NULL;
    // Archivo de salida
    FILE * outfile = NULL;
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
    // Variable auxiliar para tokenizar la entrada
    char* token = NULL;
    // Permutacion introducida por pantalla
    int permutacion[MAX_ELEMS];
    // Numero de elementos de la permutacio
    int num_elem = 0;
    // Variable auxiliar
    int foobar = 0;
    // Indice generado aleatoriamente utilizado para generar la permutacion
    int random_index = 0;

    // Caso de que el numero de argumentos no sea el correcto
    if((argc < MIN_ARG || argc > MAX_ARG) && argc != AYUDA){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./transposicion -h.\n");
        return -1;
    }

    // Mostrar ayuda
    if(argc == AYUDA && strcmp(argv[1],"-h") == 0){
        printf("Ejemplo:\n\ttransposicion {-C|-D} {-p permutacion | -n Nperm}");
        printf("[-i filein] [-o fileout]\n\n");
        printf("OPCION\t\t\tDESCRIPCION\n");
        printf("-C\t\t\tEl programa cifra\n");
        printf("-D\t\t\tEl programa descifra\n");
        printf("-p permutacion\t\tCadena conteniendo la permutacion\n");
        printf("  Ejemplo:\t\t1 2 3 4 5\n");
        printf("\t\t\t4 1 2 5 3\n");
        printf("\t\t\t-p '4 1 2 5 3'\n\n");
        printf("-n Nperm\t\tNumero de elementos que se permutan (Generá una permutacion aleatoria de n elementos). Solo se utilizara con la opcion -C.\n");
        printf("-i filein\t\tfichero de entrada. En caso de no especificarse se pedirá por pantalla el texto a cifrar\n");
        printf("-o fileout\t\tichero de salida. En caso de no especificarse se pedirá por pantalla el texto a descifrar\n\n");
        printf("Algunos ejemplos de uso:\n");
        printf("\t\t\ttransposicion -C -p '4 1 2 5 3' -i archivo_a_cifrar.txt -o archivo_cifrado.txt\n");
        printf("\t\t\ttransposicion -C -n 4\n");
        printf("\t\t\ttransposicion -D -p '4 1 2 5 3' -i archivo_a_descifrar.txt -o archivo_descifrado.txt\n");
        return -1;
    }

    // Comprobamos si queremos cifrar o descifrar
    if(!strcmp(argv[1], "-C")){
        flag_cifrar = 1;
    }else if(!strcmp(argv[1], "-D")){
        flag_cifrar = 0;
    }else{
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./transposicion -h.\n");
        return -1;
    }

    // Recuperamos los parametros introducidos por terminal
    if(argv[2] == NULL || argv[3] == NULL){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./transposicion -h.\n");
        return -1;
    }else{
    	// Le decimos que idioma se va a utilizar
    	set_modulo(IDIOMA);
        if(!strcmp(argv[2], "-p")){
        	i = 0;

        	token = strtok (argv[3], " ");
        	permutacion[i] = atoi(token);


        	if(token != NULL){
	            for(i = 1; (token = strtok(NULL, " ")) != NULL; i++){
	            	permutacion[i] = atoi(token);
		        }
		        num_elem = i;
				if(num_elem > MAX_ELEMS){
					fprintf (stderr, "Error: Tamaño máximo de la permutación %d.\n", MAX_ELEMS);
		            return -1;
				}
				if(comprueba_inyectividad(permutacion, num_elem) == -1){
                    printf("Error: La permutacion no es inyectiva.\n");
                    return -1;
                }
        	}else{
        		printf("Introduzca correctamente los argumentos.\n");
		        printf("Para ver la ayuda ./transposicion -h.\n");
		        return -1;
        	}
        }
        else if(!strcmp(argv[2], "-n") && (num_elem = atoi(argv[3])) > 0){

			if(num_elem > MAX_ELEMS){
				fprintf (stderr, "Error: Tamaño máximo de la permutación %d.\n", MAX_ELEMS);
	            return -1;
			}

        	srand(time(NULL));
        	for (i = 0; i < num_elem; i++) {     // fill array
			    permutacion[i] = i+1;
			}
			for (i = 0; i < num_elem; i++) {    // shuffle array
			    foobar = permutacion[i];
			    random_index = rand() % num_elem;
			    permutacion[i] = permutacion[random_index];
			    permutacion[random_index] = foobar;
			}
			infile = fopen("permutacion.dat", "w");
			for(i = 0; i < num_elem; i++)
				fprintf(infile, "%d ", permutacion[i]);
			fclose(infile);
        }
        else{
        	printf("Introduzca correctamente los argumentos.\n");
            printf("Para ver la ayuda ./transposicion -h.\n");
            return -1;
        }
    }

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
        // Caso en el que se ha elegido cifrar el texto por pantalla
        if(flag_cifrar)
            printf("\nIntroduzca el texto que desea cifrar (hasta %d caracteres)\n > ", MAX_TEXTO-1);
        // Caso en el que se ha elegido descifrar el texto por pantalla
        else
            printf("\nIntroduzca el texto que desea descifrar (hasta %d caracteres)\n > ", MAX_TEXTO-1);

        texto = (char*)malloc(MAX_TEXTO * sizeof(char));
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

    matriz = matriz_identidad(permutacion, num_elem);

    if(flag_cifrar){
        texto = cifrar_hill(texto, num_elem, matriz);
        if(texto == NULL){
            fprintf(stderr, "Error: Al cifrar el texto.\n");
            for(i = 0; i < num_elem; i++)
                free(matriz[i]);
            free(matriz);
            return -1;
        }
    }else{
        texto = descifrar_hill(texto, num_elem, matriz);
        if(texto == NULL){
            fprintf(stderr, "Error: Al descifrar el texto.\n");
            for(i = 0; i < num_elem; i++)
                free(matriz[i]);
            free(matriz);
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

	for(i = 0; i < num_elem; i++)
        free(matriz[i]);
    free(matriz);
    free(texto);

    return 1;
}
