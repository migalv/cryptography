/**
 * Main generar las sboxes del
 * aes
 *
 * Autor: Alberto Garcia Redero
 * Version: 1.0
 *
 * Fecha: 25/11/2018
 */

#include "../include/sbox_aes.h"


int main (int argc,char *argv[]) {

    uint16_t transf_afin = 0;
    int16_t m = 283;        //modulo
    uint16_t n = 0;
    //uint16_t n = 149;//'95'
    int i = 0, j = 0;
    int flag_archivo = 0;   //0 no hay archivo de salida
    FILE * f_out = NULL;

    uint16_t d = 0;
    uint16_t s = 0;
    uint16_t t = 0;
/*    //euclides(m, n, &s, &t);
    euclides(m, n, &d, &s, &t);
    //euclides(result, m, n);
    //printf("%"PRIu16"\n", result[1]);
    printf("%"PRIu16"\n", d);
    printf("%"PRIu16"\n", s);
    printf("%"PRIu16"\n", t);
    return 0;
    ////////////////////////////77
*/
    if(argc == 4 &&  strcmp(argv[2], "-o") == 0){  //Archivo entrada
        flag_archivo = 1;
        f_out = fopen (argv[3], "w");
    }

    if(argc == 4 || argc == 2){
        if(strcmp(argv[1], "-C") == 0){

            printf("Generando S-Box directa...\n");

            if(flag_archivo){
                fprintf(f_out, "\nS-Box directa:\n\n");
            }
            else{
                printf("\nS-Box directa:\n\n");
            }
            //Directa
            for(i = 0, n = 0; i < 16; i++){
                for(j = 0, d = 0, s = 0, t = 0; j < 16; j ++, n++){
                    if(n == 0){
                        transf_afin = transformacion_afin_directa(0);
                    }
                    else{
                        euclides(m, n, &d, &s, &t);
                        transf_afin = transformacion_afin_directa(t);
                    }
                    print_hexa(&transf_afin, 1, 1, f_out);
                    if(flag_archivo){
                        fprintf(f_out, "\t");
                    }
                    else{
                        printf("\t");
                    }
                }
                if(flag_archivo){
                    fprintf(f_out, "\n");
                }
                else{
                    printf("\n");
                }
            }
            if(flag_archivo){
                fclose(f_out);
            }
            return 0;
        }
        else if(strcmp(argv[1], "-D") == 0){
            printf("Generando S-Box inversa...\n");
            if(flag_archivo){
                fprintf(f_out, "\nS-Box inversa:\n\n");
            }
            else{
                printf("\nS-Box inversa:\n\n");
            }
            //Inversa
            for(i = 0, n = 0; i < 16; i++){
                for(j = 0; j < 16; j ++, n++){
                    transf_afin = transformacion_afin_inversa(n);

                    if(transf_afin == 0){
                        t = 0;
                    }
                    else{
                        euclides(m, n, &d, &s, &t);
                    }

                    print_hexa(&t, 1, 1, f_out);
                    if(flag_archivo){
                        fprintf(f_out, "\t");
                    }
                    else{
                        printf("\t");
                    }
                }
                if(flag_archivo){
                    fprintf(f_out, "\n");
                }
                else{
                    printf("\n");
                }
            }
            if(flag_archivo){
                fclose(f_out);
            }
            return 0;
        }
        else if(strcmp(argv[1], "-h") == 0){
            printf("Ejemplo:\n\t./sbox_aes {-C | -D} [-o fileout]\n\n");
            printf("OPCION\t\t\tDESCRIPCION\n");
            printf("-C\t\t\tcalcular la S-Box directa\n");
            printf("-D\t\t\tcalcular la S-Box inversa\n");
            printf("-o\t\t\tfichero de salida\n");
            if(flag_archivo){
                fclose(f_out);
            }
            return(0);
        }
    }
    else{
        printf("Introduzca correctamente la entrada.\n");
        printf("Para ver la ayuda ./sbox_aes -h.\n");
    }
    if(flag_archivo){
        fclose(f_out);
    }
    return (0);

}
