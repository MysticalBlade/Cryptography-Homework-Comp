/*
Created by Andy Pham (64937705) on 9/23/25
*/

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RequiredFunctionsHW2.c"


static void eea_implement(const mpz_t a, const mpz_t b, mpz_t x, mpz_t y, mpz_t g) {
    mpz_t prev_r, prev_s, prev_t, r, s, t, q, temp;

    mpz_inits(prev_r, prev_s, prev_t, r, s, t, q, temp, NULL);
    //set up values
    mpz_set(prev_r, a);
    mpz_set(r, b);
    mpz_set_ui(prev_s, 1);
    mpz_set_ui(s, 0);
    mpz_set_ui(prev_t, 0);
    mpz_set_ui(t, 1);
    // main extended euclied loop
    while (mpz_sgn(r) != 0) {
        // floor prev_r by r
        mpz_fdiv_q(q, prev_r, r);
        // update temp to r, then update r = prevr - q*r then update prevr = temp
        mpz_set(temp, r);
        mpz_mul(r, q, r);
        mpz_sub(r, prev_r, r);
        mpz_set(prev_r, temp);
        // update temp to s, then update s = prevs - q*s then updaye prevs = temp
        mpz_set(temp, s);
        mpz_mul(s, q, s);
        mpz_sub(s, prev_s, s);
        mpz_set(prev_s, temp);
        // update temp to t, then update t = prevt - q*t then update prevt
        mpz_set(temp, t);
        mpz_mul(t, q, t);
        mpz_sub(t, prev_t, t);
        mpz_set(prev_t, temp);
    }
    // x=prevs y=prevt g=prevr
    mpz_set(x, prev_s);
    mpz_set(y, prev_t);
    mpz_set(g, prev_r);
}

int main(int argc, char *argv[]) {
    int num_lenth;
    int mod_lenth;
    unsigned char *num_str = Read_File(argv[1], &num_lenth);
    unsigned char *mod_str = Read_File(argv[2], &mod_lenth);
    // gmp variables
    mpz_t num, mod, x, y, g_res;
    mpz_inits(num, mod, x, y, g_res, NULL);
    mpz_set_str(num, (const char*)num_str, 10);
    mpz_set_str(mod, (const char*)mod_str, 10);
    eea_implement(num, mod, x, y, g_res);
    mpz_mod(x, x, mod);
    Write_File("Result.txt", mpz_get_str(NULL, 10, x));
    // freeing mem
    free(num_str);
    free(mod_str);
    mpz_clears(num, mod, x, y, g_res, NULL);
    return 0;
}