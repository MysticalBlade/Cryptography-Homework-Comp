//
// Created by annie on 10/2/25.
//

#include "shamir_helper.c"
int main(int argc, char *argv[]) {
    uint64_t mod = uint64_from_file(argv[1]);
    reconstruction(mod);
    return 0;
}