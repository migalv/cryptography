
#include "../includes/criptoanalisis.h"
#include "../includes/euclides.h"

/****************************************************************
 * Funcion que implementa el criptoanalisis por el metodo del
 * indice de coincidencia, obtiene la clave con la que se ha cifrado.
 *
 * Function: ic_clave
 * Arguments: [0]-char* --> Fichero con texto cifrado
 *            [1]-char* --> Fichero donde se guardara el informe 
 *							del criptoanalisis
 *            [2]-int   --> Tamano de la clave
 * Return: Nada, se guarda en el informe
 ****************************************************************/
void ic_clave(char* fin, char* fout, int tam_k){

	FILE* f_entrada = NULL;
	FILE* f_salida = NULL;
	float frec[] = {8.04, 1.54, 3.06, 3.99, 12.51, 						/*Frecuencia de aparicion de las letras en el ingles*/
					2.3, 1.96, 5.49, 7.26, 0.16, 
					0.67, 4.14, 2.53, 7.09, 7.6, 
					2.0, 0.11, 6.12, 6.54, 9.25, 
					2.71, 0.99, 1.92, 0.19, 1.73, 0.19};
	char* buf = NULL;
	float* clave = NULL;
	float n_repeticiones[26] = {0.0};
	int tamano = 0, i = 0, j = 0, k = 0, caracter;
	float ic;
	float divisor = 0.0;


	f_entrada = fopen(fin, "r");
	if(!f_entrada){
		printf("Error abriendo el fichero\n");
		return;
	}
	f_salida = fopen (fout, "a");
	if(!f_salida){
		printf("Error abriendo el fichero\n");
		fclose(f_entrada);
		return;
	}

	fseek(f_entrada,0,SEEK_END); 											/*Guardamos el tamano del fichero*/
	tamano = ftell(f_entrada); 
	fseek(f_entrada, 0, SEEK_SET);

	buf = (char*)malloc(sizeof(char)*tamano+1);
	if(!buf){
		fclose(f_entrada);
		fclose(f_salida);
		return;
	}
	clave = (float*)malloc(sizeof(float)*tam_k+1);
	if(!clave){
		free(buf);
		fclose(f_entrada);
		fclose(f_salida);
		return;
	}

	fseek(f_entrada, 0, SEEK_SET);
	if(fread(buf, sizeof(char), tamano, f_entrada)==0){							/*Guardamos el texto en el buffer*/
		free(clave);
		fclose(f_entrada);
		fclose(f_salida);
		free(buf);
		return;
	}

	for(i=0; i<tam_k; i++){														//BUSCAMOS EN CADA BLOQUE CON QUE LETRA SE HA CIFRADO
		for(j=0; j<26; j++){													//VAMOS PROBANDO CON CADA LETRA DEÑ ABECEDARIO

			for(k=0;k<26;k++){
				n_repeticiones[k]=0.0;
			}

			for(k=i; k<tamano; k+=tam_k){										//DESCIFRAMOS CON LA LETRA Y OBTENEMOS FRECUENCIAS
				caracter = (buf[k]-65) - j; 									//Desencriptamos el texto
				caracter = (caracter >= 0 ? caracter : caracter+26);
				n_repeticiones[caracter]+=1.0;
			}

			divisor = (float)tamano/(float)tam_k;
			for(k=0, ic=0.0; k<26; k++){										//COMPROBAMOS SI TIENE SENTIDO
				ic += ((frec[k]/100)*n_repeticiones[k])/divisor;
			}

			if(ic>0.057){
				printf("%c",j+65);												//Guardamos los resultados en el fichero de informe
				fprintf(f_salida, "%c",j+65);
			}
		}
	}	

	fprintf(f_salida, "\n");
	printf("\n");

	free(buf);
	free(clave);
	fclose(f_entrada);
	fclose(f_salida);	
	return;
}

/****************************************************************
 * Funcion que implementa el criptoanalisis por el metodo del
 * indice de coincidencia.
 *
 * Function: indice_coincidencia
 * Arguments: [0]-char* --> Fichero con texto cifrado
 *            [1]-char* --> Fichero donde se guardara el informe 
 *							del criptoanalisis
 *            [2]-char* --> Tamano del ngrama, se comprueba si este
 *							es el tamano real de la clave
 * Return: Nada, se guarda en el informe
 ****************************************************************/
