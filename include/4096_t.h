#ifndef BIG4096_H
#define BIG4096_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define S ((size_t)(4096 / 64))
#define BYTES (S * sizeof(uint64_t))

uint64_t bigadd(uint64_t *a, uint64_t *b, uint64_t *sum);
uint64_t bigsub(uint64_t *a, uint64_t *b, uint64_t *diff);
uint64_t bigmul(uint64_t *a, uint64_t *b, uint64_t *out);
uint64_t bigquo(uint64_t *num, uint64_t *den, uint64_t *quo);
uint64_t bigrem(uint64_t *num, uint64_t *den, uint64_t *rem);

#endif
