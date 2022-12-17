#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 128
#define NUMMONKEYS 8

void printarray(int arr[], int sz) {
    printf("[");
    for (int i = 0; i < sz; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("\b\b]");
}

int conv_next_int_until_char(char *str, char ch, int *i) {
    int start = *i;
    for (; str[*i]; ++*i)
        if (str[*i] == ch || str[*i] == '\n')
            break;
    str[*i] = 0;
    ++*i;
    return atoi(str+start);
}

struct Monkey {
    int items[40];
    int numitems;
};

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    struct Monkey monkeys[NUMMONKEYS];
    int curr = 0;
    monkeys[curr].numitems = 0;
    while (fgets(buff, BUFFSZ, file)) {
        printf("%s", buff);
        if (strncmp(buff, "\n", 1) == 0) {
            ++curr;
            monkeys[curr].numitems = 0;
        }

        if (strncmp(buff, "  Starting items: ", 18) == 0) {
            int i = 18;
            int item;
            while ((item = conv_next_int_until_char(buff, ',', &i))) {
                monkeys[curr].items[monkeys[curr].numitems++] = item;
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        printf("monkey %d has %d items: ", i, monkeys[i].numitems);
        printarray(monkeys[i].items, monkeys[i].numitems);
        printf("\n");
    }
}
