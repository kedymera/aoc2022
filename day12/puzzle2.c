#include <stdio.h>
#include <string.h>
#include <limits.h>

#define BUFFSZ 128
#define GRIDW 67
#define GRIDH 41
#define GRIDSZ GRIDW*GRIDH

int c(int x, int y) {
    return y*GRIDW + x;
}

void printbests(int bests[GRIDSZ], int sx, int sy) {
    for (int j = 0; j < GRIDH; ++j) {
        for (int i = 0; i < GRIDW; ++i) {
            if (i == sx && j == sy) printf("\033[31;1;4m");
            if (bests[c(i,j)] == INT_MAX)
                printf(".");
            else
                printf("\033[38;2;%d;%d;%dm%x", 255-bests[c(i,j)]/2, 255-bests[c(i,j)]/2, 255-bests[c(i,j)]/2, bests[c(i,j)]%16);
            if (i == sx && j == sy) printf("\033[0m");
        }
        printf("\n");
    }
}

int FindPath(int sx, int sy, int ex, int ey, char elevations[GRIDSZ], int bests[GRIDSZ]) {
    int h = c(sx, sy);
    int l = c(sx-1, sy);
    int r = c(sx+1, sy);
    int u = c(sx, sy-1);
    int d = c(sx, sy+1);
    if (sx == ex && sy == ey) return bests[h];
    int bl = INT_MAX;
    int br = INT_MAX;
    int bu = INT_MAX;
    int bd = INT_MAX;
    // go left?
    if (sx > 0 && elevations[l] <= elevations[h] + 1 && bests[l] > bests[h] + 1) {
        bests[l] = bests[h] + 1;
        bl = FindPath(sx-1, sy, ex, ey, elevations, bests);
    }
    // go right?
    if (sx < GRIDW-1 && elevations[r] <= elevations[h] + 1 && bests[r] > bests[h] + 1) {
        bests[r] = bests[h] + 1;
        br = FindPath(sx+1, sy, ex, ey, elevations, bests);
    }
    // go up?
    if (sy > 0 && elevations[u] <= elevations[h] + 1 && bests[u] > bests[h] + 1) {
        bests[u] = bests[h] + 1;
        bu = FindPath(sx, sy-1, ex, ey, elevations, bests);
    }
    // go down?
    if (sy < GRIDH-1 && elevations[d] <= elevations[h] + 1 && bests[d] > bests[h] + 1) {
        bests[d] = bests[h] + 1;
        bd = FindPath(sx, sy+1, ex, ey, elevations, bests);
    }
    
    int best = bl;
    if (br < best) best = br;
    if (bu < best) best = bu;
    if (bd < best) best = bd;
    return best;
}

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    char elevations[GRIDSZ];
    int row = 0;
    while (fgets(buff, BUFFSZ, file)) {
        memcpy(elevations+row*GRIDW, buff, GRIDW*sizeof(char));
        ++row;
        printf("%s", buff);
    }

    int startx, starty, endx, endy;
    for (int j = 0; j < GRIDH; ++j) {
        for (int i = 0; i < GRIDW; ++i) {
            printf("%c", elevations[c(i,j)]);
            if (elevations[c(i,j)] == 'S') {
                startx = i;
                starty = j;
                elevations[c(i,j)] = 'a';
            }
            if (elevations[c(i,j)] == 'E') {
                endx = i;
                endy = j;
                elevations[c(i,j)] = 'z';
            }

        }
        printf("\n");
    }

    int bests[GRIDSZ]; // track the length of the shortest path to each pt

    int shortestlength = INT_MAX;
    for (int j = 0; j < GRIDH; ++j) {
        for (int i = 0; i < GRIDW; ++i) {
            if (elevations[c(i,j)] == 'a') {
                for (int i = 0; i < GRIDSZ; ++i) {
                    bests[i] = INT_MAX;
                }
                bests[c(i,j)] = 0;
                printbests(bests, i, j);
                int length = FindPath(i, j, endx, endy, elevations, bests);
                printf("shortest path from [%d, %d] is of length %d\n", i, j, length); 
                if (length < shortestlength) {
                    printf("beats prev best %d\n", length);
                    shortestlength = length;
                }
            }
        }
    }
    printf("shortest length: %d\n", shortestlength);
}
