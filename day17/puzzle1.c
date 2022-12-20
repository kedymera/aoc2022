#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SMOL
#define BUFFSZ 128
#else
#define BUFFSZ 10100
#endif

#define WIDTH 7

int max(int a, int b) {
    return a > b ? a : b;
}

int c(int x, int y) {
    return y*WIDTH + x;
}

void PrintChamber(char *chamber, int height) {
    for (int j = height - 1; j >= 0; --j) {
        printf("|");
        for (int i = 0; i < WIDTH; ++i) {
            printf("%c", chamber[c(i, j)]);
        }
        printf("|\n");
    }
    printf("+");
    for (int i = 0; i < WIDTH; ++i) printf("-");
    printf("+\n");
}

int main() {
    char buff[BUFFSZ];
#ifdef SMOL
    FILE *file = fopen("inputsmol.txt", "r");
#else
    FILE *file = fopen("input.txt", "r");
#endif
    if (!file) return 1;
    fgets(buff, BUFFSZ, file);
    int jetlen;
    for (jetlen = 0; buff[jetlen] != '\n'; ++jetlen);

    int height = 10;
    char *chamber = malloc(height*WIDTH * sizeof(char));
    memset(chamber, '.', height*WIDTH);
    PrintChamber(chamber, height);

    int highest = -1; // y coord of highest landed rock (or -1 for floor)
    int i = 0; // where we are in the jet sequence
    int rocks = 0; // how many rocks have settled
    int rockx = 2, rocky = highest + 4, rockh = 2;
    while (rocks < 2022) {
        // rock move down
        if (rocky == 0 || chamber[c(rockx, rocky - 1)] == '#' || chamber[c(rockx + 1, rocky - 1)] == '#') {
            // settle rock
            chamber[c(rockx, rocky)] = '#';
            chamber[c(rockx+1, rocky)] = '#';
            chamber[c(rockx, rocky+1)] = '#';
            chamber[c(rockx+1, rocky+1)] = '#';
            ++rocks;
            rockx = 2;
            highest = max(highest, rocky + rockh - 1);
            if (highest > height - 10) {
                int newheight = height * 2;
                chamber = realloc(chamber, newheight*WIDTH * sizeof(char));
                for (; height < newheight; ++height) {
                    for (int i = 0; i < WIDTH; ++i) {
                        chamber[c(i, height)] = '.';
                    }
                }
            }
            rocky = highest + 4;
        } else {
            --rocky;
        }
        ++i;
        i %= jetlen;
    }

    PrintChamber(chamber, height);
    printf("tower is of height %d\n", highest + 1);

    free(chamber);
    printf("hello world\n");
}
