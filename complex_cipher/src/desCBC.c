/**
 * Modulo encargado de implementar las funciones del cifrado
 * y descifrado DES
 *
 * Modulo: desCBC.c
 * Autor: Miguel Álvarez Lesmes
 * Version: 1.0
 *
 * Fecha: 05/11/2018
 */


#include "../include/desCBC.h"
#include <inttypes.h>

short primer_bloque = 1;
uint64_t l_block = 0x0;
unsigned int seed = 0;
/****************************************************************
 * Función de cifrado del DES. Se le pasa un stream de datos
 * binarios y ésta los cifra según DES. Tiene 2 diferentes
 * modos de operacion: ECB, CBC.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 * ATENCION! -> Es importante que el stream esté en bloques de 64
 *				bits además de paddeado.
 *
 * Function: cifrar_des
 * Arguments: [0]-uint64_t* stream--> Stream de datos. Datos binarios
 *			  Deben ser bloques de 64bits. Con padding incluido.
 *			  [1]-int nbytes --> Número de bloques de 64Bits
 * 			  que tiene el stream de datos.
 *			  [2]-uint64_t clave--> Array de 16 claves de 56 bits
 *		      [3]-uint64_t IV--> Vector de inicializacion. utilizado
 * 			  para el modo CBC. Es de 64bits.
 *			  [4]-MODO modo--> Modo de cifrado del DES (ECB o CBC)
 * Return: uint64_t* Stream de datos cifrado.
 ****************************************************************/
uint64_t* cifrar_des(uint64_t* stream, int nbytes, uint64_t* claves, uint64_t iv,
	MODO modo){

	// Contadores para los bucles.
	int i = 0, j = 0;

	// 32bits de mayor peso de un bloque
	uint32_t l0 = 0x0;
	// Variable auxiliar para almacenar el valor de l0 antes de sobreescribirlo
	uint32_t aux_l0 = 0x0;
	// 32bits de menor peso de un bloque
	uint32_t r0 = 0x0;
	// Bloque de 64 bits
	uint64_t bloque64 = 0;
	// Variable auxiliar para liberar memoria
	uint64_t* p_bloque64 = NULL;
	// Stream cifrado
	uint64_t* c_stream = NULL;
	int nbloques = 0;

	if(stream == NULL || ((nbytes % TAM_CLAVE_BYTES) != 0)){
		fprintf(stderr, "Error: Parametros incorrectos en la función cifrar_des.\n");
		return NULL;
	}

	nbloques = nbytes/TAM_CLAVE_BYTES;
	c_stream = (uint64_t*)malloc(nbytes);

	if(c_stream == NULL){
		fprintf(stderr, "Error[71]: Se realizó mal la reserva de memoria de c_stream\n");
		return NULL;
	}

	for(i = 0; i < nbloques; i++){


		if(modo == CBC){
			// Modo CBC, el primer bloque XOR el IV
			if(primer_bloque == 1){

			 	bloque64 = iv ^ stream[0];
				primer_bloque = 0;
			}
			else // Los siguientes con el anterior bloque cifrado
				bloque64 = l_block ^ stream[i];
		}else if(modo == ECB){
			bloque64 = stream[i];
		}

		// Permutacion IP
		p_bloque64 = (uint64_t*)permutar(&bloque64, TAM_CLAVE_BITS, IP, BITS_IN_IP);
		if(p_bloque64 == NULL){
			fprintf(stderr, "Error[123]: Al llamar a permutar.\n");
			return NULL;
		}
		bloque64 = *p_bloque64;
		free(p_bloque64);

		// Bucle que realiza las 16 rondas
		for(j = 0; j < ROUNDS; j++){
			bloque64 = ronda(bloque64, claves[j]);
		}

		// Recogemos los 32 bits de mayor peso del bloque
		l0 = (bloque64 >> TAM_CLAVE_BITS/2);

		// Recogemos los 32 bits de menor peso de la bloque
		r0 = bloque64 & 0xFFFFFFFF;

		// Ultimo swap
		aux_l0 = l0;
		l0 = r0;
		r0 = aux_l0;

		bloque64 = (bloque64 << TAM_CLAVE_BITS/2) | l0;
		bloque64 = (bloque64 << TAM_CLAVE_BITS/2) | r0;

		// Permutacion IP inversa
		p_bloque64 = (uint64_t*)permutar(&bloque64, TAM_CLAVE_BITS, IP_INV, BITS_IN_IP);
		if(p_bloque64 == NULL){
			fprintf(stderr, "Error[171]: Al llamar a permutar.\n");
			return NULL;
		}
		bloque64 = *p_bloque64;
		free(p_bloque64);
		c_stream[i] = bloque64;
		l_block = bloque64;

	}

	return c_stream;
}

