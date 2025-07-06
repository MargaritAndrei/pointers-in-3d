#include "chunk.h"

char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth) {
    char ***new_chunk = malloc(depth * sizeof (char **));
    for (int x = 0; x < depth; x++) {
        new_chunk[x] = malloc(height * sizeof (char *));
        for (int y = 0; y < height; y++) {
            new_chunk[x][y] = malloc(width * sizeof (char));
        }
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                new_chunk[depth - z -1][y][x] = chunk[x][y][z];
            }
        }
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(chunk[x][y]);
        }
        free(chunk[x]);
    }
    free(chunk);
    return new_chunk;
}

char*** chunk_fill_connected(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
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
                            if (nrdif0 < 2 && chunk[x + x1][y + y1][z + z1] != BLOCK_AIR &&
                            chunk[x + x1][y + y1][z + z1] != LIMIT) {
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

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {
    int n = 0, val = 0, found = 0, ok = 0;
    struct element {
        int x, y, z, val;
    };
    struct element* block = (struct element *) malloc (width * height * depth * sizeof(struct element));
    while (!ok) {
        ok = 1;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    if (chunk[x][y][z] != BLOCK_AIR) {
                        val = chunk[x][y][z];
                        chunk_fill(chunk, width, height, depth, x, y, z, LIMIT + 1);
                        n = 0;
                        for (int x1 = 0; x1 < width; x1++) {
                            for (int y1 = 0; y1 < height; y1++) {
                                for (int z1 = 0; z1 < depth; z1++) {
                                    if (chunk[x1][y1][z1] == LIMIT + 1) {
                                        block[n].x = x1;
                                        block[n].y = y1;
                                        block[n].z = z1;
                                        n++;
                                    }
                                }
                            }
                        }
                        found = 1;
                        for (int i = 0; i < n; i++) {
                            int x1 = block[i].x;
                            int y1 = block[i].y;
                            int z1 = block[i].z;
                            if (y1 == 0 || (chunk[x1][y1 - 1][z1] != BLOCK_AIR &&
                            chunk[x1][y1 - 1][z1] != LIMIT + 1)) {
                                found = 0;
                            }
                        }
                        if (found) {
                            ok = 0;
                            for (int i = 0; i < n; i++) {
                                int x1 = block[i].x;
                                int y1 = block[i].y;
                                int z1 = block[i].z;
                                chunk[x1][y1][z1] = BLOCK_AIR;
                                block[i].y = y1 - 1;
                            }
                        }
                        for (int i = 0; i < n; i++) {
                            int x1 = block[i].x;
                            int y1 = block[i].y;
                            int z1 = block[i].z;
                            chunk[x1][y1][z1] = (char) val;
                        }
                    }
                }
            }
        }
    }
    char ***chunk2 = malloc(width * sizeof (char **));
    for (int x = 0; x < width; x++) {
        chunk2[x] = malloc(height * sizeof(char *));
        for (int y = 0; y < height; y++) {
            chunk2[x][y] = malloc(depth * sizeof(char));
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                chunk2[x][y][z] = chunk[x][y][z];
            }
        }
    }
    ok = 0;
    while (!ok) {
        ok = 1;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    if (chunk[x][y][z] != BLOCK_AIR) {
                        chunk_fill_connected(chunk, width, height, depth, x, y, z, LIMIT + 1);
                        n = 0;
                        for (int x1 = 0; x1 < width; x1++) {
                            for (int y1 = 0; y1 < height; y1++) {
                                for (int z1 = 0; z1 < depth; z1++) {
                                    if (chunk[x1][y1][z1] == LIMIT + 1) {
                                        block[n].x = x1;
                                        block[n].y = y1;
                                        block[n].z = z1;
                                        block[n].val = chunk2[x1][y1][z1];
                                        n++;
                                    }
                                }
                            }
                        }
                        found = 1;
                        for (int i = 0; i < n; i++) {
                            int x1 = block[i].x;
                            int y1 = block[i].y;
                            int z1 = block[i].z;
                            if (y1 == 0 || (chunk[x1][y1 - 1][z1] != BLOCK_AIR &&
                            chunk[x1][y1 - 1][z1] != LIMIT + 1)) {
                                found = 0;
                            }
                        }
                        if (found) {
                            ok = 0;
                            for (int i = 0; i < n; i++) {
                                int x1 = block[i].x;
                                int y1 = block[i].y;
                                int z1 = block[i].z;
                                chunk[x1][y1][z1] = BLOCK_AIR;
                                chunk2[x1][y1][z1] = BLOCK_AIR;
                                block[i].y = y1 - 1;
                            }
                        }
                        for (int i = 0; i < n; i++) {
                            int x1 = block[i].x;
                            int y1 = block[i].y;
                            int z1 = block[i].z;
                            chunk[x1][y1][z1] = (char) block[i].val;
                            chunk2[x1][y1][z1] = (char) block[i].val;
                        }
                    }
                }
            }
        }
    }
    found = 0;
    *new_height = height;
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            for (int z = 0; z < depth; z++) {
                if (chunk[x][y][z] != BLOCK_AIR) {
                    found = 1;
                }
            }
        }
        if (found) {
            *new_height = y + 1;
            break;
        }
    }
    // deci in final observ faptul ca imi cad blocurile dar mai este un test
    // in care blocurile cad dar sunt conectate intre ele si nu pot cadea
    // decat precum un "singur" obiect asa cum le-am luat eu
    // voi face un fill pentru obiectele unite luandu-le precum unul singur
    // apoi voi incerca sa vad daca imi ia acest test
    for (int x = 0; x < width; x++) {
        for (int y = *new_height; y < height; y++) {
            free(chunk[x][y]);
        }
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(chunk2[x][y]);
        }
        free(chunk2[x]);
    }
    free(chunk2);
    free(block);
    return chunk;
}
