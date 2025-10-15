//
// Created by annie on 10/10/25.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include "RequiredFunctionsEGECDSA.c"

// Trimming inputs (i have learnt my lesson)
static void trim(char* s)
{
    if (!s) return;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
    {
        s[len - 1] = '\0';
        len--;
    }
}

int main(int argc, char *argv[])
{
    // Input and trim
    int throwaway; // we call strlen later
    char *pk_hex = Read_File(argv[1], &throwaway);
    char *sk_hex = Read_File(argv[2], &throwaway);
    char *k_hex = Read_File(argv[3], &throwaway);
    char *msg_hex = Read_File(argv[4], &throwaway);

    trim(pk_hex);
    trim(sk_hex);
    trim(k_hex);
    trim(msg_hex);

    // EC setting up
    BN_CTX *ctx = BN_CTX_new();
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp192k1);
    EC_GROUP_set_point_conversion_form(ec_group, POINT_CONVERSION_UNCOMPRESSED);

    // Parsing
    BIGNUM *k;
    BN_hex2bn(&k, k_hex);
    EC_POINT *Y = EC_POINT_hex2point(ec_group,pk_hex, NULL, ctx);
    EC_POINT *Pm = EC_POINT_hex2point(ec_group,msg_hex, NULL, ctx);

    // ElGamal
    // C = kG, C' = kY, D = C' + msg

    // TODO: ElGamal, ECDSA Sig
}