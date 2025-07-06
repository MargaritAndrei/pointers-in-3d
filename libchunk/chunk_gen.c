#include "chunk.h"

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth) {
        return chunk;
    }
    chunk[x][y][z] = block;
    return chunk;
}

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {
    int aux = 0;
    if (x0 > x1) {
        aux = x0;
        x0 = x1;
        x1 = aux;
    }
    if (y0 > y1) {
        aux = y0;
        y0 = y1;
        y1 = aux;
    }
    if (z0 > z1) {
        aux = z0;
        z0 = z1;
        z1 = aux;
    }
    for (int x = x0; x <= x1; x++) {
        for (int y = y0; y <= y1; y++) {
            for (int z = z0; z <= z1; z++) {
                chunk_place_block(chunk, width, height, depth, x, y, z, block);
            }
        }
    }
    return chunk;
}

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {
    int int_radius = (int)radius;
    for (int i = x - int_radius; i <= x + int_radius; i++) {
        for (int j = y - int_radius; j <= y + int_radius; j++) {
            for (int k = z - int_radius; k <= z + int_radius; k++) {
                if ((i - x) * (i - x) + (j - y) * (j - y) + (k - z) * (k - z) <= radius * radius) {
                    chunk_place_block(chunk, width, height, depth, i, j, k, block);
                }
            }
        }
    }
    return chunk;
}
