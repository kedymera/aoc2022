#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 128

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    int X = 1;
    int cycle = 0;
    while (fgets(buff, BUFFSZ, file)) {
        switch (buff[0]) {
            case 'a': // addx
                X += atoi(buff+5);
                cycle += 2;
                break;
            case 'n': // noop
                ++cycle;
                break;
            default:
                printf("invalid instruction\n");
                exit(1);
        }
        printf("%s", buff);
        printf("cycle %d: X=%d\n", cycle, X);
    }
}