void indice_coincidencia(char* fin, char* fout, int tam_ngrama){

	FILE* f_entrada = NULL;
	FILE* f_salida = NULL;
	char* buf = NULL;
	float* indices_C = NULL;
	float resultado = 0.0, total=0.0;
	int n_repeticiones[26] = {0};
	int i=0, j=0, tamano=0;
	
	f_entrada = fopen(fin, "r");
	if(!f_entrada){
		printf("Error abriendo el fichero\n");
		return;
	}
	f_salida = fopen (fout, "a");
	if(!f_salida){
		printf("Error abriendo el fichero\n");
		fclose(f_entrada);
		return;
	}	
	indices_C = (float*)malloc(sizeof(float)*tam_ngrama*+8);								/*Guardamos en el tamngrama la frecuencia_total en el grupo*/
	if(!indices_C){
		fclose(f_entrada);
		fclose(f_salida);
		return;
	}
	for(i=0; i<tam_ngrama; i++){
		indices_C[i]=0.0;
	}	


	fseek(f_entrada,0,SEEK_END);														    //Nos vamos el final del archivo
	tamano = ftell(f_entrada); 																//Obtenemos el tamaño del archivo

	buf = (char*)malloc(sizeof(char)*tamano+1);
	if(!buf){
		free(indices_C);
		fclose(f_entrada);
		fclose(f_salida);
		return;
	}

	fseek(f_entrada, 0, SEEK_SET);
	if(fread(buf, sizeof(char), tamano, f_entrada)==0){										/*Guardamos el texto en el buffer*/
		free(indices_C);
		fclose(f_entrada);
		fclose(f_salida);
		free(buf);
		return;
	}

	
	for(i=0; i<tam_ngrama; i++){															/*Buscamos en cada bloque los IC*/
		for(j=0;j<26;j++){
			 n_repeticiones[j]=0;
		}

		for(j=i, total=0.0; j<tamano ;j+=tam_ngrama, total++){								/*CUENTA EL NUMERO DE REPETICIONES DE CADA LETRA EN LA SUBCADENA*/
			n_repeticiones[buf[j]-65]+=1;
			
		}
		
		for(j=0;j<26;j++){																	/*SUMATORIO DE LAS REPETICIONES*/
			indices_C[i]+=(n_repeticiones[j]*(n_repeticiones[j]-1));
		}
	
		resultado = indices_C[i]/(total*(total-1));											/*DIVIDIMOS POR TOTAL BLOQUE*/

		if(resultado<0.06){																	/*En caso de que algun resultado no tenga un IC parecido al ingles no se guarda*/
			printf("TAMAÑO DE CLAVE NO CORRECTO, el tamaño correcto es el de kasiski\n");	
			fprintf(f_salida, "INDICE DE COINCIDENCIA: TAMAÑO DE CLAVE NO CORRECTO, el tamaño correcto es el de kasiski\n");	
			
			free(indices_C);
			fclose(f_entrada);
			fclose(f_salida);
			free(buf);
			return;
		}
	}	

	printf("El tamaño de clave es: %d\n", tam_ngrama);											/*Si todo ha ido bien confirmamos que el tamaño de */
	fprintf(f_salida, "INDICE DE COINCIDENCIA: El tamaño de clave es: %d\n", tam_ngrama);		/*la clave es el mismo que el del ngrama introducido*/

	free(indices_C);
	fclose(f_entrada);
	fclose(f_salida);
	free(buf);

	return;
}


/****************************************************************
 * Funcion que implementa el criptoanalisis por el metodo de 
 * kasiski.
 *
 * Function: kasiski
 * Arguments: [0]-char* --> Fichero con texto cifrado
 *            [1]-char* --> Fichero donde se guardara el informe 
 *							del criptoanalisis
 *            [2]-char* --> Tamano del ngrama
 * Return: int, tamano de la clave con la que se cifrado el texto
 ****************************************************************/
