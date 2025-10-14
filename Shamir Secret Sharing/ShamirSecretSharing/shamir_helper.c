//
// Created by annie on 10/2/25.
//

// As the 2 files share too many functions, I've decided to write a helper file and keep shamirgen and shamirrecon as short as possible

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <openssl/rand.h>
#include "RequiredFunctionsHW3.c"

#define NUMBER_OF_SHARES 5
#define THRESHOLD 3

// uint64_t functions

// Read File
static uint64_t uint64_from_file(char* filename) {
    int len = 0;
    unsigned char *buffer = Read_File(filename, &len);
    if (buffer == NULL) {return 0;}

    uint64_t accumulator = 0;
    for (int i = 0; i < len; i++) {
        unsigned char c = buffer[i];
        if (c >= '0' && c <= '9') {
            accumulator = accumulator * 10 + (c - '0');
        }
    }
    free(buffer);
    return accumulator;
}

// Write File
static void uint64_to_file(uint64_t value, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {return;}
    fprintf(file, "%" PRIu64 , value);
    fclose(file);
}

// Write share (x, y) to Share_.txt
static void share_to_file(uint64_t index, uint64_t x, uint64_t y) {
    char name[64];
    snprintf(name, sizeof(name), "Share%" PRIu64 ".txt", index);
    FILE* file = fopen(name, "w");
    if (file == NULL) {return;}
    fprintf(file, "%" PRIu64 "\n", x);
    fprintf(file, "%" PRIu64 "\n", y);
    fclose(file);
}

// Read share (x, y) from Share_.txt
static int share_from_file(uint64_t index, uint64_t *x, uint64_t *y) {
    char name[64];
    snprintf(name, sizeof(name), "Share%" PRIu64 ".txt", index);
    FILE* file = fopen(name, "r");
    if (file == NULL) {return -1;}

    char line[64];

    // x from file
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return -1;
    }
    uint64_t x_accumulator = 0;
    for (char *p = line; *p; p++) {
        if (*p >= '0' && *p <= '9') {
            x_accumulator = x_accumulator * 10 + (*p - '0');
        }
    }

    // y from file
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return -1;
    }
    uint64_t y_accumulator = 0;
    for (char *p = line; *p; p++) {
        if (*p >= '0' && *p <= '9') {
            y_accumulator = y_accumulator * 10 + (*p - '0');
        }
    }

    fclose(file);
    if (x) *x = x_accumulator;
    if (y) *y = y_accumulator;
    return 0;
}


// Modular Arithmetic
// x, y are numbers, z is the modulus
static uint64_t mod_add(uint64_t x, uint64_t y, uint64_t z) {
    x %= z;
    y %= z;
    uint64_t result = x + y;
    if (result >= z) result -= z;
    return result;
}

static uint64_t mod_subtract(uint64_t x, uint64_t y, uint64_t z) {
    uint64_t result;
    if (x >= y) {
        result = x - y;
    } else {
        result = x - y + z;
    }
    return result;
}

static uint64_t mod_multiply(uint64_t x, uint64_t y, uint64_t z) {
    __uint128_t result = x * y;
    return (uint64_t)(result % z);
}

static uint64_t mod_exp(uint64_t x, uint64_t y, uint64_t z) {
    uint64_t result = 1 % z;
    uint64_t base = x % z;
    while (y) {
        if (y & 1) {
            result = mod_multiply(result, base, z);
        }
        base = mod_multiply(base, base, z);
        y >>= 1;

    }
    return result;
}

static uint64_t mod_inverse(uint64_t x, uint64_t z) {
    return mod_exp(x % z, z - 2, z);
}

// PRNG using OpenSSL
static uint64_t rand_mod(uint64_t mod) {
    uint64_t result = 0;
    RAND_bytes((unsigned char *)&result, sizeof(result));
    if (mod == 0) {
        return 0;
    } else {
        return result % mod;
    }
}

// Horner's Method for Polynomial Evaluation
static uint64_t horner_evaluation(uint64_t x, uint64_t *a, int degree, uint64_t mod) {
    uint64_t result = 0;
    for (int i = degree; i >= 0; i--) {
        result = mod_multiply(result, x, mod);
        result = mod_add(result, a[i], mod);
    }
    return result;
}

// Shamir Coeffs
static void shamir_coefficient(uint64_t *a, int t, uint64_t secret, uint64_t mod) {
    a[0] = secret % mod;
    for (int i = 1; i < t; i++) {
        a[i] = rand_mod(mod);
    }
}

// Lagrange basis for x = 0 given an xs from 0 to t-1
static uint64_t lagrange(uint64_t *xs, uint64_t mod, int i, int t) {
    uint64_t numerator = 1 % mod;
    uint64_t denominator = 1 % mod;
    for (int j = 0; j < t; j++) {
        if (j == i) continue;
        uint64_t xj;
        if (xs[j] == 0) {
            xj = 0;
        } else {
            xj = mod - xs[j];
        }
        numerator = mod_multiply(numerator, xj, mod);
        uint64_t diff = mod_subtract(xs[i], xs[j], mod);
        denominator = mod_multiply(denominator, diff, mod);
    }
    return mod_multiply(numerator, mod_inverse(denominator, mod), mod);
}

// Wrappers
// Read secret and mod
static void read_secret_mod(char *secret_path, char *mod_path, uint64_t *secret, uint64_t *mod) {
    *secret = uint64_from_file(secret_path);
    *mod = uint64_from_file(mod_path);
}

// Generate shares
static void generate_shares(uint64_t secret, uint64_t mod) {
    uint64_t coeffs[THRESHOLD];
    shamir_coefficient(coeffs, THRESHOLD, secret, mod);

    for (uint64_t i = 1; i <= NUMBER_OF_SHARES; i++) {
        const uint64_t x = i;                               // nonzero, distinct
        const uint64_t y = horner_evaluation(x, coeffs, THRESHOLD - 1, mod);
        share_to_file(i, x, y);
    }
}


// Reconstruct Secret from threshold shares
static void reconstruction(uint64_t mod) {
    uint64_t x[THRESHOLD], y[THRESHOLD];

    for (uint64_t i = 0; i < THRESHOLD; i++) {
        if (share_from_file(i+1, &x[i], &y[i]) != 0) {
            uint64_to_file(0, "Recovered.txt");
            return;
        }
        x[i] %= mod;
        y[i] %= mod;
    }
    uint64_t secret = 0;
    for (int i = 0; i < THRESHOLD; i++) {
        uint64_t l_0 = lagrange(x, mod, i, THRESHOLD);
        uint64_t term = mod_multiply(y[i], l_0, mod);
        secret = mod_add(secret, term, mod);
    }
    uint64_to_file(secret, "Recovered.txt");
}











































