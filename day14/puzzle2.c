#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 1024
#define XMIN 300
#define XMAX 700
#define YMIN 0
#define YMAX 163
#define CAVEW (XMAX-XMIN)
#define CAVEH (YMAX-YMIN)
#define CAVESZ (CAVEW*CAVEH)

int c(int x, int y) {
    return (y-YMIN)*CAVEW + (x-XMIN);
}

void printcave(char cave[CAVESZ]) {
    for (int j = YMIN; j < YMAX; ++j) {
        for (int i = XMIN; i < XMAX; ++i) {
            printf("%c", cave[c(i, j)]);
        }
        printf("\n");
    }
}

void ParseRockCurve(char cave[CAVESZ], char buff[BUFFSZ]) {
    long x = -1, y = -1, newx, newy;
    for (char *p = buff; *p; ++p) {
        if (*p >= '0' && *p <= '9') {
            newx = strtol(p, &p, 10);
            // p now on comma after x value
            ++p;
            newy = strtol(p, &p, 10);

            if (x == -1) {
                x = newx;
                y = newy;
            } else {
                if (newx == x) // vertical line
                    if (newy > y)
                        for (; y < newy; ++y)
                            cave[c(x, y)] = '#';
                    else
                        for (; y > newy; --y)
                            cave[c(x, y)] = '#';
                else // horizontal line
                    if (newx > x)
                        for (; x < newx; ++x)
                            cave[c(x, y)] = '#';
                    else
                        for (; x > newx; --x)
                            cave[c(x, y)] = '#';
            }
        }
    }
    cave[c(x, y)] = '#';
}

bool DropSand(char cave[CAVESZ]) {
    int x = 500, y = 0;
    while (y < YMAX-1) {
        if (cave[c(x, y+1)] == '.') {
            ++y;
        } else if (cave[c(x-1, y+1)] == '.') {
            --x;
            ++y;
        } else if (cave[c(x+1, y+1)] == '.') {
            ++x;
            ++y;
        } else {
            cave[c(x, y)] = 'o';
            return !(x==500 && y==0);
        }
    }
    cave[c(x, y)] = 'o';
    return true;
}

int main() {
    char cave[CAVESZ];
    memset(cave, '.', CAVESZ);
    cave[c(500, 0)] = '+';

    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        ParseRockCurve(cave, buff);
    }

    int i = 0;
    while (DropSand(cave)) {
        printf("dropped sand #%d\n", ++i);
        // prints far too much! (~25000*160 lines of 400 chars)
        //printcave(cave);
    }
    printf("dropped last sand, #%d, blocking the source\n", ++i);
    printcave(cave);

    printf("dropped %d sands in total\n", i);
}
