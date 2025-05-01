
#include <stdio.h>
#include "btcinc.h"

int main() {
    signtx txs[2] = {
        { .data = "Alice pays Bob", .sig = sha256("Alice", 5) },
        { .data = "Bob pays Charlie", .sig = sha256("Bob", 3) }
    };

    block_t *block = new_block(txs, 2);
    printf("Merkle root begins with: %02x\n", block->merkle->hash[0]);
    free_block(block);
    return 0;
}
