
#include "../include/sbox_aes.h"


int rango_num(int16_t num){

    int16_t aux;
    int i=0;

    for(i=8; i>=0; i--){
        aux = num & (int16_t)pow (2,i);
        aux >>= i;
        if(aux==1){
            return i;
        }
    }

    return 0;
}

void division(uint16_t dividendo, uint16_t divisor, uint16_t *cociente, uint16_t *resto) {

    int16_t divisor_aux = 0;
    int rangoDividendo = 0;
    int rangoDivisor = 0;
    int desplazamiento = 0;

    *resto = dividendo;
    *cociente=0;
    rangoDividendo = rango_num(dividendo);
    rangoDivisor = rango_num(divisor);

    desplazamiento = (rangoDividendo-rangoDivisor)+1;

    while(*resto>divisor || desplazamiento != 0){
        divisor_aux = divisor;


        rangoDividendo = rango_num(*resto);
        rangoDivisor = rango_num(divisor_aux);

        if(rangoDividendo >= rangoDivisor){
            divisor_aux <<= (rangoDividendo-rangoDivisor);
        }

        rangoDivisor = rango_num(divisor_aux);

        if(rangoDividendo >= rangoDivisor){
            if(*resto != 0){
                *resto = *resto ^ divisor_aux;
                *cociente <<= 1;
                *cociente |= 1;
            }
            else{
                *cociente <<= 1;
                *cociente |= 0;
            }
        }else{
            *cociente <<= 1;
            *cociente |= 0;
        }
        desplazamiento--;

    }
}

int16_t xtime(int16_t num){

    uint16_t aux = 0;

    if(rango_num(num) != 7){
        aux = num << 1;
        aux &= 255;
    }
    else{
        aux = (num <<= 1)^27;// '1B' = 27 en base 10
        aux &= 255;
    }

    return aux;
}

uint16_t multiplicar(int16_t num1, int16_t num2){

    int i = 0;
    uint16_t xtimes[8];
    int16_t aux;
    uint16_t total = 0;

    for(i=0; i<8; i++){
        if(i==0){
            xtimes[i] = num1;
        }
        else{
            xtimes[i] = xtime(xtimes[i-1]);
        }
    }

    for(i=0; i<8; i++){
        aux = num2 & (int16_t)pow (2,i);
        aux >>= i;
        if(aux==1){
            total ^= xtimes[i];
        }
    }

    return total;
}

void euclides(uint16_t a, uint16_t b, uint16_t* d, uint16_t* s, uint16_t* t){

    uint16_t modulo = 0, s_aux = 0;
    uint16_t q = 0, c = 0;

    if(b == 0){
        *d = a;
        *s = 1;
        *t = 0;
        return;
    }

    division(a, b, &q, &modulo);

    euclides(b, modulo, d, s, t);

    s_aux = *t;

    division(a, b, &c, &modulo);
    *(t) = (*s) ^ multiplicar(c, (*t));

    *s = s_aux;

    return;
}
uint8_t* multiplicar_matrices(uint8_t x[8][8], uint8_t s[8]){

    uint8_t* mult;
    int i = 0, j = 0;
    int8_t aux;

    mult = (uint8_t*)malloc(sizeof(uint8_t)*8);

    for(i = 0; i < 8; i++){
        for(j = 0, aux = 0; j < 8; j++){
            aux += x[i][j]*s[j];
        }

        aux %= 2;
        mult[i] = aux;
    }
    return mult;
}
uint16_t transformacion_afin_directa(uint16_t n){

    uint16_t aux = 0;
    uint8_t s[8] = {0};
    uint8_t *s_aux;
    uint8_t c[8] = {1, 1, 0, 0, 0, 1, 1, 0};
    uint8_t afin[8];
    int i = 0;
    uint8_t x[8][8] = {
            {1, 0, 0, 0, 1, 1, 1, 1},
            {1, 1, 0, 0, 0, 1, 1, 1},
            {1, 1, 1, 0, 0, 0, 1, 1},
            {1, 1, 1, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 1, 1, 1, 1, 1},
            };

    for(i = 0; i<8; i++){
        aux = n & (int16_t)pow (2,i);
        aux >>= i;
        s[i] = aux;
    }

    s_aux = multiplicar_matrices(x, s);
    for(i = 0; i<8; i++){
        afin[i] = s_aux[i] ^ c[i];
        //printf("%" PRIu16 " ", afin[i]);
    }
    free(s_aux);

    for(i = 0, aux = 0; i<=8; i++){
        aux <<= 1;
        aux |= afin[8-i];
    }

    return aux;
}

uint16_t transformacion_afin_inversa(uint16_t n){

    uint16_t aux = 0;
    uint8_t s[8] = {0};
    uint8_t *s_aux;
    uint8_t d[8] = {1, 0, 1, 0, 0, 0, 0, 0};
    uint8_t afin[8];
    int i = 0;
    uint8_t x[8][8] = {
            {0, 0, 1, 0, 0, 1, 0, 1},
            {1, 0, 0, 1, 0, 0, 1, 0},
            {0, 1, 0, 0, 1, 0, 0, 1},
            {1, 0, 1, 0, 0, 1, 0, 0},
            {0, 1, 0, 1, 0, 0, 1, 0},
            {0, 0, 1, 0, 1, 0, 0, 1},
            {1, 0, 0, 1, 0, 1, 0, 0},
            {0, 1, 0, 0, 1, 0, 1, 0},
            };

    for(i = 0; i<8; i++){
        aux = n & (int16_t)pow (2,i);
        aux >>= i;
        s[i] = aux;
    }

    s_aux = multiplicar_matrices(x, s);
    for(i = 0; i<8; i++){
        afin[i] = s_aux[i] ^ d[i];
    }
    free(s_aux);

    for(i = 0, aux = 0; i<=8; i++){
        aux <<= 1;
        aux |= afin[8-i];
    }

    return aux;
}

void print_hexa(void* bytes, int nbloques, int tam_bloque, FILE * f_out){

	int i = 0, j = 1;

	uint8_t* p_bytes = (uint8_t*)bytes;
	if(bytes == NULL || nbloques < 0 || tam_bloque < 0){
		fprintf(stderr, "Error: Parametros de la función print_hexa incorrectos.\n");
		return;
	}

	for(i = 0; i < nbloques; i++){
		for(j = 1; j <= tam_bloque; j++){
            if(f_out){
                fprintf(f_out, "%02X", p_bytes[(i+1)*tam_bloque-j]);
            }
            else{
                printf("%02X", p_bytes[(i+1)*tam_bloque-j]);
            }
		}
	}
}
