//
// Created by annie on 10/1/25.
//

#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include "RequiredFunctionsHW3.c"


int main(int argc, char *argv[]) {

    // Loading the seed
    int seed_len = 0;
    const unsigned char *seed = Read_File(argv[1], &seed_len);

    // Input
    unsigned char messages[NUMBER_OF_MESSAGES][LENGTH_OF_EACH_MESSAGE] = {0};
    Read_Multiple_Lines_from_File(argv[2], messages, NUMBER_OF_MESSAGES);

    FILE *keys = fopen("Keys.txt", "w");
    FILE *ciphertexts = fopen("Ciphertexts.txt", "w");

    unsigned char key[32];
    SHA256(seed, seed_len, key);

    for (int i = 0; i < NUMBER_OF_MESSAGES; i++) {
        char ciphertext_hex[LENGTH_OF_EACH_MESSAGE * 2 + 1];
        char key_hex[65];
        unsigned char ciphertext[LENGTH_OF_EACH_MESSAGE];

        // AES-CTR would encrypt a 64b line with key here
        AES256CTR_Encrypt(key, messages[i], LENGTH_OF_EACH_MESSAGE, ciphertext);

        // Output
        Convert_to_Hex(key_hex, key, 32);
        Convert_to_Hex(ciphertext_hex, ciphertext, LENGTH_OF_EACH_MESSAGE);
        fprintf(keys, "%.*s", 64, key_hex);
        fprintf(ciphertexts, "%.*s", LENGTH_OF_EACH_MESSAGE * 2, ciphertext_hex);
        if (i + 1 < NUMBER_OF_MESSAGES) {
            fputc('\n', keys);
            fputc('\n', ciphertexts);
        }

        // Advancing the hash chain
        unsigned char next_key[32];
        SHA256(key, 32, next_key);
        memcpy(key, next_key, 32);
    }

    fclose(keys);
    fclose(ciphertexts);
    return 0;
    }