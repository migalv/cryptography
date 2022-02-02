/* 
 * Modulo encargado de parsear el texto a nuestro alfabeto
 * para el cifrado
 *
 * Modulo: parser.c
 * Autor: Miguel Alvarez Lesmes
 */

#include "../include/parser.h"

/****************************************************************
 * Parsea los caracteres de un archivo y los guarda en otro. Las 
 * letras como enie, y vocales con acentos las pasa a su respectiva
 * letra en mayusculas sin el acento. Ademas pasa todas las 
 * minusculas a mayusculas y elimina los espacios en blanco.
 * El resultado es un texto todo en mayusculas sin acentos, sin 
 * enies y sin espacios en blanco.
 *
 * Function: limpiar_archivo
 * Arguments: [0]-char* --> Nombre del archivo a limpiar
 *            [1]-char* --> Nombre del archivo de salida
 * Return: VOID 
 ****************************************************************/
void limpiar_archivo(char* infilename, char* outfilename){

    setlocale(LC_CTYPE, "");
    FILE * f_entrada;
    FILE * f_salida;
    wchar_t c;
    
    f_entrada = fopen(infilename, "r");
    if (!f_entrada)
        return;
    f_salida = fopen (outfilename, "w");
    if (!f_salida)
        return;

    while((c = fgetwc(f_entrada)) != WEOF){
        switch (c) {
            case L'á': c=L'A';break;
            case L'é': c=L'E';break;
            case L'í': c=L'I';break;
            case L'ó': c=L'O';break;
            case L'ú': c=L'U';break;

            case L'Á': c=L'A';break;
            case L'É': c=L'E';break;
            case L'Í': c=L'I';break;
            case L'Ó': c=L'O';break;
            case L'Ú': c=L'U';break;

            case L'Ñ': c=L'N';break;
            case L'ñ': c=L'N';break;
            default:    break;
        }
        if(c>96 && c<123){ /*Letras minusculas*/
            c -=32;
        } 

        if(c>64 && c<91){ /*Imprimimos solo las mayusculas, puesto que debe estar parseado a estas el texto correctamente*/
            fwprintf(f_salida, L"%lc", c);

        }
    }

    fclose(f_entrada);
    fclose(f_salida);
    return;
}