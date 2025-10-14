//
// Created by annie on 10/2/25.
//

#include "shamir_helper.c"

int main(int argc, char *argv[]) {
    uint64_t secret, mod;
    read_secret_mod(argv[1], argv[2], &secret , &mod);
    generate_shares(secret , mod);
    return 0;
}