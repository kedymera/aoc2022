#include <stdio.h>

#define BUFFSZ 8*1024

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    fgets(buff, BUFFSZ, file);
    int i;
    for (i = 3; buff[i] != '\n'; ++i) {
        // wish there was more incentive to optimise this puke... in part 2?
        if (buff[i-3] != buff[i-2]
         && buff[i-3] != buff[i-1]
         && buff[i-3] != buff[i-0]
         && buff[i-2] != buff[i-1]
         && buff[i-2] != buff[i-0]
         && buff[i-1] != buff[i-0])
            break;
    }

    printf("index i=%d, or 1-based=%d\n", i, i+1);
}
