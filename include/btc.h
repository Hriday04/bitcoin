#ifndef BTC_H
#define BTC_H

#include <stddef.h>
#include <stdint.h>
#include "shainc.h"

typedef struct {
    char data[256];
    hash_t sig;
} signtx;

typedef struct {
    size_t num_tx;
    signtx *txs;
    hash_t merkle_root;
    hash_t prev_hash;
    hash_t block_hash;
    unsigned long nonce;
} block_t;

block_t *create_block(signtx *txs, size_t num_tx, hash_t prev_hash);
void free_block(block_t *block);

#endif
