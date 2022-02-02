/** 
 * Main encargado de mostrar el funcionamiento 
 * del cifrado de vignere
 *
 * Autor: Alberto Garcia Redero
 * Version: 1.0
 *
 * Fecha: 18/10/2018 
 */

#include "../includes/vignere.h"


int main (int argc,char *argv[]) {

    char faux[] = "f_aux.txt";
    int i=0;
    if(argc != MAX_ARG_VIGNERE && argc!=AYUDA){
        printf("Introduzca correctamente los argumentos.\n");
        printf("Para ver la ayuda ./vignere -h.\n");
        return (0);
    }

    if(argc == AYUDA && strcmp(argv[1],"-h") == 0){
        printf("Ejemplo:\n\t./vignere {-C|-D} {-k clave} ");
        printf("[-i filein] [-o fileout]\n\n");
        printf("OPCION\t\t\tDESCRIPCION\n");
        printf("-C\t\t\tEl programa cifra\n");
        printf("-D\t\t\tEl programa descifra\n");
        printf("-k\t\t\tcadena de caracteres usada como clave\n");
        printf("-i\t\t\tfichero de entrada\n");
        printf("-o\t\t\tfichero de salida\n");
        return(0);
    }       

    for(i=0; i<strlen(argv[3]); i++){               /*Comprobamos que la clave sea correcta*/
        if(argv[3][i] < 64 || argv[3][i] >91){
            printf("-k debe ser en Z26, letras mayusculas sin ñ\n");
            return(0);
        }
    }                                       


    limpiar_archivo(argv[5], faux);                    /*Preparamos el fichero*/

    if(strcmp(argv[1],"-C") == 0){
        vignere_cifrar(faux, argv[7], argv[3]);         /*Cifrar*/
        printf("Texto cifrado correctamente\n");
    } else if(strcmp(argv[1],"-D") == 0){
        vignere_descifrar(faux, argv[7], argv[3]);      /*Descifrar*/
        printf("Texto descifrado correctamente\n");
    } else{
        printf("Elija entre cifrar o descifrar\n");
    }

    remove(faux);

    return (0);
}