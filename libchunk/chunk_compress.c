#include "chunk.h"
#include <string.h>
#define MAX_VALUE 4095
#define MEDIUM_VALUE 32
#define FOUR_BITS 4
#define FIVE_BITS 5
#define SIX_BITS 6
#define TWELVE_BITS 12
#define STONE_VALUE 192
#define WOOD_VALUE 128
#define GRASS_VALUE 64
#define EIGHT_BITS 8

unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length) {
    int n = 0;
    unsigned char* chunk_compressed = malloc((width * height * depth) * sizeof(unsigned char));
    for (int i = 0; i < width * height * depth; i++) {
        chunk_compressed[i] = (unsigned char) 0;
    }
    struct compress {
        int nr;
        char val;
    };
    struct compress *runs = (struct compress *)malloc((width * height * depth) * sizeof(struct compress));
    for (int i = 0; i < width * height * depth; i++) {
        runs[i].nr = 0;
        runs[i].val = (char) 0;
    }
    runs[0].nr = 0;
    runs[0].val = (char) LIMIT;
    for (int y = 0; y < height; y++) {
        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                if (chunk[x][y][z] == runs[n].val) {
                    if (runs[n].nr == MAX_VALUE) {
                        n++;
                        runs[n].nr = 1;
                        runs[n].val = chunk[x][y][z];
                    } else {
                        runs[n].nr++;
                    }
                } else {
                    n++;
                    runs[n].nr = 1;
                    runs[n].val = chunk[x][y][z];
                }
            }
        }
    }
    (*length) = 0;
    for (int i = 1; i <= n; i++) {
        chunk_compressed[*length] = (unsigned char) 0;
        switch (runs[i].val) {
            case BLOCK_GRASS:
                chunk_compressed[*length] += (unsigned char) GRASS_VALUE;
                break;
            case BLOCK_WOOD:
                chunk_compressed[*length] += (unsigned char) WOOD_VALUE;
                break;
            case BLOCK_STONE:
                chunk_compressed[*length] += (unsigned char) STONE_VALUE;
                break;
            default: break;
        }
        if (runs[i].nr < MEDIUM_VALUE) {
            chunk_compressed[*length] += (unsigned char)(runs[i].nr);
            (*length)++;
        } else {
            chunk_compressed[*length] += (unsigned char) MEDIUM_VALUE;
            chunk_compressed[*length] += (unsigned char)(runs[i].nr / (1 << EIGHT_BITS));
            (*length)++;
            chunk_compressed[*length] += (unsigned char)(runs[i].nr % (1 << EIGHT_BITS));
            (*length)++;
        }
    }
    free(runs);
    return chunk_compressed;
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {
    struct compress {
    int nr;
    char val;
    };
    struct compress *runs = (struct compress *)malloc((width * height * depth + 1) * sizeof(struct compress));
    char*** chunk = malloc(width * sizeof(char**));
    for (int x = 0; x < width; x++) {
        chunk[x] = malloc (height * sizeof(char*));
        for (int y = 0; y < height; y++) {
            chunk[x][y] = malloc (depth * sizeof(char));
        }
    }
    // parcurg vectorul de unsigned char :((
    int m = 0, k = 0, n = 0, nr = 0, i = 0;
    while (nr < width * height * depth) {
        runs[m].val = (char) (code[i] >> SIX_BITS);
        if ((code[i] >> FIVE_BITS) % 2) {
            runs[m].nr = (int) (code[i] % (1 << FOUR_BITS));
            runs[m].nr = runs[m].nr << EIGHT_BITS;
            runs[m].nr += (int) (code[i + 1]);
            i++;
        } else {
            runs[m].nr = (int) (code[i] % (1 << FIVE_BITS));
        }
        nr = nr + runs[m].nr;
        m++;
        i++;
    }
    for (int y = 0; y < height; y++) {
        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                if (runs[k].nr > 0) {
                    chunk[x][y][z] = runs[k].val;
                    runs[k].nr--;
                } else {
                    k++;
                    chunk[x][y][z] = runs[k].val;
                    runs[k].nr--;
                }
            }
        }
    }
    free(runs);
    return chunk;
}