uint64_t* descifrar_des(uint64_t* stream, int nbytes, uint64_t* claves, uint64_t iv,
	MODO modo){

	// Contadores para los bucles.
	int i = 0, j = 0;

	// 32bits de mayor peso de un bloque
	uint32_t l0 = 0x0;
	// Variable auxiliar para almacenar el valor de l0 antes de sobreescribirlo
	uint32_t aux_l0 = 0x0;
	// 32bits de menor peso de un bloque
	uint32_t r0 = 0x0;
	// Bloque de 64 bits
	uint64_t bloque64 = 0;
	// Variable auxiliar para liberar memoria
	uint64_t* p_bloque64 = NULL;
	// Stream cifrado
	uint64_t* c_stream = NULL;
	int nbloques = 0;

	if(stream == NULL || ((nbytes % TAM_CLAVE_BYTES) != 0)){
		fprintf(stderr, "Error: Parametros incorrectos en la función cifrar_des.\n");
		return NULL;
	}

	nbloques = nbytes/TAM_CLAVE_BYTES;
	c_stream = (uint64_t*)malloc(nbytes);

	if(c_stream == NULL){
		fprintf(stderr, "Error[71]: Se realizó mal la reserva de memoria de c_stream\n");
		return NULL;
	}

	for(i = 0; i < nbloques; i++){

		bloque64 = stream[i];

		// Permutacion IP
		p_bloque64 = (uint64_t*)permutar(&bloque64, TAM_CLAVE_BITS, IP, BITS_IN_IP);
		if(p_bloque64 == NULL){
			fprintf(stderr, "Error[123]: Al llamar a permutar.\n");
			return NULL;
		}
		bloque64 = *p_bloque64;
		free(p_bloque64);

		// Bucle que realiza las 16 rondas
		for(j = 0; j < ROUNDS; j++){
			bloque64 = ronda(bloque64, claves[ROUNDS-j-1]);
		}

		// Recogemos los 32 bits de mayor peso del bloque
		l0 = (bloque64 >> TAM_CLAVE_BITS/2);

		// Recogemos los 32 bits de menor peso de la bloque
		r0 = bloque64 & 0xFFFFFFFF;

		// Ultimo swap
		aux_l0 = l0;
		l0 = r0;
		r0 = aux_l0;

		bloque64 = (bloque64 << TAM_CLAVE_BITS/2) | l0;
		bloque64 = (bloque64 << TAM_CLAVE_BITS/2) | r0;

		// Permutacion IP inversa
		p_bloque64 = (uint64_t*)permutar(&bloque64, TAM_CLAVE_BITS, IP_INV, BITS_IN_IP);
		if(p_bloque64 == NULL){
			fprintf(stderr, "Error[171]: Al llamar a permutar.\n");
			return NULL;
		}
		bloque64 = *p_bloque64;
		free(p_bloque64);


		if(modo == CBC){
			// Modo CBC, el primer bloque XOR el IV
			if(primer_bloque == 1){
			 	c_stream[i] = iv ^ bloque64;
				primer_bloque = 0;

			}
			else	// Los siguientes con el anterior bloque cifrado
				c_stream[i] = l_block ^ bloque64;
		}else if(modo == ECB){
			c_stream[i] = bloque64;
		}

		l_block = stream[i];

	}

	return c_stream;
}
/****************************************************************
 * Función que genera las 16 claves de 56 bits utilizadas en las
 * diferentes rondas del DES.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: generar_claves
 * Arguments: [0]-uint64_t clave--> Clave de 64bits utilizada para
 *			  cifrar. Contiene bits de paridad (8, 16, 24, 32, 40, 48,
 *			  56, 64). Se comprobará si la paridad es correcta.
 ****************************************************************/
