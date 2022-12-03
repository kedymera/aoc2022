#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *infile = fopen("input.txt", "r");
    if (!infile) return 1;
    char buff[5];
    char elfchoice = 0, mychoice = 0;
    int score = 0;
    while (fgets(buff, 5, infile)) {
        elfchoice = buff[0];
        mychoice = buff[2];
        printf("adding choice score %c == %d\n", mychoice, mychoice - 'X' + 1);
        score += mychoice - 'X' + 1;
    }
    fclose(infile);

    printf("score == %d\n", score);
}
