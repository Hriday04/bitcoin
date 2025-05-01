#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "btc.h"
#include "shainc.h"

#define DIFFICULTY_BYTE 0x00

static hash_t hash_combine(hash_t a, hash_t b) {
    unsigned char buffer[64];
    memcpy(buffer, a.hash, 32);
    memcpy(buffer + 32, b.hash, 32);
    return sha256(buffer, 64);
}

static hash_t compute_merkle_root(signtx *txs, size_t num_tx) {
    size_t i, next_n;
    size_t n = num_tx;
    hash_t *level = malloc(n * sizeof(hash_t));
    for (i = 0; i < n; i++)
        level[i] = txs[i].sig;

    while (n > 1) {
        next_n = (n + 1) / 2;
        for (i = 0; i < next_n; i++) {
            if (2 * i + 1 < n)
                level[i] = hash_combine(level[2 * i], level[2 * i + 1]);
            else
                level[i] = level[2 * i];
        }
        n = next_n;
    }

    hash_t root = level[0];
    free(level);
    return root;
}

static int meets_difficulty(hash_t h) {
    return h.hash[0] == DIFFICULTY_BYTE;
}

block_t *create_block(signtx *txs, size_t num_tx, hash_t prev_hash) {
    block_t *blk = malloc(sizeof(block_t));
    blk->txs = malloc(num_tx * sizeof(signtx));
    memcpy(blk->txs, txs, num_tx * sizeof(signtx));
    blk->num_tx = num_tx;
    blk->prev_hash = prev_hash;
    blk->merkle_root = compute_merkle_root(txs, num_tx);

    blk->nonce = 0;
    hash_t h;
    unsigned char input[sizeof(hash_t) * 2 + sizeof(unsigned long)];

    do {
        memcpy(input, blk->prev_hash.hash, 32);
        memcpy(input + 32, blk->merkle_root.hash, 32);
        memcpy(input + 64, &blk->nonce, sizeof(unsigned long));
        h = sha256(input, 64 + sizeof(unsigned long));
        blk->nonce++;
    } while (!meets_difficulty(h));

    blk->block_hash = h;
    return blk;
}

void free_block(block_t *blk) {
    free(blk->txs);
    free(blk);
}