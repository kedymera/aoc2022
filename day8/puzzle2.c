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
    int scenic_score_max = 0;
    int scenic_score_max_i = 0;
    int scenic_score_max_j = 0;
    int k;
    for (i = 1; i < GRIDSZ-1; ++i) {
        for (j = 1; j < GRIDSZ-1; ++j) {
            int scenic_score = 1;
            // up
            for (k = i-1; k >= 0; --k)
                if (grid[i][j] <= grid[k][j]) {
                    --k;
                    break;
                }
            scenic_score *= i-k-1;
            // down
            for (k = i+1; k < GRIDSZ; ++k)
                if (grid[i][j] <= grid[k][j]) {
                    ++k;
                    break;
                }
            scenic_score *= k-i-1;
            // left
            for (k = j-1; k >= 0; --k)
                if (grid[i][j] <= grid[i][k]) {
                    --k;
                    break;
                }
            scenic_score *= j-k-1;
            // right
            for (k = j+1; k < GRIDSZ; ++k)
                if (grid[i][j] <= grid[i][k]) {
                    ++k;
                    break;
                }
            scenic_score *= k-j-1;
            if (scenic_score > scenic_score_max) {
                scenic_score_max = scenic_score;
                scenic_score_max_i = i;
                scenic_score_max_j = j;
            }
        }
    }
    
    printf("max scenic score is %d at grid[%d][%d]\n", scenic_score_max, scenic_score_max_i, scenic_score_max_j);
}
