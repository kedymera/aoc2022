#include <stdbool.h>
#include <stdio.h>

#define GRIDSZ 99
#define BUFFSZ (GRIDSZ+2) // grid content + \n + \0

int ANSWER = 0;

int main() {
    char buff[BUFFSZ];
    char grid[GRIDSZ][GRIDSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    int i = 0, j;
    while (fgets(buff, BUFFSZ, file)) {
        for (j = 0; j < GRIDSZ; ++j) {
            grid[i][j] = buff[j];
        }
        ++i;
    }

    printf("Hello world\n");
}
