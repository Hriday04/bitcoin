#include "4096_t.h"

/* Addition of two 4096-bit integers */
uint64_t bigadd(uint64_t *in0, uint64_t *in1, uint64_t *sum) {
    uint64_t carry = 0;
    size_t i;

    for (i = 0; i < S; i++) {
        uint64_t temp = in0[i] + in1[i] + carry;
        if (temp != in0[i]) {
            carry = (temp < in0[i]) ? 1 : 0;
        }
        sum[i] = temp;
    }
    return carry;
}

/* Subtraction of two 4096-bit integers */
uint64_t bigsub(uint64_t *min, uint64_t *sub, uint64_t *dif) {
    uint64_t borrow = 0;
    size_t i;

    for (i = 0; i < S; i++) {
        uint64_t temp = min[i] - sub[i] - borrow;
        if (temp != min[i]) {
            borrow = (temp > min[i]) ? 1 : 0;
        }
        dif[i] = temp;
    }
    return borrow;
}

/* Multiplication of two 4096-bit integers */
uint64_t bigmul(uint64_t *in0, uint64_t *in1, uint64_t *out) {
    uint64_t temp[2 * S];
    size_t i, j;
    uint64_t a, b, al, ah, bl, bh;
    uint64_t low, high, mid1, mid2, mid;

    memset(temp, 0, sizeof(temp));

    for (i = 0; i < S; i++) {
        for (j = 0; j < S; j++) {
            if (i + j < 2 * S) {
                a = in0[i];
                b = in1[j];
                al = (uint32_t)a;
                ah = a >> 32;
                bl = (uint32_t)b;
                bh = b >> 32;

                low = al * bl;
                high = ah * bh;
                mid1 = al * bh;
                mid2 = ah * bl;

                mid = mid1 + mid2;
                if (mid < mid1) {
                    high += ((uint64_t)1 << 32);
                }

                high += (mid >> 32);
                mid <<= 32;

                low += mid;
                if (low < mid) {
                    high++;
                }

                temp[i + j] += low;
                if (temp[i + j] < low) {
                    high++;
                }

                if (i + j + 1 < 2 * S) {
                    temp[i + j + 1] += high;
                }
            }
        }
    }

    memcpy(out, temp, S * sizeof(uint64_t));
    return 0;
}

/* Division of two 4096-bit integers */
uint64_t bigdiv(uint64_t *num, uint64_t *den, uint64_t *quo, uint64_t *rem) {
    size_t i, word_index, bit_index, j;
    uint64_t bit;

    memset(quo, 0, BYTES);
    memset(rem, 0, BYTES);

    for (i = S * 64; i > 0; i--) {
        word_index = (i - 1) / 64;
        bit_index = (i - 1) % 64;

        bit = (num[word_index] >> bit_index) & 1;

        for (j = 0; j < S - 1; j++) {
            rem[j] = (rem[j] << 1) | ((rem[j + 1] >> 63) & 1);
        }
        rem[S - 1] = (rem[S - 1] << 1) | bit;

        if (memcmp(rem, den, BYTES) >= 0) {
            (void)bigsub(rem, den, rem);
            quo[word_index] |= ((uint64_t)1 << bit_index);
        }
    }

    return 0;
}

/* Quotient only */
uint64_t bigquo(uint64_t *num, uint64_t *den, uint64_t *quo) {
    uint64_t rem[S];
    return bigdiv(num, den, quo, rem);
}

/* Remainder only */
uint64_t bigrem(uint64_t *num, uint64_t *den, uint64_t *rem) {
    uint64_t quo[S];
    return bigdiv(num, den, quo, rem);
}
