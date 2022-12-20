#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SMOL
#define BUFFSZ 128
#else
#define BUFFSZ 10100
#endif

#define WIDTH 7

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
    printf("%s", buff);

    char *chamber = malloc(10*WIDTH * sizeof(char));
    memset(chamber, '.', 10*WIDTH);
    for (int i = 0; i < 10; ++i) {
        chamber[i] = '0' + i;
    }
    PrintChamber(chamber, 10);

    free(chamber);
    printf("hello world\n");
}
