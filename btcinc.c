
#include <stdlib.h>
#include <string.h>
#include "btcinc.h"
#include "shainc.h"

static hash_t hash_combine(hash_t a, hash_t b) {
    uint8_t buffer[64];
    memcpy(buffer, a.hash, 32);
    memcpy(buffer + 32, b.hash, 32);
    return sha256(buffer, 64);
}

hash_t compute_merkle_root(signtx *txs, size_t num_tx) {
    size_t leaf_count = num_tx;
    size_t total = 2 * num_tx;
    hash_t *tree = malloc(total * sizeof(hash_t));
    for (size_t i = 0; i < num_tx; ++i)
        tree[num_tx + i] = txs[i].sig;

    for (size_t i = num_tx - 1; i > 0; --i)
        tree[i] = hash_combine(tree[2 * i], tree[2 * i + 1]);

    hash_t root = tree[1];
    free(tree);
    return root;
}

block_t *new_block(signtx *txs, size_t num_tx) {
    block_t *block = malloc(sizeof(block_t));
    block->num_tx = num_tx;
    block->txtree = malloc(num_tx * sizeof(signtx));
    memcpy(block->txtree, txs, num_tx * sizeof(signtx));
    block->merkle = malloc(sizeof(hash_t));
    *block->merkle = compute_merkle_root(block->txtree, num_tx);
    return block;
}

void free_block(block_t *block) {
    if (block) {
        free(block->merkle);
        free(block->txtree);
        free(block);
    }
}
