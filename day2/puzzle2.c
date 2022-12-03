#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *infile = fopen("input.txt", "r");
    if (!infile) return 1;
    char buff[5];
    char elfchoice = 0, mychoice = 0;
    int score = 0;
    while (fgets(buff, 5, infile)) {
        // rock == 0, paper == 1, scissors == 2
        elfchoice = buff[0] - 'A';
        // winning choice is always one more than the prev
        // buff[2] - 'Y' is -1 if X, 0 if Y, +1 if Z
        // +3 because some idiot decided -1%3==-1 not ==2
        mychoice = (elfchoice + buff[2] - 'Y' + 3) % 3;
        score += mychoice + 1;
        // outcomes
        //     draw
        if (mychoice == elfchoice) score += 3;
        //     i win
        else if (mychoice == (elfchoice+1) % 3) score += 6;
        //     i lose -> do nothing
        printf("score increased to %d by line %s", score, buff);
    }
    fclose(infile);

    printf("score == %d\n", score);
}
