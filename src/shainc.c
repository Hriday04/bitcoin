#include <string.h>
#include "shainc.h"

/* SHA-256 Constants and Initial Values (same as yours) */
static const unsigned long K[64] = { /* ... */ };
static const unsigned long H0[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

/* All your macros */
#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SIGMA0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define sigma0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
#define sigma1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

static void process_chunk(unsigned long *hash, const unsigned char *chunk) {
    unsigned long W[64];
    unsigned long a, b, c, d, e, f, g, h;
    int i;

    for (i = 0; i < 16; i++) {
        W[i] = (chunk[i * 4] << 24) | (chunk[i * 4 + 1] << 16) |
               (chunk[i * 4 + 2] << 8) | chunk[i * 4 + 3];
    }
    for (i = 16; i < 64; i++) {
        W[i] = sigma1(W[i - 2]) + W[i - 7] + sigma0(W[i - 15]) + W[i - 16];
    }

    a = hash[0]; b = hash[1]; c = hash[2]; d = hash[3];
    e = hash[4]; f = hash[5]; g = hash[6]; h = hash[7];

    for (i = 0; i < 64; i++) {
        unsigned long temp1 = h + SIGMA1(e) + CH(e, f, g) + K[i] + W[i];
        unsigned long temp2 = SIGMA0(a) + MAJ(a, b, c);
        h = g; g = f; f = e;
        e = d + temp1;
        d = c; c = b; b = a;
        a = temp1 + temp2;
    }

    hash[0] += a; hash[1] += b; hash[2] += c; hash[3] += d;
    hash[4] += e; hash[5] += f; hash[6] += g; hash[7] += h;
}

hash_t sha256(const void *data, size_t len) {
    unsigned long H[8];
    unsigned char block[64];
    size_t i, blocks;
    hash_t out;

    memcpy(H, H0, sizeof(H));

    blocks = len / 64;
    for (i = 0; i < blocks; i++) {
        process_chunk(H, (const unsigned char *)data + (i * 64));
    }

    memset(block, 0, sizeof(block));
    memcpy(block, (const unsigned char *)data + (blocks * 64), len % 64);
    block[len % 64] = 0x80;

    if (len % 64 > 55) {
        process_chunk(H, block);
        memset(block, 0, sizeof(block));
    }

    block[63] = len * 8;
    process_chunk(H, block);

    for (i = 0; i < 8; i++) {
        out.hash[i*4 + 0] = (H[i] >> 24) & 0xff;
        out.hash[i*4 + 1] = (H[i] >> 16) & 0xff;
        out.hash[i*4 + 2] = (H[i] >> 8) & 0xff;
        out.hash[i*4 + 3] = (H[i]) & 0xff;
    }

    return out;
}
