//
// Created by ampharos on 10/10/25.
//
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
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

int main (int argc, char *argv[])
{
    // Input and trimming
    int seed_file_len = 0;
    char *seed_file = Read_File(argv[1], &seed_file_len);


    // y = SHA256(seed) mod n
    unsigned char digest[SHA256_DIGEST_LENGTH];
    const size_t seed_len = strlen(seed_file);
    SHA256((const unsigned char*)seed_file, seed_len, digest);
    free(seed_file);

    BN_CTX *bn_ctx = BN_CTX_new();

    // Create a group and ensure uncompressed conversion for the hex
    EC_GROUP *ec_group = EC_GROUP_new_by_curve_name(NID_secp192k1);
    EC_GROUP_set_point_conversion_form(ec_group, POINT_CONVERSION_UNCOMPRESSED);

    // Group order
    BIGNUM *order = BN_new();
    EC_GROUP_get_order(ec_group, order, bn_ctx);

    // Digest gets turned into scalar and modded
    BIGNUM *y = BN_bin2bn(digest, SHA256_DIGEST_LENGTH, NULL);
    BN_mod(y, y, order, bn_ctx);
    if (BN_is_zero(y))
    {
        BN_one(y);
    }

    // Y = y * G
    EC_POINT *point = EC_POINT_new(ec_group);
    EC_POINT_mul(ec_group, point, y,NULL, NULL, bn_ctx);

    // Serialize and write to SK.txt and PK.txt
    char *sk_hex = BN_bn2hex(y);
    char *pk_hex = EC_POINT_point2hex(ec_group, point, POINT_CONVERSION_UNCOMPRESSED, bn_ctx);

    Write_File("SK.txt", sk_hex);
    Write_File("PK.txt", pk_hex);

    // Housekeeping
    OPENSSL_free(sk_hex);
    OPENSSL_free(pk_hex);
    EC_POINT_free(point);
    BN_free(order);
    BN_free(y);
    EC_GROUP_free(ec_group);
    BN_CTX_free(bn_ctx);
    return 0;
}