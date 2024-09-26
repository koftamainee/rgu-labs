#include "bits.h"
#include <math.h>

int null_bit(int n, int k) {
    return n & ~(1 << k);
}


int num_to_ones(int num) {
    int i, res = 0;
    for (i = 0; i < num; ++i) {
        res += pow(2, i);
    }
    return res;
}