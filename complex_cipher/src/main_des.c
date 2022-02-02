
#include "../include/desCBC.h"
#include <inttypes.h>

// Maximo de argumentos de entrada
#define MAX_ARG 12
// Minimo de argumentos de entrada
#define MIN_ARG 2
// Minimo numero de argumentos para descifrar
#define MIN_ARG_D 6
// Buffer de 4000 bytes
#define TAM_BUFFER 4000

void destroy(void* puntero);

int main(int argc, char* argv[]) {

    // Contador de ayuda para los bucles.
    int i = 0, j = 0;
    // Flag para saber si cifrar o descifrar (Cifrar = 1 // Descifrar = 0)
    int flag_cifrar = 0;
    // Archivo de entrada
    FILE * infile = NULL;
    // Archivo de salida
    FILE * outfile = NULL;
    // Cadena de caracteres
    char* texto = NULL;
    // Texto en Bytes
    uint64_t* texto_b = NULL;
    // Numero de bytes antes y despues de hacer el padding
    int nbytes = 0;
    // Variables que recogen el nombre de los archivos de entrada y salida
    char* infilename = NULL; char* outfilename = NULL;
    // Index que indica cual es el proximo argumento que se debe leer
    int arg_index = 1;
    // Clave de 64 bits.
    uint64_t clave = 0x0;
    // Array de claves de 56 bits.
    uint64_t* claves = NULL;
    // Vector de inicializacion;
    uint64_t iv = 0x0;
    // Bits cifrados
    uint64_t* result = NULL;
    // Buffer de bits listos para cifrar
    uint8_t buffer[TAM_BUFFER];
    // Puntero auxiliar para liberar memoria
    uint64_t* p_aux = NULL;
    // Determina el modo de cifrado del DES, CBC por defecto
    MODO modo = CBC;

    // Caso de que el numero de argumentos no sea el correcto
    if(argc < MIN_ARG || argc > MAX_ARG){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda %s -h.\n", argv[0]);
        return -1;
    }

    // Mostrar ayuda
    if(argc == MIN_ARG && !strcmp(argv[arg_index],"-h")){
        printf("Ejemplo:\n\n%s {-C|-D -k clave -iv vectorinicializacion}", argv[0]);
        printf("[-i infile] [-o outfile] [-m modo]\n\n");
        printf("ARGUMENTO\t\t\tDESCRIPCION\n");
        printf("-C\t\t\tEl programa cifra. La clave e iv utilizados se proporcionarán por salida estandar.\n");
        printf("-D\t\t\tEl programa descifra.\n");
        printf(" └> Argumentos obligatorios al descifrar (En este orden):\n");
        printf("   └> -k Clave\t\t\tClave de 64 bits: 56 bits + 8 bits de paridad (es decir 16 Hexadecimal)\n");
        printf("   └> -iv Vector\t\t\tVector de inicializacion (iv). (64 bits) (es decir 16 Hexadecimal)\n");
		printf("\nOPCION\t\t\tDESCRIPCION\n");
        printf("-i infile\t\tFichero de entrada. En caso de no especificarse se pedirá por pantalla el texto a cifrar\n");
        printf("-o outfile\t\tFichero de salida. En caso de no especificarse se pedirá por pantalla el texto a descifrar\n");
        printf("-m modo\t\t\tModo de cifrado del DES. (CBC o ECB)\n");
        printf("\n  Ejemplos de uso:\n\n");
        printf("\t%s -C \n", argv[0]);
        printf("\t%s -C -o fichero_cifrado.txt -m CBC\n", argv[0]);
        printf("\t%s -D -k \"89751A7983627CFD\" -iv \"28ECD735A60996AB\" -i fichero_a_descifrar.txt -o fichero_descifrado.txt -m ECB\n", argv[0]);
        return 1;
    }

    // Comprobamos si queremos cifrar o descifrar
    if(!strcmp(argv[arg_index], "-C")){
		// Indicamos que queremos cifrar
        flag_cifrar = 1;
        arg_index++;
    }
    // En caso de querer descifrar obtenemos la clave y el iv
    else if(!strcmp(argv[arg_index++], "-D")){
		// Comprobamos que el número de argumentos es el correcto.
        if(argc < MIN_ARG_D){
            printf("Introduzca correctamente los argumentos.\n");
            printf("Para ver la ayuda %s -h.\n", argv[0]);
            return -1;
        }
		// Obtenemos la clave
		if(!strcmp(argv[arg_index++], "-k")){
			if(strlen(argv[arg_index]) != 16){
				fprintf (stderr, "Error: El tamaño de la clave debe ser de 64 bits.\n");
		        return -1;
			}
            clave = strtoull(argv[arg_index++], NULL, 16);

            // Comprobamos la paridad de clave para ver si es correcta
        	if(!check_parity(clave)){
        		fprintf(stderr, "Error: La clave no es correcta. a que se haya"
        						" modificado. Vuelve a intentarlo\n");
        		return -1;
        	}    ///////////////////////////////////////////////////////////////////////////////////////////DESCOMENTAR
		}else{
            printf("Introduzca correctamente los argumentos.\n");
            printf("Para ver la ayuda %s -h.\n", argv[0]);
            return -1;
        }
		// Obtenemos el iv.
		if(!strcmp(argv[arg_index++], "-iv")){
			if(strlen(argv[arg_index]) != 16){
				fprintf (stderr, "Error: El tamaño del vector de inicializacion debe ser de 64 bits.\n");
		        return -1;
			}
            iv = strtoull(argv[arg_index++], NULL, 16);
		}else{
            printf("Introduzca correctamente los argumentos.\n");
            printf("Para ver la ayuda %s -h.\n", argv[0]);
            return -1;
        }

		// Indicamos que queremos descifrar
        flag_cifrar = 0;
    }else{
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda %s -h.\n", argv[0]);
        return -1;
    }

    // Parseamos las posibles opciones.
    for(i = arg_index; i < argc; i++){
        // Comprobamos si se introduce un archivo de entrada
        if(!strcmp(argv[i], "-i")){
            // Nos aseguramos de que hay otro argumento con el nombre del archivo
            if(i+1 == argc){
                printf("Introduzca correctamente los argumentos.\n");
                printf("Para ver la ayuda %s -h.\n", argv[0]);
                return -1;
            }
            infilename = strdup(argv[++i]);
            // Se han leido dos argumentos
            arg_index += 2;
        }
        // Comprobamos si se introduce un archivo de salida
        else if(!strcmp(argv[i], "-o")){
            // Nos aseguramos de que hay otro argumento con el nombre del archivo
            if(i+1 == argc){
                printf("Introduzca correctamente los argumentos.\n");
                printf("Para ver la ayuda %s -h.\n", argv[0]);
                return -1;
            }
            outfilename = strdup(argv[++i]);
            // Se han leido dos argumentos
            arg_index += 2;
        }
        // Comprobamos se ha elegido algún modo
        else if(!strcmp(argv[i], "-m")){
            // Nos aseguramos de que hay otro argumento con el modo
            if(i+1 == argc){
                printf("Introduzca correctamente los argumentos.\n");
                printf("Para ver la ayuda %s -h.\n", argv[0]);
                return -1;
            }
            if(!strcmp("CBC", argv[i+1])){
                modo = CBC;
                i++;
            }
            else if(!strcmp("ECB", argv[i+1])){
                modo = ECB;
                i++;
            }else{
                printf("Introduzca correctamente los argumentos.\n");
                printf("Para ver la ayuda %s -h.\n", argv[0]);
                return -1;
            }
            // Se han leido dos argumentos
            arg_index += 2;
        }
        // Caso por defecto, no se ha reconocido la opcion.
        else{
            printf("No se reconoce la opcion \"%s\"\n", argv[i]);
            printf("Para ver la ayuda %s -h.\n", argv[0]);
            return -1;
        }
    }

    // Caso en el que ciframos
    if(flag_cifrar){
        printf("========== SE HA ELEGIDO LA OPCIÓN DE CIFRAR ==========\n");
        // Primero vamos a generar una clave y iv random.
        printf("Generando clave...\n");
        clave = generar_clave();
        // Comprobamos la paridad de clave para ver si es correcta
    	if(!check_parity(clave)){
    		fprintf(stderr, "Error: La clave no es correcta. Es posible que se haya"
    						" modificado. Vuelve a intentarlo\n");
    		return -1;
    	}
        printf("Clave --> ");
        print_hexa(&clave, 1, sizeof(uint64_t));

        printf("\nGenerando iv...\n");
        iv = generar_iv(clave);
        printf("IV -->" );
        print_hexa(&iv, 1, sizeof(uint64_t));

    }else{
        printf("========== SE HA ELEGIDO LA OPCIÓN DE DESCIFRAR ==========\n");
    }

    // Caso en el que te pasan un archivo a cifrar
    if(infilename){
        printf("Se está leyendo el archivo %s . . .\n", infilename);

        // Abrimos el archivo en modo binario
        infile = fopen(infilename, "rb");
        if(infile == NULL){
			fprintf(stderr, "Error[179]: Ha ocurrido un error al abrir el archivo %s.\n", infilename);
			return -1;
		}

        // Si hay archivo de salida lo abrimos
        if(outfilename){
            printf("Se escribirá el resultado en %s . . .\n", outfilename);
            outfile = fopen(outfilename, "wb");
            if(outfile == NULL){
    			fprintf(stderr, "Error[187]: Ha ocurrido un error al abrir el archivo %s.\n", outfilename);
    			return -1;
    		}
        }
        else{ // Si no lo mostraremos por pantalla
            printf("\nTexto cifrado\n > ");
        }
        claves = generar_claves(clave);
        // Leemos byte a byte el archivo hasta que no quede nada
        while((nbytes = fread(buffer, sizeof(uint8_t), TAM_BUFFER, infile)) != 0){
            if(flag_cifrar){ // Ciframos el buffer
                // Transformamos la entrada y le hacemos el padding
                p_aux = tandpad(buffer, &nbytes);
                result = cifrar_des(p_aux, nbytes, claves, iv, modo);
                free(p_aux);
            }
            else{
                // Transformamos la entrada y le hacemos el padding
                p_aux = tandpad(buffer, &nbytes);
                result = descifrar_des(p_aux, nbytes, claves, iv, modo);
                free(p_aux);
            }
            if(result == NULL){
                fprintf(stderr, "Error: Al cifrar o descifrar el texto.\n");
                if(infile != NULL)
                    fclose(infile);
                if(outfile != NULL)
                    fclose(outfile);
        		return -1;
            }
            // Escribimos en el archivo de salida
            if(outfilename){
                // Transformamos el resultado al formato de salida
                p_aux = tandpad(result, &nbytes);
                // Y comprobamos que no da error
                if(fwrite(p_aux, sizeof(uint64_t), nbytes/sizeof(uint64_t), outfile) == 0){
                    fprintf(stderr, "Error: Al escribir en el archivo %s.\n", outfilename);
                    if(infile != NULL)
                        fclose(infile);
                    if(outfile != NULL)
                        fclose(outfile);
                    return -1;
                }
                free(p_aux);
            }else{ // Si no por pantalla
                print_hexa(result, nbytes/sizeof(uint64_t), sizeof(uint64_t));
            }
        }
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
            fprintf(stderr, "Error[189]: Al reservar memoria para el texto.\n");
            return -1;
        }

        // Lees el texto de pantalla
        fgets(texto, MAX_TEXTO, stdin);
        // Eliminamos el \n del final
        texto[strcspn(texto, "\n")] = '\0';
        claves = generar_claves(clave);

        if(flag_cifrar){
            // Recuperamos la longitud del texto
            nbytes = strlen(texto);
            // Traducimos el texto que viene por pantalla a binario
            // Y le hacemos padding en caso de ser necesario
            texto_b = tandpad((uint8_t*)texto, &nbytes);
            // Ciframos o desciframos el texto, con la clave y el IV obtenidos anteriormente
            // Se le pasa el numero de bloques de 8 bytes del texto
            // Y el modo de cifrado (ECB o CBC)
            result = cifrar_des(texto_b, nbytes, claves, iv, modo);
        }
        else{

            nbytes = (strlen(texto)/16);
            texto_b = (uint64_t*)malloc(nbytes*sizeof(uint64_t));

            char* s  = NULL;
            s = (char*)malloc(sizeof(char)*100);

            for(j = 0; j < nbytes; j++){
                strncpy(s, texto+(16*j), 16);
                texto_b[j] = strtoull(s, NULL, 16);
            }
            free(s);


            result = descifrar_des(texto_b, nbytes*8, claves, iv, modo);
            nbytes*=8;
        }
        // Caso en el que la funcion descifrar fallase
        if(result == NULL){
            fprintf(stderr, "Error: Al cifrar el texto.\n");
            destroy(texto_b);
            destroy(texto);
    		return -1;
        }
        // Escribimos en el archivo de salida
        if(outfilename){
            printf("Se escribirá el resultado en %s . . .\n", outfilename);
            outfile = fopen(outfilename, "wb");
            if(outfile == NULL){
    			fprintf(stderr, "Error[267]: Ha ocurrido un error al abrir el archivo %s.\n", outfilename);
    			return -1;
    		}
            // Transformamos el resultado al formato de salida
            p_aux = tandpad(result, &nbytes);
            // Y comprobamos que no da error
            if(fwrite(result, sizeof(uint64_t), nbytes/sizeof(uint64_t), outfile) == 0){
                fprintf(stderr, "Error: Al escribir en el archivo %s.\n", outfilename);
                if(infile != NULL)
                    fclose(infile);
                if(outfile != NULL)
                    fclose(outfile);
                return -1;
            }
            free(p_aux);
        }else{// Mostramos el texto cifrado por pantalla
            if(flag_cifrar)
                printf("Texto cifrado \n> ");
            else
                printf("Texto descifrado \n> ");
            print_hexa(result, nbytes/sizeof(uint64_t), sizeof(uint64_t));
        }

    }

    destroy(texto_b);
    destroy(texto);
    destroy(result);
    destroy(infilename);
    destroy(outfilename);
    destroy(claves);
    if(infile != NULL)
        fclose(infile);
    if(outfile != NULL)
        fclose(outfile);

    return 1;
}

void destroy(void* puntero){
    if(puntero != NULL){
        free(puntero);
        puntero = NULL;
    }
}