int kasiski(char* fin, char* fout, int tam_ngrama){

	FILE* f_entrada;
	FILE* f_salida;
	int* encontrados;
	int* distancia;
	int mcds[BUF_1000K];
	int mcd;
	int n_mcd;
	int i, j, n_encontrados;
	char* ngrama;
	char* buf;
	int leidos;
	int total_leidos;
	int encontrado;
	int n_distancia;
	int* d;
	int* s;
	int* t;
	int resultado;
	int tamano;



	f_entrada = fopen(fin, "r");
	if(!f_entrada){
		printf("Error abriendo el fichero\n");
		return -1;
	}
	f_salida = fopen (fout, "w");
	if(!f_salida){
		printf("Error abriendo el fichero\n");
		fclose(f_entrada);
		return -1;
	}

	ngrama = (char*)malloc(sizeof(char)*tam_ngrama+1);
	if(!ngrama){
		fclose(f_entrada);
		fclose(f_salida);
		return -1;
	}
	buf = (char*)malloc(sizeof(char)*BUFFER_1MB+1);
	if(!buf){
		fclose(f_entrada);
		fclose(f_salida);
		free(ngrama);
		return -1;
	}
	encontrados = (int*)malloc(sizeof(int)*BUF_1000K);
	if(!encontrados){
		fclose(f_entrada);
		fclose(f_salida);
		free(ngrama);
		free(buf);
		return -1;
	}
	d = (int*)malloc(sizeof(int));
	if(!d){
		fclose(f_entrada);
		fclose(f_salida);
		free(ngrama);
		free(buf);
		free(encontrados);
		return -1;
	}
	s = (int*)malloc(sizeof(int));
	if(!s){
		fclose(f_entrada);
		fclose(f_salida);
		free(ngrama);
		free(buf);
		free(encontrados);
		free(d);
		return -1;
	}
	t = (int*)malloc(sizeof(int));	
	if(!t){
		fclose(f_entrada);
		fclose(f_salida);
		free(ngrama);
		free(buf);
		free(encontrados);
		free(d);
		free(s);
		return -1;
	}
	distancia = (int*)malloc(sizeof(int)*BUF_1000K);
	if(!distancia){
		fclose(f_entrada);
		fclose(f_salida);
		free(ngrama);
		free(buf);
		free(encontrados);
		free(d);
		free(s);
		free(t);
		return -1;
	}


	fseek(f_entrada,0,SEEK_END); 															/*Obtenemos el tamano del fichero*/
	tamano = ftell(f_entrada); 
		
	for(i=0, n_mcd=0, n_encontrados=0, total_leidos=0, n_distancia=0; i<tamano;i++){		/*Recorremos todo el fichero obteniendo cada posible subcadena del tamaaño dado*/

		fseek(f_entrada, i, SEEK_SET);
		memset(ngrama,'\0',tam_ngrama);
		if(fread(ngrama, sizeof(char), tam_ngrama, f_entrada) != tam_ngrama){				/*Obtenemos la cadena inicial que buscamos*/
			break;
		}
	
		fseek(f_entrada, 0, SEEK_SET);	

		do{																					/*Comenzamos a buscar repeticiones de la subcadena*/
			leidos=-1;
			memset(buf,'\0',BUFFER_1MB);
			leidos = fread(buf, sizeof(char), BUFFER_1MB, f_entrada);
			
			encontrado = kmp(buf, ngrama);													/*Buscamos la subcadena con el Algoritmo Knuth-Morris-Pratt, por su velocidad y eficiencia*/
			if(encontrado != -1){															/*Encontrado en el texto el ngrama*/		
				encontrados[n_encontrados] = encontrado+total_leidos;
				
				n_encontrados++;
				total_leidos+=encontrado+tam_ngrama+1;										/*+1 para evitar que encuentre luego la misma cadena*/
				fseek(f_entrada, 0, SEEK_SET);
				fseek(f_entrada, total_leidos, SEEK_SET);								
				
			} else{																			/*En caso de no haber encontrado seguimos buscando*/
				total_leidos+=1000000;
				fseek(f_entrada, 0, SEEK_SET);
				fseek(f_entrada, total_leidos, SEEK_SET);
			}
		}while(leidos > 0);

		
		for(j=0; n_encontrados>2 && j<n_encontrados-1; j++){								/*Calculamos distancias entre las repeticiones*/
			distancia[j] = encontrados[j+1]-encontrados[j];
			n_distancia++;
		}

		for(j=0, mcd=-1; n_distancia>1 && j<n_distancia-1; j++){							/*Realizamos el mcd entre las distancias*/
			if(j==0){
				euclides(distancia[j], distancia[j+1], d, s, t);
				mcd=*d;
				j++;
			}
			else{
				euclides(mcd, distancia[j], d, s, t);
				mcd = *d;
			}
		}

		n_distancia=0;
		n_encontrados=0;
		encontrado=0;
		total_leidos=0;

		if(mcd!=-1){
			mcds[n_mcd] = mcd;																	/*Guardamos los mcd*/
			n_mcd++;
			mcd=-1;
		}
	}

	resultado = elemento_mas_popular(mcds, n_mcd);												/*Para filtrar y borrar basura nos quedamos con el mcd mas popular*/
	
	printf("El tamaño de clave es: %d\n", resultado);
	fprintf(f_salida, "KASISKI: El tamaño de clave es:  %d\n", resultado);

	fclose(f_entrada);
	fclose(f_salida);
	free(ngrama);
	free(buf);
	free(encontrados);
	free(d);
	free(s);
	free(t);		
	free(distancia);

	return resultado;
}

