#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 128
#ifdef SMOL
#define MAXCOORD 7
#else
#define MAXCOORD 22
#endif

int min(int a, int b) {
    return a<b?a:b;
}
int max(int a, int b) {
    return a>b?a:b;
}

int main() {
    int cubefield[MAXCOORD][MAXCOORD][MAXCOORD] = {0};
    char buff[BUFFSZ];
#ifdef SMOL
    FILE *file = fopen("inputsmol.txt", "r");
#else
    FILE *file = fopen("input.txt", "r");
#endif
    if (!file) return 1;

    while (fgets(buff, BUFFSZ, file)) {
        char *p = buff;

        int x = strtol(p, &p, 10);
        int y = strtol(++p, &p, 10);
        int z = strtol(++p, &p, 10);
        assert(cubefield[x][y][z] == 0);
        cubefield[x][y][z] = 1;

        printf("%s", buff);
    }

    int area = 0;
    for (int x = 0; x < MAXCOORD; ++x) {
        for (int y = 0; y < MAXCOORD; ++y) {
            for (int z = 0; z < MAXCOORD; ++z) {
                if (!cubefield[x][y][z]) continue;
                area += (x == MAXCOORD-1 || x < MAXCOORD-1 && !cubefield[x+1][y][z])
                      + (x == 0          || x > 0          && !cubefield[x-1][y][z])
                      + (y == MAXCOORD-1 || y < MAXCOORD-1 && !cubefield[x][y+1][z])
                      + (y == 0          || y > 0          && !cubefield[x][y-1][z])
                      + (z == MAXCOORD-1 || z < MAXCOORD-1 && !cubefield[x][y][z+1])
                      + (z == 0          || z > 0          && !cubefield[x][y][z-1]);
                printf("area: %d\n", area);
            }
        }
    }

    printf("area: %d\n", area);
}
