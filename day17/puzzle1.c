#include <stdio.h>
#include <stdlib.h>

#ifdef SMOL
#define BUFFSZ 128
#else
#define BUFFSZ 10100
#endif

int main() {
    char buff[BUFFSZ];
#ifdef SMOL
    FILE *file = fopen("inputsmol.txt", "r");
#else
    FILE *file = fopen("input.txt", "r");
#endif
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        printf("%s", buff);
    }
    printf("hello world\n");
}
