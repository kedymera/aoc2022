#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 128

void CRTDraw(int cycle, int X) {
    int drawnpix = (cycle-1) % 40;
    if (drawnpix - X >= -1 && drawnpix - X <= 1) printf("#");
    else printf(".");
    if (drawnpix == 39) printf("\n");
}

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    int X = 1;
    int cycle = 1;
    while (fgets(buff, BUFFSZ, file)) {
        switch (buff[0]) {
            case 'a': // addx
                CRTDraw(cycle++, X);
                CRTDraw(cycle++, X);
                X += atoi(buff+5);
                break;
            case 'n': // noop
                CRTDraw(cycle++, X);
                break;
            default:
                printf("invalid instruction\n");
                exit(1);
        }
    }
}
