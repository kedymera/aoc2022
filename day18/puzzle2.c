#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 128
#ifdef SMOL
#define MAXCOORD 9
#else
#define MAXCOORD 24
#endif

int area = 0;
int cubefield[MAXCOORD][MAXCOORD][MAXCOORD] = {0};

void FloodFill(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x > MAXCOORD-1 || y > MAXCOORD-1 || z > MAXCOORD-1) return;
    if (cubefield[x][y][z] == 2) return; // already scanned

    cubefield[x][y][z] = 2;

    if (x > 0 && cubefield[x-1][y][z] == 1) ++area;
    else FloodFill(x-1, y, z);
    if (x < MAXCOORD-1 && cubefield[x+1][y][z] == 1) ++area;
    else FloodFill(x+1, y, z);
    if (y > 0 && cubefield[x][y-1][z] == 1) ++area;
    else FloodFill(x, y-1, z);
    if (y < MAXCOORD-1 && cubefield[x][y+1][z] == 1) ++area;
    else FloodFill(x, y+1, z);
    if (z > 0 && cubefield[x][y][z-1] == 1) ++area;
    else FloodFill(x, y, z-1);
    if (z < MAXCOORD-1 && cubefield[x][y][z+1] == 1) ++area;
    else FloodFill(x, y, z+1);
}

int main() {
    char buff[BUFFSZ];
#ifdef SMOL
    FILE *file = fopen("inputsmol.txt", "r");
#else
    FILE *file = fopen("input.txt", "r");
#endif
    if (!file) return 1;

    while (fgets(buff, BUFFSZ, file)) {
        char *p = buff;

        int x = strtol(p, &p, 10) + 1; // shift up, so guarantee spots with coord 0 are empty
        int y = strtol(++p, &p, 10) + 1;
        int z = strtol(++p, &p, 10) + 1;
        assert(cubefield[x][y][z] == 0);
        cubefield[x][y][z] = 1;

        printf("%s", buff);
    }

    FloodFill(0, 0, 0);

    for (int x = 0; x < MAXCOORD; ++x) {
        for (int y = 0; y < MAXCOORD; ++y) {
            for (int z = 0; z < MAXCOORD; ++z) {
                switch(cubefield[x][y][z]) {
                    case 0:
                        printf(".");
                        break;
                    case 1:
                        printf("#");
                        break;
                    case 2:
                        printf("O");
                        break;
                    default:
                        assert(0);
                        break;
                }
            }
            printf("\n");
        }
        printf("\n\n");
    }

    printf("external surface area: %d\n", area);
}
