
#ifndef BTCINC_H
#define BTCINC_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t hash[32];
} hash_t;

typedef struct {
    char data[256];
    hash_t sig;
} signtx;

typedef struct {
    size_t num_tx;
    hash_t *merkle;
    signtx *txtree;
} block_t;

hash_t sha256(const void *data, size_t len);
hash_t compute_merkle_root(signtx *txs, size_t num_tx);
block_t *new_block(signtx *txs, size_t num_tx);
void free_block(block_t *block);

#endif
