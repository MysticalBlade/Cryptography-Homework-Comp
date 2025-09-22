//
// Created by annie on 9/21/25.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Helper absolute value function
static int64_t i64_abs(int64_t a) {
    return (a < 0) ? (int64_t)(-(uint64_t)a) : a;
}

// GCD main body
static int64_t i64_gcd(int64_t a, int64_t b) {
    a = i64_abs(a);
    b = i64_abs(b);
    if (a == 0) return b;
    if (b == 0) return a;
    while (b != 0) {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int main(int argc, char *argv[]) {
    int64_t a = (int64_t)strtoll(argv[1], NULL, 10);
    int64_t b = (int64_t)strtoll(argv[2], NULL, 10);
    printf("%lld", (long long)i64_gcd(a, b));
    return 0;
}
