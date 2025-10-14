//
// Created by annie on 9/18/25.
//

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

#include "RequiredFunctionsHW2.c"

static void snm(const mpz_t base, const mpz_t exp, const mpz_t mod, mpz_t out) {

    // Setup
    mpz_t a, r;
    mpz_init(a);
    mpz_init(r);


    // a = base % mod
    // r = 1
    mpz_mod(a, base, mod);
    mpz_set_ui(r, 1);


    // Fast out
    if (mpz_cmp_ui(mod, 1) == 0) {
        mpz_set_ui(out, 0);
        return;
    };

    if (mpz_cmp_ui(exp, 0) == 0) {
        mpz_set(out, r);
        return;
    }

    // SNM main loop
    size_t nbits = mpz_sizeinbase(exp, 2);
    for (ssize_t i = (ssize_t)nbits - 1; i >= 0; i--) {

        // r = r^2 mod mod
        mpz_mul(r, r, r);
        mpz_mod(r, r, mod);

        if (mpz_tstbit(exp, (mp_bitcnt_t)i)) {
            // r = r * a mod mod
            mpz_mul(r, r, a);
            mpz_mod(r, r, mod);
        }
    }
    mpz_set(out, r);
    // Freeing memory
    mpz_clear(a);
    mpz_clear(r);
}

int main(int argc, char *argv[]) {

    // Reading inputs and initialization of numbers
    int base_len = 0, exp_len = 0, mod_len = 0;
    unsigned char *base_buf = Read_File(argv[1], &base_len);
    unsigned char *exp_buf  = Read_File(argv[2], &exp_len);
    unsigned char *mod_buf  = Read_File(argv[3], &mod_len);

    mpz_t base, exp, mod, res;
    mpz_inits(base, exp, mod, res, NULL);

    // Err catching
    if (!(mpz_set_str(base, (const char*)base_buf, 10) == 0 &&
          mpz_set_str(exp,  (const char*)exp_buf,  10) == 0 &&
          mpz_set_str(mod,  (const char*)mod_buf,  10) == 0)) {
        fprintf(stderr, "Error: failed to parse one of Base/Exponent/Modulo as decimal\n");
        exit(1);
      }

    // Using snm
    snm(base, exp, mod, res);
    char *res_str = mpz_get_str(NULL, 10, res);

    Write_File("Result.txt", res_str);
    free(res_str);
    mpz_clears(base, exp, mod, res, NULL);
    free(exp_buf);
    free(mod_buf);
    free(base_buf);
    return 0;

}

