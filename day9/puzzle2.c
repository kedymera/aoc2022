#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFSZ 128
#define GRIDX 68
#define GRIDY 106
#define GRIDW 377
#define GRIDH 215

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    char dir;
    int steps;

    // dry run used to find min/max
    int rx[10], ry[10];
    for (int i = 0; i < 10; ++i) rx[i] = GRIDX;
    for (int i = 0; i < 10; ++i) ry[i] = GRIDY;
    bool visited[GRIDW*GRIDH];
    memset(visited, 0, GRIDW*GRIDH*sizeof(bool));
    while (fgets(buff, BUFFSZ, file)) {
        dir = buff[0];
        for (steps = atoi(buff+2); steps > 0; --steps) {
            switch (dir) {
                case 'U':
                    ++ry[0];
                    break;
                case 'D':
                    --ry[0];
                    break;
                case 'R':
                    ++rx[0];
                    break;
                case 'L':
                    --rx[0];
                    break;
                default:
                    printf("WTFWTFWTFWTF");
                    exit(1);
            }
            // update tail positions
            for (int i = 1; i < 10; ++i) {
                int dx = rx[i-1] - rx[i], dy = ry[i-1] - ry[i];
                if (dx > 1) {
                    ++rx[i];
                    if (dy >= 1) ++ry[i];
                    else if (dy <= -1) --ry[i];
                } else if (dx < -1) {
                    --rx[i];
                    if (dy >= 1) ++ry[i];
                    else if (dy <= -1) --ry[i];
                } else if (dy > 1) {
                    ++ry[i];
                    if (dx >= 1) ++rx[i];
                    else if (dx <= -1) --rx[i];
                } else if (dy < -1) {
                    --ry[i];
                    if (dx >= 1) ++rx[i];
                    else if (dx <= -1) --rx[i];
                }
                // (if dx and dy are both within [-1,1], h and t are touching and so t doesn't move)
            }
            visited[ry[9]*GRIDW + rx[9]] = true;
        }
    }

    int totalvisited = 0;
    for (int i = 0; i < GRIDW; ++i) {
        for (int j = 0; j < GRIDH; ++j) {
            if (visited[j*GRIDW + i]) {
                printf("#");
                ++totalvisited;
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("visited %d in total\n", totalvisited);
}
