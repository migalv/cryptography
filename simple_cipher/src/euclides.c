
#include "../includes/euclides.h"

void euclides_gmp(mpz_t a, mpz_t b, mpz_t d, mpz_t s, mpz_t t){

    mpz_t d_aux, s_aux, t_aux, modulo, cociente, producto;
    mpz_init (d_aux);
    mpz_init (s_aux);
    mpz_init (t_aux);
    mpz_init (modulo);
    mpz_init (cociente);
    mpz_init (producto);

    if(mpz_cmp_ui(b,(unsigned long)0) == 0){
        mpz_set(d, a);
        mpz_set_si(s, 1);
        mpz_set_si(t, 0);

        mpz_clear (d_aux);
        mpz_clear (s_aux);
        mpz_clear (t_aux);
        mpz_clear (modulo);
        mpz_clear (cociente);
        mpz_clear (producto);
        return;
    }

    mpz_mod(modulo, a, b);
    euclides_gmp(b, modulo, d, s, t);

    mpz_set(d_aux, d);
    mpz_set(s_aux, t);

    mpz_div(cociente, a, b);
    mpz_mul(producto, cociente, t);
    mpz_sub(t_aux, s, producto);


    mpz_set(d, d_aux);
    mpz_set(s, s_aux);
    mpz_set(t, t_aux);

    mpz_clear (d_aux);
    mpz_clear (s_aux);
    mpz_clear (t_aux);
    mpz_clear (modulo);
    mpz_clear (cociente);
    mpz_clear (producto);

    return;
}

void euclides(int a, int b, int* d, int* s, int* t){

    int modulo = 0, s_aux = 0;

    if(b == 0){
        *d = a;
        *s = 1;
        *t = 0;
        return;
    }

    modulo = a%b;

    euclides(b, modulo, d, s, t);

    s_aux = *t;

    *(t) = (*s) - a/b * (*t);

    *s = s_aux;

    return;
}