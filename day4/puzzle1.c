#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 1024

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
    FILE *infile = fopen("input.txt", "r");
    if (!infile) return 1;
    char buff[BUFFSZ];
    int i, e1l, e1r, e2l, e2r, c = 0;

    while (fgets(buff, BUFFSZ, infile)) {
        i = 0;
        e1l = conv_next_int_until_char(buff, '-', &i);
        e1r = conv_next_int_until_char(buff, ',', &i);
        e2l = conv_next_int_until_char(buff, '-', &i);
        e2r = conv_next_int_until_char(buff, '\n', &i);

        if (e1l >= e2l && e1r <= e2r) ++c; // 2 contains 1
        else if (e2l >= e1l && e2r <= e1r) ++c; // 1 contains 2
    }
    fclose(infile);

    printf("containing pairs == %d\n", c);
}