uint64_t* generar_claves(uint64_t clave){

	int i = 0;
	// Array con las 16 claves reducidas y permutadas por PC-1 (Sin los bits de paridad)
	// (56b) generadas apartir de la clave original
	uint64_t* clave56 = NULL;
	// Variable auxiliar para liberar memoria
	uint64_t* p_aux = NULL;
	// Variable con los 28 bits de mayor peso de la clave
	uint32_t c0 = 0x0;
	// Variable con los 28 bits de menor peso de la clave
	uint32_t d0 = 0x0;

	// Reservamos memoria para las 16 claves
	clave56 = (uint64_t*)malloc((ROUNDS)*sizeof(uint64_t));
	if(clave56 == NULL){
		fprintf(stderr, "Error[177]: Se realizó mal la reserva de memoria de clave56\n");
		return NULL;
	}

	// Realizamos la PC1 sobre la clave (64b -> 56b)
	p_aux = (uint64_t*)permutar(&clave, TAM_CLAVE_BITS, PC1, BITS_IN_PC1);
	if(p_aux == NULL){
		fprintf(stderr, "Error[186]: Al llamar a permutar.\n");
		return NULL;
	}

	// Recogemos los 28 bits de mayor peso de la clave
	c0 = (*p_aux >> BITS_IN_PC1/2) & 0x0FFFFFFF;

	// Recogemos los 28 bits de menor peso de la clave
	d0 = *p_aux & 0x0FFFFFFF ;
	free(p_aux);

	// Bucle que crea las clave K1 a K16 con (c1 a c16 y d1 a d16)
	for(i = 0; i < ROUNDS; i++){
		// Realizamos la rotación segun nos diga el array ROUND_SHIFTS
		c0 = (c0 << ROUND_SHIFTS[i]) | (c0 >> (BITS_IN_PC1/2 - ROUND_SHIFTS[i]));
		d0 = (d0 << ROUND_SHIFTS[i]) | (d0 >> (BITS_IN_PC1/2 - ROUND_SHIFTS[i]));
		// Eliminamos los bits que sobran (Ya que no es una variable de 28bits si no de 32)
		c0 = c0 & 0x0FFFFFFF;
		d0 = d0 & 0x0FFFFFFF;

		// Juntamos Ci y Di en la clave56[i] (Limpiamos la variable clave56)
		// Y le metemos Ci y lo desplazamos para meter los 28bits de Di
		clave56[i] = clave56[i] & 0;
		clave56[i] = ((clave56[i] | c0) << (BITS_IN_PC1/2)) | (d0 & 0x0FFFFFFF);
	}

	return clave56;
}

/****************************************************************
 * Función que realiza una ronda del DES. Se le pasa el de 64 Bits
 * que se está cifrando y la clave correspondiente a la ronda.
 * Este generara la clave de 48bits con PC-2 y utilizara la funcion
 * feistel.
 *
 * Function: ronda
 *
 * Arguments: [0]-uint64_t bloque64--> Bloque de 64bits
 *			  [1]-uint64_t clave56--> Clave de 56 bits correspondiente
 *			  a la ronda.
 * Return: uint64_t Bloque de 64 bits.
 ****************************************************************/
