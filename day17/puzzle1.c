#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef SMOL
#define BUFFSZ 128
#else
#define BUFFSZ 10100
#endif

#define WIDTH 7
char rocktypes[5][4][4] = { {
        {'#', '.', '.', '.'},
        {'#', '.', '.', '.'},
        {'#', '.', '.', '.'},
        {'#', '.', '.', '.'}
    }, {
        {'.', '#', '.', '.'},
        {'#', '#', '#', '.'},
        {'.', '#', '.', '.'},
        {'.', '.', '.', '.'},
    }, {
        {'#', '.', '.', '.'},
        {'#', '.', '.', '.'},
        {'#', '#', '#', '.'},
        {'.', '.', '.', '.'},
    }, {
        {'#', '#', '#', '#'},
        {'.', '.', '.', '.'},
        {'.', '.', '.', '.'},
        {'.', '.', '.', '.'},
    }, {
        {'#', '#', '.', '.'},
        {'#', '#', '.', '.'},
        {'.', '.', '.', '.'},
        {'.', '.', '.', '.'},
    }
};

int max(int a, int b) {
    return a > b ? a : b;
}

int c(int x, int y) {
    assert(x >= 0 && x < WIDTH);
    assert(y >= 0);
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

bool CanMoveLeft(char *chamber, int rocktype, int rockx, int rocky) {
    if (rockx == 0) return false;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (rocktypes[rocktype][i][j] == '#' && chamber[c(rockx+i-1, rocky+j)] == '#')
                return false;
    return true;
}

bool CanMoveRight(char *chamber, int rocktype, int rockx, int rocky) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (rocktypes[rocktype][i][j] == '#'
                && (rockx + i >= WIDTH - 1 // that part of the rock would move into wall
                    || chamber[c(rockx+i+1, rocky+j)] == '#')) // would move into a settled rock
                return false;
    return true;
}

bool CanMoveDown(char *chamber, int rocktype, int rockx, int rocky) {
    if (rocky == 0) return false;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (rocktypes[rocktype][i][j] == '#' && chamber[c(rockx+i, rocky+j-1)] == '#')
                return false;
    return true;
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
    int rocktype = 0;
    int rockx = 2, rocky = highest + 4;
    while (rocks < 2022) {
        if (buff[i] == '<' && CanMoveLeft(chamber, rocktype, rockx, rocky))
            --rockx;
        if (buff[i] == '>' && CanMoveRight(chamber, rocktype, rockx, rocky))
            ++rockx;

        if (CanMoveDown(chamber, rocktype, rockx, rocky)) {
            --rocky;
        } else {
            // settle rock
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (rocktypes[rocktype][i][j] == '#') {
                        chamber[c(rockx+i, rocky+j)] = '#';
                        highest = max(highest, rocky+j);
                    }
            ++rocks;
            // 'spawn' new rock
            rocktype = rocks % 5;
            rockx = 2;
            rocky = highest + 4;
            if (highest > height - 10) {
                chamber = realloc(chamber, 2*height*WIDTH * sizeof(char));
                memset(chamber+height*WIDTH, '.', height*WIDTH);
                height *= 2;
            }
        }
        ++i;
        i %= jetlen;
    }

    PrintChamber(chamber, height);
    printf("tower is of height %d\n", highest + 1);

    free(chamber);
}
