#include <stdio.h>
#include <stdlib.h>

int read_next(FILE *file) {
    // reads the next number from the file and returns it
    // returns -1 on a blank line, 0 at EOF
    char buff[8];
    size_t i = 0;
    int n = -1;
    while ((buff[i++] = fgetc(file)) != EOF) {
        printf("i=%lu, buf=%s\n", i, buff);
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
    int biggest = 0, current = 0, n;
    for (;;) {
        n = read_next(infile);
        printf("b=%d   c=%d   n=%d\n", biggest, current, n);
        if (n <= 0) {
            biggest = current > biggest ? current : biggest;
            current = 0;
            if (n == 0) break;
        } else {
            current += n;
        }
    }
    fclose(infile);

    printf("biggest == %d\n", biggest);
}
