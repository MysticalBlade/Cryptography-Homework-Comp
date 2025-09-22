cmake_minimum_required(VERSION 3.29)
project(CIS4930_HW2 C)

set(CMAKE_C_STANDARD 11)

add_executable(CIS4930_HW2
        SNM_GMP/RequiredFunctionsHW2.c
        SNM_GMP/snm_gmp.c
        GCD/RequiredFunctionsHW2.c
        GCD/gcd.c)
