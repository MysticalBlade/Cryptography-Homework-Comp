//
// Created by annie on 10/1/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "RequiredFunctionsHW3.c"

int main(int argc, char *argv[]) {

    // Loading the seed
    int seed_len = 0;
    unsigned char *seed = Read_File(argv[1], &seed_len);

    // Reading alice's hex
    char ciphertext_hex[NUMBER_OF_MESSAGES][LENGTH_OF_EACH_MESSAGE * 2 + 2];
    FILE *input = fopen(argv[2], "r");

    for (int i = 0; i < NUMBER_OF_MESSAGES; i++) {
        if (!fgets(ciphertext_hex[i], sizeof ciphertext_hex[i], input)) {
            ciphertext_hex[i][0] = '\0';
        }
        ciphertext_hex[i][strcspn(ciphertext_hex[i], "\r\n") ] = '\0'; // Trimming
    }

    fclose(input);

    // Getting plaintext
    FILE *plaintext = fopen("Plaintexts.txt", "w");

    // Getting key
    unsigned char key[32];
    SHA256(seed, seed_len, key);

    // Decryption
    for (int i = 0; i < NUMBER_OF_MESSAGES; i++) {
        unsigned char plain[LENGTH_OF_EACH_MESSAGE];
        unsigned char ciphertext[LENGTH_OF_EACH_MESSAGE];
        Convert_to_Uchar(ciphertext_hex[i], ciphertext, LENGTH_OF_EACH_MESSAGE);

        AES256CTR_Decrypt(key, ciphertext, LENGTH_OF_EACH_MESSAGE, plain);

        fwrite(plain, 1, LENGTH_OF_EACH_MESSAGE, plaintext);
        if (i + 1 < NUMBER_OF_MESSAGES) {
            fputc('\n', plaintext);
        }
        // Same hash chain
        unsigned char next_key [32];
        SHA256(key, 32, next_key);
        memcpy(key, next_key, 32);
    }
    fclose(plaintext);
    return 0;
}