uint64_t ronda(uint64_t bloque64, uint64_t clave56){

	// Clave de reducida y permutada por PC-2 (48b)
	uint64_t* clave48 = NULL;
	// 32bits de mayor peso de un bloque
	uint32_t l0 = 0x0;
	// Variable auxiliar para almacenar el valor de l0 antes de sobreescribirlo
	uint32_t aux_l0 = 0x0;
	// 32bits de menor peso de un bloque
	uint32_t r0 = 0x0;

	// Recogemos los 32 bits de mayor peso del bloque
	l0 = (bloque64 >> TAM_CLAVE_BITS/2);

	// Recogemos los 32 bits de menor peso de la bloque
	r0 = bloque64 & 0xFFFFFFFF;

	// Realizamos la PC2 sobre la clave (56b -> 48b)
	clave48 = (uint64_t*)permutar(&clave56, BITS_IN_PC1, PC2, BITS_IN_PC2);
	if(clave48 == NULL){
		fprintf(stderr, "Error[87]: Al llamar a permutar.\n");
		return -1;
	}

	// Hacemos el swap y la función feistel sobre r0 y l0
	// rn = ln-1 XOR feistel(rn, Kn) y ln = rn-1
	aux_l0 = l0;
	l0 = r0;
	r0 = aux_l0 ^ feistel(r0, *clave48);

	bloque64 = (bloque64 << TAM_CLAVE_BITS/2) | l0;
	bloque64 = (bloque64 << TAM_CLAVE_BITS/2) | r0;

	free(clave48);

	return bloque64;
}

/****************************************************************
 * Funcion Feistel (F). Funcion que se encarga de realizar la
 * confusion de la clave en el DES. Utiliza las SBOX para ello.
 * Recibe los 32bits de menor peso de los datos y la clave (48bits)
 * y devuelve 32 bits.
 *
 * Function: feistel
 * Arguments: [0]-uint32_t --> 32bits de menor peso de los datos
 * 			  [1]-uint64_t --> Clave de 48bits, tras PC-2
 * Return: uint32_t 32 bits con la clave confundida.
 ****************************************************************/
uint32_t feistel(uint32_t r0, uint64_t clave){

	// R0 Expandido (32b -> 48b)
	uint64_t* exp_r0 = NULL;
	// R0 Permutado
	uint32_t* p_r0 = NULL;
	uint64_t xor = 0;
	uint32_t s_box = 0;
	uint32_t result = 0 ;

	// Se realiza la expasion/permutacion (32b -> 48b)
	exp_r0 = (uint64_t*)permutar((uint64_t*)&r0, BITS_IN_P, E, BITS_IN_E);
	if(exp_r0 == NULL){
		fprintf(stderr, "Error[210]: Al llamar a permutar.\n");
		return -1;
	}

	// Hacemos la xor de r0 expandido y clave de 48bits
	xor = *exp_r0 ^ clave;

	// Se realizan las sustituciones de las SBox
	s_box = sbox(xor);

	p_r0 = (uint32_t*)permutar((uint64_t*)&s_box, BITS_IN_P, P, BITS_IN_P);
	if(p_r0 == NULL){
		fprintf(stderr, "Error[222]: Al llamar a permutar.\n");
		free(exp_r0);
		return -1;
	}
	result = *p_r0;

	free(p_r0);
	free(exp_r0);

	return result;
}
/****************************************************************
 * Función que realiza la sustituciones de las Sbox. Utiliza las
 * tablas de sustitución de DES_tables.h. Recibe 48bits de Datos
 * y los transforma en 32 bits.
 *
 * Function: sbox
 * Arguments: [0]-uint64_t --> Recibe 48 bits de datos
 * Return: uint32_t 32 bits de datos.
 ****************************************************************/
uint32_t sbox(uint64_t xor){
	uint8_t a1 = 0, a6 = 0;
	uint8_t s = 0;
	uint8_t fila = 0;
	uint8_t columna = 0;
	uint32_t result = 0;
	int i = 0;

	for(i = 0; i < NUM_S_BOXES; i ++){

		// Vamos cogiendo los 6 bits que vamos a sustituir
		s = xor >> (42 - i*6);
		// Solo cogemos los 6 ultimos bits
		s = s & 0x3F;

		// Recuperamos el bit de más peso de s (a1)
		a1 = (s & 0x20) >> 4;
		// Recuperamos el bit de menor peso de s (a6)
		a6 = s & 0x01;

		columna = (s & 0x1E) >> 1;
		fila = a1 | a6;

		// Añadimos el resultado de la sustitucion a result.
		result = result | S_BOXES[i][fila][columna];

		// Hacemos espacio para los 4 bits
		if(i != NUM_S_BOXES-1)
			result = result << 4;
	}

	return result;
}

