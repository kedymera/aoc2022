#include <stdio.h>

#define BUFFSZ 8*1024

#define DISTINCTCHARS 14

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    fgets(buff, BUFFSZ, file);
    int i;
    for (i = DISTINCTCHARS-1; buff[i] != '\n'; ++i) {
        // wish there was more incentive to optimise this puke... in part 2?
        // well, i generalised it to any number of DISTINCTCHARS... still fast-enough puke though
        // even removing the short-circuiting didn't make it slower
        int is_coll = 0;
        for (int j = 1; j < DISTINCTCHARS; ++j)
            for (int k = 0; k < j; ++k)
                if (buff[i-j] == buff[i-k])
                    is_coll = 1;
        if (!is_coll) break;
    }

    printf("index i=%d, or 1-based=%d\n", i, i+1);
}
