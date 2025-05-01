#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include "btc.h"

int count_blocks() {
    int count = 0;
    DIR *dir = opendir("blocks");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
        if (strstr(entry->d_name, "block") != NULL) count++;
    closedir(dir);
    return count;
}

hash_t get_prev_hash(int block_num) {
    hash_t h = {{0}};
    if (block_num == 0) return h;

    char fname[64];
    sprintf(fname, "blocks/block%03d.dat", block_num - 1);
    FILE *f = fopen(fname, "rb");
    if (f) {
        fread(&h, sizeof(hash_t), 1, f);
        fclose(f);
    }
    return h;
}

int main() {
    mkdir("blocks", 0755);

    signtx txs[2] = {
        { .data = "Alice pays Bob", .sig = sha256("Alice", 5) },
        { .data = "Bob pays Charlie", .sig = sha256("Bob", 3) }
    };

    int block_num = count_blocks();
    hash_t prev_hash = get_prev_hash(block_num);

    block_t *blk = create_block(txs, 2, prev_hash);

    char fname[64];
    sprintf(fname, "blocks/block%03d.dat", block_num);
    FILE *f = fopen(fname, "wb");
    if (f) {
        fwrite(&blk->block_hash, sizeof(hash_t), 1, f);
        fwrite(&blk->prev_hash, sizeof(hash_t), 1, f);
        fwrite(&blk->merkle_root, sizeof(hash_t), 1, f);
        fwrite(&blk->nonce, sizeof(unsigned long), 1, f);
        fclose(f);
        printf("Block %03d saved with hash starting %02x\\n", block_num, blk->block_hash.hash[0]);
    } else {
        perror("Failed to write block");
    }

    free_block(blk);
    return 0;
}