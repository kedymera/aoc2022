#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 128

int addsignal(int cycle, int X) {
    if (cycle % 40 == 20) return cycle*X;
    return 0;
}

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    int X = 1;
    int cycle = 1;
    int signalsum = 0;
    while (fgets(buff, BUFFSZ, file)) {
        switch (buff[0]) {
            case 'a': // addx
                signalsum += addsignal(cycle++, X);
                signalsum += addsignal(cycle++, X);
                X += atoi(buff+5);
                break;
            case 'n': // noop
                signalsum += addsignal(cycle++, X);
                break;
            default:
                printf("invalid instruction\n");
                exit(1);
        }
    }
    printf("signalsum == %d\n", signalsum);
}
