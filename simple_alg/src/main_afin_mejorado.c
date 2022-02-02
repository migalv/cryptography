/** 
 * Main encargado de mostrar el funcionamiento 
 * del cifrado de vignere
 *
 * Autor: Alberto Garcia Redero
 * Version: 1.0
 *
 * Fecha: 18/10/2018 
 */

#include "../includes/afin_mejorado.h"
#include "../includes/afin.h"
#include "../includes/euclides.h"
#include "../includes/parser.h"

int main (int argc,char *argv[]) {

	mpz_t a, b, m, d, s, t;
	mpz_t m2, s2;
	char faux[] = "f_aux.txt";
	char entrada[TAM_FICH];
	char buf_entrada[BUF_TAM];
	char salida[TAM_FICH];
	FILE * f_entrada;


	if((argc<MIN_ARG_AFIN_M || argc>MAX_ARG_AFIN_M) && argc!=AYUDA){
		printf("Introduzca correctamente los argumentos.\n");
		printf("Para ver la ayuda ./afin -h.\n");
		return (0);
	}

	if(argc == AYUDA && strcmp(argv[1],"-h") == 0){
	  	printf("Ejemplo:\n\t./afin {-C|-D} {-m|Zm} {-a Nx} {-b N+} ");
	  	printf("[-i filein] [-o fileout]\n\n");
	  	printf("OPCION\t\t\tDESCRIPCION\n");
	  	printf("-C\t\t\tEl programa cifra\n");
	  	printf("-D\t\t\tEl programa descifra\n");
	  	printf("-m\t\t\tTamaño del espacio de texto cifrado\n");
	  	printf("-a\t\t\tcoeficiente multiplicativo de la funcion afin\n");
	  	printf("-b\t\t\ttermino constante de la funcion afin\n");
	  	printf("-i\t\t\tfichero de entrada\n");
	  	printf("-o\t\t\tfichero de salida\n");
	  	return (0);
	}

	if(argc == MIN_ARG_AFIN_M){
		printf("Introduzca un texto que cifrar\n");
		fgets(buf_entrada, BUF_TAM, stdin);
		printf("El resultado se guardara en salida.txt\n");
		strcpy(salida, "salida.txt");
		
		strcpy(entrada, "f.txt");
		f_entrada = fopen (entrada, "w");
		fprintf(f_entrada, "%s", buf_entrada);
		fclose(f_entrada);

	} else{
		strcpy(entrada, argv[5]);
		strcpy(salida, argv[6]);
		printf("El resultado se guardara en %s\n", salida);

	}

	mpz_init (a);
	mpz_init (b);
	mpz_init (m);
	mpz_init (d);
	mpz_init (s);
	mpz_init (t);
	mpz_init (m2);
	mpz_init (s2);

	mpz_set_str (b, argv[4], 10);
	mpz_set_str (a, argv[3], 10);
	mpz_set_str (m, argv[2], 10);

	mpz_mul(m2, b, m);

	euclides_gmp(a,m,d,s,t);
	if(mpz_cmp_ui(s,(unsigned long)0) < 0){
		mpz_add(s, s, m);
	}
	if(mpz_cmp_ui(d,(unsigned long)1) != 0){
		
		printf("El coeficiente multiplicativo no tiene inverso\n");
		mpz_clear (a);
		mpz_clear (b);
		mpz_clear (m);
		mpz_clear (d);
		mpz_clear (s);
		mpz_clear (t);
		mpz_clear (m2);
		mpz_clear (s2);

	  	return(0);
	}

	euclides_gmp(a,m2,d,s2,t);
	if(mpz_cmp_ui(s2,(unsigned long)0) < 0){
		mpz_add(s2, s2, m2);
	}
	if(mpz_cmp_ui(d,(unsigned long)1) != 0){
		
		printf("El coeficiente multiplicativo no tiene inverso\n");
		mpz_clear (a);
		mpz_clear (b);
		mpz_clear (m);
		mpz_clear (d);
		mpz_clear (s);
		mpz_clear (t);
		mpz_clear (m2);
		mpz_clear (s2);

	  	return(0);
	}

	limpiar_archivo(entrada, faux);

	if(strcmp(argv[1],"-C") == 0){
		afin_mejorado_cifrar(faux, salida, a, b, m, m2);
		printf("\nTexto cifrado correctamente\n");
	} else if(strcmp(argv[1],"-D") == 0){
		afin_mejorado_descifrar(faux, salida, b, s, s2, m, m2);
		printf("\nTexto descifrado correctamente\n");
	} else{
		printf("Elija entre cifrar o descifrar\n");
	}

	strcpy(entrada, "f.txt");
	if((f_entrada = fopen (entrada, "w"))!=NULL){
		fclose(f_entrada);
		remove(entrada);
	}
	remove(faux);

	mpz_clear (a);
	mpz_clear (b);
	mpz_clear (m);
	mpz_clear (d);
	mpz_clear (s);
	mpz_clear (t);
	mpz_clear (m2);
	mpz_clear (s2);

  	return(0);
}
