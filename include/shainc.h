#ifndef SHAINC_H
#define SHAINC_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t hash[32];
} hash_t;

hash_t sha256(const void *data, size_t len);

#endif