/****************************************************************
 * Funcion que auxiliar para kmp que inicializa el array
 *
 * Function: kasiski
 * Arguments: [0]-char* --> Fichero con texto cifrado
 *            [1]-char* --> Fichero donde se guardara el informe 
 *							del criptoanalisis
 *            [2]-char* --> Tamano del ngrama
 * Return: int, tamano de la clave con la que se cifrado el texto
 ****************************************************************/
int* init_array(int size) {
	int* arr = (int*)malloc(size * sizeof(int));
	int i;
	for(i = 0; i < size; i++) {
    	arr[i] = 0;
	}

 	return arr;
}

/****************************************************************
 * Funcion que implementa el Algoritmo Knuth-Morris-Pratt, el cual
 * nos permite encontrar una subcadena en una cadena a un coste 
 * bajo, lo que ayuda a reducir los tiempos de busqueda.
 *
 * Function: kmp
 * Arguments: [0]-char* --> Cadena con el texto
 *            [1]-char* --> Subcadena a abuscar
 * Return: int, posicion donde se encuentra el inicio de la subcadena
 ****************************************************************/
int kmp(char* t, char* p) {
	int m = strlen(p);
	int n = strlen(t);

	int* f = init_array(m); // Failure function values.
	int i = 0;
	int j = 0;

	while (i < n) {
		if (t[i] == p[j]) {
    		if (j == m - 1) {			/*En caso de encontrar la subcadena devolvemos la posicion*/
       			free(f);
		        return i - j;
      		}
      		else {						/*En caso contrario seguimos buscando*/
        		i += 1;
        		j += 1;
      		}
    	}
    	else {
      		if (j > 0) {
        		j = f[j-1];
      		}
     		 else {
        		i += 1;
      		}
    	}
  	}

	free(f);

	return -1;							/*Caso de no haberlo encontrado*/
}


/****************************************************************
 * Funcion que devuelve el elemento mas popular en un array de enteros,
 * es util para eliminar basura en kasiski
 *
 * Function: euclides
 * Arguments: [0]-int* --> Array de enteros
 *            [1]-int  --> Numero de elementos en el array
 *			  
 * Return: int, numero mas popular
 ****************************************************************/
int elemento_mas_popular(int* numeros, int n_mcd){

	int count = 0, curr_cnt = 1, freq_num = 0, key = 0, i = 0, j = 0;
	
	for(i = 0; i < n_mcd-1; i++)							/*Buscamos el elemento mas popular entre los del array*/
	{
	    key = numeros[i];
	    for(j =i+1; j < n_mcd; j++)
	    {
	        if(key == numeros[j] && freq_num != key)
	        {
	            curr_cnt++;
	        }
	    }
	    if(count < curr_cnt)
	    {
	        count = curr_cnt;
	        curr_cnt = 1;
	        freq_num = key;
	    }
	}

	if(n_mcd==1){											/*Si solo hay un elemento devolvemos este*/
		freq_num = numeros[0];
	}

	return freq_num;
}