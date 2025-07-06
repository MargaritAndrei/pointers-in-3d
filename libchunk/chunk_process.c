#include "chunk.h"

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == target_block) {
                    chunk[i][j][k] = LIMIT;
                }
            }
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == LIMIT) {
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            for (int z = -1; z <= 1; z++) {
                                int nrdif0 = 0;
                                if (x != 0) {
                                    nrdif0++;
                                }
                                if (y != 0) {
                                    nrdif0++;
                                }
                                if (z != 0) {
                                    nrdif0++;
                                }
                                if (i + x >= 0 && i + x < width && j + y >= 0 &&
                                j + y < height && k + z >= 0 && k + z < depth) {
                                    if (nrdif0 < 3) {
                                        if (chunk[i + x][j + y][k + z] != LIMIT) {
                                            chunk_place_block(chunk, width, height, depth,
                                            i + x, j + y, k + z, shell_block);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == LIMIT) {
                    chunk[i][j][k] = target_block;
                }
            }
        }
    }
    return chunk;
}

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    int val = chunk[x][y][z];
    struct element {
        int x, z;
    }*q = (struct element *) malloc (width * depth * sizeof(struct element));
    int qstart = 0, qend = 0;
    q[qend].x = x;
    q[qend].z = z;
    chunk[x][y][z] = LIMIT;
    while (qstart <= qend) {
        x = q[qstart].x;
        z = q[qstart].z;
        qstart++;
        if (chunk[x][y][z] == LIMIT) {
            for (int x1 = -1; x1 <= 1; x1++) {
                for (int z1 = -1; z1 <= 1; z1++) {
                    int nrdif0 = 0;
                    if (x1 != 0) {
                        nrdif0++;
                    }
                    if (z1 != 0) {
                        nrdif0++;
                    }
                    if (x + x1 >= 0 && x + x1 < width && y >= 0 &&
                    y < height && z + z1 >= 0 && z + z1 < depth) {
                        if (nrdif0 < 2 && chunk[x + x1][y][z + z1] == val) {
                            qend++;
                            q[qend].x = x + x1;
                            q[qend].z = z + z1;
                            chunk[x + x1][y][z + z1] = LIMIT;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < width; i++) {
        for (int k = 0; k < depth; k++) {
            if (chunk[i][y][k] == LIMIT) {
                chunk[i][y][k] = block;
            }
        }
    }
    free(q);
    return chunk;
}

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    int val = chunk[x][y][z];
    struct element {
        int x, y, z;
    };
    struct element *q = (struct element *) malloc (width * height * depth * sizeof(struct element));
    int qstart = 0, qend = 0;
    q[qend].x = x;
    q[qend].y = y;
    q[qend].z = z;
    chunk[x][y][z] = LIMIT;
    while (qstart <= qend) {
        x = q[qstart].x;
        y = q[qstart].y;
        z = q[qstart].z;
        qstart++;
        if (chunk[x][y][z] == LIMIT) {
            for (int x1 = -1; x1 <= 1; x1++) {
                for (int y1 = -1; y1 <= 1; y1++) {
                    for (int z1 = -1; z1 <= 1; z1++) {
                        int nrdif0 = 0;
                        if (x1 != 0) {
                            nrdif0++;
                        }
                        if (y1 != 0) {
                            nrdif0++;
                        }
                        if (z1 != 0) {
                            nrdif0++;
                        }
                        if (x + x1 >= 0 && x + x1 < width && y + y1 >= 0 &&
                        y + y1 < height && z + z1 >= 0 && z + z1 < depth) {
                            if (nrdif0 < 2 && chunk[x + x1][y + y1][z + z1] == val) {
                                qend++;
                                q[qend].x = x + x1;
                                q[qend].y = y + y1;
                                q[qend].z = z + z1;
                                chunk[x + x1][y + y1][z + z1] = LIMIT;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < depth; k++) {
                if (chunk[i][j][k] == LIMIT) {
                    chunk[i][j][k] = block;
                }
            }
        }
    }
    free(q);
    return chunk;
}
