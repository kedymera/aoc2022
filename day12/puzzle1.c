#include <stdio.h>

#define BUFFSZ 128

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        printf("%s", buff);
    }
    printf("hello world\n");
}