/****************************************************************
 * Funcion que realiza las permutaciones del DES. Dado un bloque,
 * de tamaño entre 1 bit y 64 bits realiza las permutaciones de
 * la permutacion pasada como parametro. Puede realizar permutaciones
 * conjutamente con reducciones o con expansiones. Retorna un void*
 * por lo que es posible darle el tipo más deseado al usarla.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: permutar
 * Arguments: [0]-uint64_t* --> Bloque de 64 bits a permutar
 * 			  [1]-int --> Numero de bits del bloque a permutar
 * 			  [2]-const unsigned short* --> Array que contiene las
 *			  permutaciones a realizar sobre el bloque.
 *			  [3]-int --> Tamaño de la permutacion. (Indica el
 * 			  numero de bits de salida de la función.)
 * Return: void* Bits permutados. Puede ser tratado de la forma
 * 		   mas conveniente.
 ****************************************************************/
void* permutar(uint64_t* bloque, int nbits, const unsigned short* permutacion,	int p_size){

	int i = 0;
	// Variable auxiliar que permite la permutacionº
	unsigned long long int bit1 = 0x0;

	uint64_t* new_block = NULL;

	if(bloque == NULL || nbits < 0 || permutacion == NULL || p_size < 0){
		fprintf(stderr, "Error: Parametros incorrectos en la función permutar.\n");
		return NULL;
	}

	new_block = (uint64_t*)malloc(TAM_CLAVE_BYTES);
	if(new_block == NULL ){
		fprintf(stderr, "Error[286]: Se realizó mal la reserva de memoria de new_block.\n");
		return NULL;
	}
	*new_block = 0;

	// Bucle que realiza las permutaciones sobre los bits.
	for(i = 0; i < p_size; i++){
		// Bit que se va a permutar
	    bit1 = (*bloque >> (nbits - permutacion[i])) & 1;
		// Colocamos en el bloque destino el bit correspondiente
	    *new_block = *new_block | bit1;

		// Se va desplazando hacia la izquierda los bits que vamos metiendo
		if(i != p_size-1)
			*new_block = *new_block << 1;
	}

	return (void *)new_block;
}

/****************************************************************
 * Funcion que genera una clave aleatoria de 64 bits. La clave
 * tendra 56 bits de datos mas 8 bits de paridad impar. Los bits
 * de paridad ocuparan las posiciones 8, 16, 24, 32, 40, 48, 56
 * y 64.
 *
 * Function: generar_clave
 * Arguments: NONE
 * Return: uint64_t Clave de 64bits.
 ****************************************************************/
uint64_t generar_clave(){
	uint64_t clave = 0;
	int i = 0;
	// Cuenta el numero de bits a 1 en un byte
	int count = 0;

	set_seed((unsigned int)time(NULL));

	for(i = 0; i < TAM_CLAVE_BITS; i++){
		// Si es un bit de paridad
		if(!((i+1) % BITS_IN_BYTE)){
			// Comprueba si es 1 o 0
			if(count % 2 == 0){
				clave = clave | 1;
			}
			count = 0;
		}
		else{
			if(rand() % 2){
				clave = clave | 1;
				count++;
			}
		}
		// Se rotan los bits menos para el último
		if(i != TAM_CLAVE_BITS-1)
			clave = clave << 1;
	}

	return clave;
}

/****************************************************************
 * Funcion que genera un vector de inicializacion aleatorio de 64
 * bits. Éste se basa en la clave para generar más incertidumbre.
 *
 * Function: generar_iv
 * Arguments: [0]-uint64_t --> Clave utilizada para generar el iv
 * Return: uint64_t Vector de inicializacion.
 ****************************************************************/
uint64_t generar_iv(uint64_t clave){

	uint64_t iv = 0;
	int i = 0;

	srand(time(NULL) * clave);

	for(i = 0; i < TAM_CLAVE_BITS; i++){
		if(rand() % 2)
			iv = iv | 1;
		if(i != TAM_CLAVE_BITS-1)
			iv = iv << 1;
	}

	return iv;
}

