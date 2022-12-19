#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 1024
#define XMIN 440
#define XMAX 510
#define YMIN 0
#define YMAX 504

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        printf("%s", buff);
        for (char *p = buff; *p; ++p) {
            if (*p >= '0' && *p <= '9') {
                long x = strtol(p, &p, 10);
                // p now on comma after x value
                ++p;
                long y = strtol(p, &p, 10);
                printf("[%ld, %ld]->", x, y);
            }
        }
        printf("\n");
    }
    printf("hello world\n");
}
