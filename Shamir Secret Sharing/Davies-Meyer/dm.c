//
// Created by annie on 10/1/25.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include "RequiredFunctionsHW3.c"

// Helper for axing newline chars
static void unnewlined_hex(const unsigned char hex[16], const char *path) {
    char new_hex[32]; // + null terminator
    Convert_to_Hex(new_hex, (unsigned char *)hex, 16);
    FILE *file = fopen(path, "w");
    if (!file) return;
    fwrite(new_hex, 1, 32, file);
    fclose(file);
}

// 1 DM round: 16 bit prev hash as AES key, with another 16 bit msg block outs a 166b next hash

static void davies_meyer(const unsigned char prev[16], const unsigned char message[16], unsigned char next[16]) {
    AES_KEY key;
    AES_set_encrypt_key(message, 128, &key);
    unsigned char encrypted[16];
    AES_ecb_encrypt(prev, encrypted, &key, AES_ENCRYPT);
    for (int i = 0; i < 16; i++) {
       next[i] = (unsigned char) (encrypted[i] ^ prev[i]); // Xor
    }
}

int main(int argc, char *argv[]) {
    // Input
    int message_length_i = 0;
    unsigned char *message = Read_File(argv[1], &message_length_i);
    size_t message_length = (size_t) message_length_i;

    // Counting numbers of blocks, and padding
    size_t blocks = (message_length == 0) ? 1 : ((message_length + 15) / 16);

    unsigned char H_prev[16] = {0}; // Initialize the zeroth hash H_0
    unsigned char H_current[16];
    unsigned char H_first[16]; // H_1
    unsigned char H_last[16]; // H_i


    for (size_t i = 0; i < blocks; i++) {
        unsigned char block[16];
        if (i < blocks - 1) {
            memcpy(block, message + i * 16, 16);
        } else {
            size_t offset = i * 16;
            size_t remaining = 0;
            if (message_length > offset) {
                remaining = message_length - offset;
            }
            if (remaining >= 16) {
                memcpy(block, message + i * 16, 16);
            } else {
                memset(block, 0, 16);
                if (remaining) {
                    memcpy(block, message + i * 16, remaining);
                }
            }
        }

        davies_meyer(H_prev, block, H_current);
        if (i == 0) {
            memcpy(H_first, H_current, 16);
        }
        memcpy(H_prev, H_current, 16);
    }

    // Final value
    memcpy(H_last, H_current, 16);

    // Outputting
    unnewlined_hex(H_first, "FirstHash.txt");
    unnewlined_hex(H_last, "FinalHash.txt");

    free(message);
    return 0;
}
