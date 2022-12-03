#include <stdio.h>
#include <stdlib.h>

int read_next(FILE *file) {
    // reads the next number from the file and returns it
    // returns -1 on a blank line, 0 at EOF
    char buff[8];
    size_t i = 0;
    int n = -1;
    while ((buff[i++] = fgetc(file)) != EOF) {
        if (buff[i-1] == '\n') {
            buff[i-1] = 0;
            n = atoi(buff);
            if (!n) return -1;
            return n;
        }
    }
    return 0;
}

int main() {
    FILE *infile = fopen("input.txt", "r");
    if (!infile) return 1;
    int big1 = 0, big2 = 0, big3 = 0, current = 0, n;
    for (;;) {
        n = read_next(infile);
        if (n <= 0) {
            printf("top3 == [%d, %d, %d]\n", big1, big2, big3);
            printf("consolidating group of size %d\n", current);
            if (current > big1) {
                big3 = big2;
                big2 = big1;
                big1 = current;
            } else if (current > big2) {
                big3 = big2;
                big2 = current;
            } else if (current > big3) {
                big3 = current;
            }
            current = 0;
            if (n == 0) break;
        } else {
            current += n;
        }
    }
    fclose(infile);

    printf("top3 == [%d, %d, %d]; tot == %d\n", big1, big2, big3, big1+big2+big3);
}
