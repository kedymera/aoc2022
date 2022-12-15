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
    int hx=GRIDX, tx=GRIDX, hy=GRIDY, ty=GRIDY;
    bool visited[GRIDW*GRIDH];
    memset(visited, 0, GRIDW*GRIDH*sizeof(bool));
    while (fgets(buff, BUFFSZ, file)) {
        dir = buff[0];
        for (steps = atoi(buff+2); steps > 0; --steps) {
            switch (dir) {
                case 'U':
                    ++hy;
                    break;
                case 'D':
                    --hy;
                    break;
                case 'R':
                    ++hx;
                    break;
                case 'L':
                    --hx;
                    break;
                default:
                    printf("WTFWTFWTFWTF");
                    exit(1);
            }
            // update tail position
            int dx = hx - tx, dy = hy - ty;
            // if dx is 2, then -1<=dy<=1, so we can just set ty=hy (and v.v.)
            if (dx > 1) {
                ++tx;
                ty=hy;
            }
            if (dx < -1) {
                --tx;
                ty=hy;
            }
            if (dy > 1) {
                ++ty;
                tx=hx;
            }
            if (dy < -1) {
                --ty;
                tx=hx;
            }
            // (if dx and dy are both within [-1,1], h and t are touching and so t doesn't move)
            
            visited[ty*GRIDW + tx] = true;
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
