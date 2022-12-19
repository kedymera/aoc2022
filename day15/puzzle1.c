#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 128
#define ROW 2000000

long ReadNextNumber(char **p) {
    for (; **p; ++*p) {
        if (**p == '-' || (**p >= '0' && **p <= '9')) {
            return strtol(*p, p, 10);
        }
    }
    printf("not reached\n");
    return 0;
}

void ReadSensor(char buff[BUFFSZ]) {
    long sx, sy, bx, by;
    sx = ReadNextNumber(&buff);
    sy = ReadNextNumber(&buff);
    bx = ReadNextNumber(&buff);
    by = ReadNextNumber(&buff);
    printf("s:[%ld,%ld], b:[%ld,%ld]\n", sx, sy, bx, by);
}

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        printf("%s", buff);
        ReadSensor(buff);
    }
    printf("hello world\n");
}