/****************************************************************
 * Funcion que transforma un array de bytes en bloques de 64 Bits
 * y que realiza un padding en caso necesario. Se le pasa la
 * longitud en bytes como segundo argumento. El padding es
 * simplemente ceros por la derecha hasta que el tamaño del stream
 * sea multiplo de 8 (TAM_CLAVE_BYTES). Retorna un puntero hacia
 * la nueva memoria y devuelve por refeferencia el nuevo tamaño
 * de la variable.
 *
 * ATENCION! -> Sera necesario liberar memoria del return de la
 *				funcion.
 *
 * Function: strtob
 * Arguments: [0]-void* --> Bytes a transformar
 *			  [1]-int* --> Numero de bytes antes y despues de
 *			  hacer el padding.
 * Return: uint64_t* Array de bloques de 64 bits.
 ****************************************************************/
uint64_t* tandpad(void* bytes, int* nbytes){
	int i = 0;
	// Número de bloques de 64bits
	int nbloques = 0;
	// Bytes del padding
	int new_bytes = 0;
	// Indice del bloque
	int ib = 0;
	// Numero de bytes antes de hacer el padding
	int nbytesb4 = 0;
	int i_bloque = 0;
	uint8_t* p_bytes = (uint8_t*)bytes;
	uint64_t* binary = NULL;

	if (bytes == NULL || *nbytes < 0) {
		fprintf(stderr, "Error: Parametros de la función strtob incorrectos.\n");
		return NULL;
	}

	// Si ya es multiplo del tamaño del bloque pues no se necesitan más bytes
	if(*nbytes%TAM_CLAVE_BYTES == 0){
		new_bytes = 0;
		nbytesb4 = *nbytes;
	}
	else{ // En caso contrario se calculan los bytes para el padding
		new_bytes = (TAM_CLAVE_BYTES-(*nbytes%TAM_CLAVE_BYTES));
		// Guardamos los bytes antes del padding
		nbytesb4 = *nbytes;
		// Actualizamos el valor de bytes tras el padding
		*nbytes = nbytesb4+new_bytes;
	}
	// Calculamos el numero de bloques de 64 bits
	nbloques = *nbytes/TAM_CLAVE_BYTES;

	binary = (uint64_t*)malloc(nbloques*sizeof(uint64_t));
	if(binary == NULL ){
		fprintf(stderr, "Error[430]: Se realizó mal la reserva de memoria de binary.\n");
		return NULL;
	}

	// Bucle que recorre los bloques
	for(ib = 0; ib < nbloques; ib++){
		binary[ib] = 0;
		i_bloque = (ib+1)*TAM_CLAVE_BYTES;
		// Bucle que recorre los bytes. Va de 8 bytes en 8 bytes.
		// Pero sin pasarse del tamaño original.
		while(i < (i_bloque > nbytesb4 ? nbytesb4 : i_bloque)) {
			binary[ib] = binary[ib] | p_bytes[i];

			// Hace hueco para cada byte en el bloque.
			// Menos para el último que no necesita hacer hueco para más
			if(i != (i_bloque > nbytesb4 ? nbytesb4 : i_bloque)-1)
				binary[ib] = binary[ib] << BITS_IN_BYTE;
			i++;
	    }
	};

	// Padding en el ultimo bloque
	binary[nbloques-1] = binary[nbloques-1] << new_bytes * BITS_IN_BYTE;

	return binary;
}

/****************************************************************
 * Funcion que permite printear los bytes de una variable en
 * hexadecimal.
 *
 * ATENCION! -> Es importante saber que numero de bloques no
 * 			   sobrepase la memoria reservada de la variable.
 *
 * Function: print_hexa
 * Arguments: [0]-void* --> Stream de bytes
 *			  [1]-int --> Número de bloques a imprimir
 *			  [2]-int --> Tamaño de los bloques en bytes
 * Return: void* Array de bytes que representan cada letra.
 ****************************************************************/
