cmake_minimum_required(VERSION 3.29)
project(CIS4930_HW2 C)

set(CMAKE_C_STANDARD 11)

add_executable(CIS4930_HW2
        "Pollard Rho/SNM_GMP/RequiredFunctionsHW2.c"
        "Pollard Rho/SNM_GMP/snm_gmp.c"
        "Pollard Rho/GCD/RequiredFunctionsHW2.c"
        "Pollard Rho/GCD/gcd.c"
        "Shamir Secret Sharing/RequiredFunctionsHW3.c"
        "Shamir Secret Sharing/ChainCrypt/alice.c"
        "Shamir Secret Sharing/ChainCrypt/bob.c"
        "Shamir Secret Sharing/Davies-Meyer/dm.c"
        "Shamir Secret Sharing/Davies-Meyer/RequiredFunctionsHW3.c"
        "Shamir Secret Sharing/ShamirSecretSharing/shamirgen.c"
        "Shamir Secret Sharing/ShamirSecretSharing/shamirrecon.c"
        "Shamir Secret Sharing/ShamirSecretSharing/RequiredFunctionsHW3.c"
        "Shamir Secret Sharing/ShamirSecretSharing/shamir_helper.c"
        ECDH/RequiredFunctionsEGECDSA.c)
