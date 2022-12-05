#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 1024
#define NUMSTACKS 9
#define STACKCAP 128

int getstacktop(char stack[STACKCAP]) {
    int i;
    for (i = 0; stack[i] != -1; ++i);
    return i - 1;
}

char pop(char stack[STACKCAP]) {
    int top = getstacktop(stack);
    char p = stack[top];
    stack[top] = -1;
    return p;
}

void push(char stack[STACKCAP], char val) {
    stack[getstacktop(stack) + 1] = val;
}

void printstacks(char stacks[NUMSTACKS][STACKCAP]) {
    for (int whichstack = 0; whichstack < NUMSTACKS; ++whichstack) {
        printf("%d: ", whichstack+1);
        for (int i = 0; i < STACKCAP; ++i)
            printf("%c", stacks[whichstack][i]);
        printf("\n");
    }
}

int conv_next_int_until_char(char *str, char ch, int *i) {
    int start = *i;
    for (; str[*i]; ++*i)
        if (str[*i] == ch)
            break;
    str[*i] = 0;
    ++*i;
    return atoi(str+start);
}

int main() {
    char buff[BUFFSZ];
    char stacks[NUMSTACKS][STACKCAP];
    memset(stacks, -1, NUMSTACKS*STACKCAP*sizeof(char));
    int whichstack = 0;

    // read stacks from file
    FILE *file = fopen("inputstacks.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file))
        memcpy(stacks[whichstack++], buff, strlen(buff) - 1);
    fclose(file);

    // read moves from file
    file = fopen("inputmoves.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        int l = 0;
        int amt = conv_next_int_until_char(buff, ' ', &l);
        int from = conv_next_int_until_char(buff, ' ', &l) - 1;
        int till = conv_next_int_until_char(buff, '\n', &l) - 1;

        int fromtop = getstacktop(stacks[from]);
        int tilltop = getstacktop(stacks[till]);
        memcpy(stacks[till] + tilltop + 1, stacks[from] + fromtop - amt + 1, amt);
        memset(stacks[from] + fromtop - amt + 1, -1, amt);
    }

    for (whichstack = 0; whichstack < NUMSTACKS; ++whichstack) {
        printf("%c", stacks[whichstack][getstacktop(stacks[whichstack])]);
    }
    printf("\n");
}
