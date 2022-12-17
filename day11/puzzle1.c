#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 128
#define NUMMONKEYS 8
#define PRINTTAB printf("\t\t")

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

enum Op {
    OpAdd,
    OpMult,
    OpPow,
};

struct Monkey {
    int items[40];
    int numitems;
    enum Op op;
    int operand;
    int divisor;
    int truerecipient;
    int falserecipient;
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

        if (strncmp(buff, "  Operation: new = old ", 23) == 0) {
            if (buff[23] == '+') monkeys[curr].op = OpAdd;
            if (buff[23] == '*') {
                if (buff[25] == 'o') {
                    monkeys[curr].op = OpPow;
                    monkeys[curr].operand = 2;
                }
                else monkeys[curr].op = OpMult;
            }
            int i = 25;
            monkeys[curr].operand = conv_next_int_until_char(buff, '\n', &i);
        }

        if (strncmp(buff, "  Test: divisible by ", 21) == 0) {
            int i = 21;
            monkeys[curr].divisor = conv_next_int_until_char(buff, '\n', &i);
        }

        if (strncmp(buff, "    If true: throw to monkey ", 29) == 0) {
            int i = 29;
            monkeys[curr].truerecipient = conv_next_int_until_char(buff, '\n', &i);
        }

        if (strncmp(buff, "    If false: throw to monkey ", 30) == 0) {
            int i = 30;
            monkeys[curr].falserecipient = conv_next_int_until_char(buff, '\n', &i);
        }
    }

    for (int i = 0; i < 8; ++i) {
        PRINTTAB;
        printf("Monkey %d\n", i);

        PRINTTAB;
        printf("  Starting items: ");
        printarray(monkeys[i].items, monkeys[i].numitems);
        printf("\n");

        PRINTTAB;
        printf("  Operation: new = old ");
        switch(monkeys[i].op) {
            case OpAdd:
                printf("+ %d\n", monkeys[i].operand);
                break;
            case OpMult:
                printf("* %d\n", monkeys[i].operand);
                break;
            case OpPow:
                printf("* old\n");
                break;
        }

        PRINTTAB;
        printf("  Test: divisible by %d\n", monkeys[i].divisor);

        PRINTTAB;
        printf("    If true: throw to monkey %d\n", monkeys[i].truerecipient);

        PRINTTAB;
        printf("    If false: throw to monkey %d\n", monkeys[i].falserecipient);
    }
}
