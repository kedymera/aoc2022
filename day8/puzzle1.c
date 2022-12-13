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
    
    // strat: loop through all internal gridpoints
    //          go in each of the 4 directions from there, and determine visibility
    // this sounds n^3 in GRIDSZ, but really, the extra n loop will usually exit very early
    int visible_count = (GRIDSZ-1)*4; // edge trees all visible
    int k;
    for (i = 1; i < GRIDSZ-1; ++i) {
        for (j = 1; j < GRIDSZ-1; ++j) {
            // up
            for (k = i-1; k >= 0; --k)
                if (grid[i][j] <= grid[k][j])
                    goto blockedup;
            goto visible;
blockedup:;
            // down
            for (k = i+1; k < GRIDSZ; ++k)
                if (grid[i][j] <= grid[k][j])
                    goto blockeddown;
            goto visible;
blockeddown:;
            // left
            for (k = j-1; k >= 0; --k)
                if (grid[i][j] <= grid[i][k])
                    goto blockedleft;
            goto visible;
blockedleft:;
            // right
            for (k = j+1; k < GRIDSZ; ++k)
                if (grid[i][j] <= grid[i][k])
                    goto blockedright;
            goto visible;
blockedright:;
            continue;

visible:;
            ++visible_count;
        }
    }
    
    printf("visible: %d (%d internal)\n", visible_count, visible_count - 4*(GRIDSZ-1));
}