void print_hexa(void* bytes, int nbloques, int tam_bloque){

	int i = 0, j = 1;

	uint8_t* p_bytes = (uint8_t*)bytes;
	if(bytes == NULL || nbloques < 0 || tam_bloque < 0){
		fprintf(stderr, "Error: Parametros de la función print_hexa incorrectos.\n");
		return;
	}

	printf("0x");
	for(i = 0; i < nbloques; i++){
		for(j = 1; j <= tam_bloque; j++){
			printf("%02X", p_bytes[(i+1)*tam_bloque-j]);
		}
	}
	printf("\n");
}

/****************************************************************
 * Funcion que permite printear por stdout bits de un stream.
 *
 * Function: print_bits
 * Arguments: [0]-size_t* --> Sizeof(Del tipo de la variable)
 *			  [1]-int --> Stream a convertir a bits
 * Return: NONE
 ****************************************************************/
void print_bits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
		printf(" ");
    }
    puts("");
}

uint64_t* tripleDES(uint64_t* stream, int nbytes, uint64_t** claves, uint64_t iv,
	MODO modo, int flag_cifrar){

    uint64_t* p_aux = NULL;
	uint64_t* p_aux2 = NULL;
	// Stream cifrado
	uint64_t* c_stream = NULL;
	int nbloques = 0;
	int i = 0;

	nbloques = nbytes/TAM_CLAVE_BYTES;
	c_stream = (uint64_t*)malloc(nbytes);

	if(flag_cifrar){
		for(i = 0; i < nbloques; i++){
	        p_aux = cifrar_des(&stream[i], TAM_CLAVE_BYTES, claves[0], iv, modo);
			l_block = 0x0;
	        p_aux2 = descifrar_des(p_aux, TAM_CLAVE_BYTES, claves[1], iv, modo);
			l_block = 0x0;
	        //free(p_aux);
	        p_aux = cifrar_des(p_aux2, TAM_CLAVE_BYTES, claves[2], iv, modo);
	        //free(p_aux2);
			c_stream[i] = *p_aux;
			//free(p_aux);
	    }
		return c_stream;
	}else{
		for(i = 0; i < nbloques; i++){
	        p_aux = descifrar_des(&stream[i], TAM_CLAVE_BYTES, claves[0], iv, modo);
			l_block = 0x0;
	        p_aux2 = cifrar_des(p_aux, TAM_CLAVE_BYTES, claves[1], iv, modo);
			l_block = 0x0;
	        //free(p_aux);
	        p_aux = descifrar_des(p_aux2, TAM_CLAVE_BYTES, claves[2], iv, modo);
	        //free(p_aux2);
			c_stream[i] = *p_aux;
			//free(p_aux);
	    }
		return c_stream;
	}

	return NULL;
}

/****************************************************************
 * Funcion que comprueba si la paridad impar de la clave se cumple
 * Esto permite saber si la clave fue modificada o se ha recibido
 * mal.
 *
 * Function: check_parity
 * Arguments: [0]-uint64_t Clave de 64 bits con 8 bits de paridad
 *			  impar, en las posiciones (8, 16 , 24, 32, 40, 48, 56, 64)
 * Return: int Retona 0 si no coincide 1 si todo está OK.
 ****************************************************************/
int check_parity(uint64_t clave){

	int i = 0;
	int count = 0;

	// Bucle que recorre todos los bits
	for(i = 63; i >= 0; i--){
		// Para el bit de paridad
		if(i % 8 == 0){
			if(1 & (clave >> i)){// Bit de paridad = 1
				if(count % 2) // Y la cuenta es impar. Entonces algo está mal
					return 0;
				else
					count = 0;
			}
			else{ // Bit de paridad = 0
				if(!(count % 2)) // Y la cuenta es par. Entonces algo está mal
					return 0;
				else
					count = 0;
			}
		}
		else if(1 & (clave >> i)){ // Cuando el bit sea un uno suma al contador
			count++;
		}
	}

	return 1;
}

void set_seed(unsigned int s){
	if(seed != 0)
		srand(s);
	return;
}
