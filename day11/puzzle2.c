#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFFSZ 128
#define NUMMONKEYS 8
#define NUMROUNDS 10000
#define PRINTTAB printf("\t\t")
typedef long ITEM;
int DIVISORMULTIPLE = 1; // this could be their *L*CM but idc and it still works to choose any CM

void printarray(ITEM arr[], int sz) {
    if (sz == 0) {
        printf("None");
        return;
    }
    printf("[");
    for (int i = 0; i < sz; ++i) {
        printf("%ld, ", arr[i]);
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
    ITEM items[40];
    int numitems;
    enum Op op;
    int operand;
    int divisor;
    int truerecipient;
    int falserecipient;
    int inspections;
};

void givemonkeyitem(struct Monkey *monkey, ITEM item) {
    monkey->items[monkey->numitems++] = item;
}

void printmonkeys(struct Monkey monkeys[], int monkeycount) {
    for (int i = 0; i < monkeycount; ++i) {
        PRINTTAB;
        printf("Monkey %d\n", i);

        PRINTTAB;
        printf("  Items: ");
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

        PRINTTAB;
        printf("  Inspections: %d\n", monkeys[i].inspections);
    }
}

int parsemonkeys(struct Monkey monkeys[], const char *filename) {
    char buff[BUFFSZ];
    FILE *file = fopen(filename, "r");
    if (!file) exit(1);
    int curr = 0;
    monkeys[curr].numitems = 0;
    monkeys[curr].inspections = 0;
    while (fgets(buff, BUFFSZ, file)) {
        //printf("%s", buff);
        if (strncmp(buff, "\n", 1) == 0) {
            ++curr;
            monkeys[curr].numitems = 0;
            monkeys[curr].inspections = 0;
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
    return curr+1;
}

ITEM applyoperation(struct Monkey monkey, int i) {
    switch (monkey.op) {
        case OpAdd:
            printf("    worry level +%d to ", monkey.operand);
            return monkey.items[i] + monkey.operand;
        case OpMult:
            printf("    worry level *%d to ", monkey.operand);
            return monkey.items[i] * monkey.operand;
        case OpPow:
            printf("    worry level squared to ");
            return monkey.items[i] * monkey.items[i];
        default:
            printf("invalid operation: %d\n", monkey.op);
            exit(1);
    }
}

void playround(struct Monkey monkeys[], int monkeycount) {
    for (int m = 0; m < monkeycount; ++m) {
        printf("Monkey %d:\n", m);
        if (!monkeys[m].numitems) {
            printf("  No items!\n");
            continue;
        }
        for (int i = 0; i < monkeys[m].numitems; ++i) {
            ++monkeys[m].inspections;
            printf(" %d inspect %ld\n", m, monkeys[m].items[i]);

            monkeys[m].items[i] = applyoperation(monkeys[m], i) % DIVISORMULTIPLE;
            printf("%ld\n", monkeys[m].items[i]);

            // WORRY LEVEL NO LONGER DIV 3
            //monkeys[m].items[i] /= 3;
            //printf("    worry level div 3 to %d\n", monkeys[m].items[i]);

            if (monkeys[m].items[i] % monkeys[m].divisor == 0) {
                printf("    worry level %ld is divisible by %d; throw to %d\n",
                        monkeys[m].items[i], monkeys[m].divisor, monkeys[m].truerecipient);
                givemonkeyitem(&monkeys[monkeys[m].truerecipient], monkeys[m].items[i]);
            } else {
                printf("    worry level %ld is NOT divisible by %d; throw to %d\n",
                        monkeys[m].items[i], monkeys[m].divisor, monkeys[m].falserecipient);
                givemonkeyitem(&monkeys[monkeys[m].falserecipient], monkeys[m].items[i]);
            }
        }
        monkeys[m].numitems = 0; // all thrown
    }
}

int main() {
    struct Monkey monkeys[NUMMONKEYS];
    int monkeycount = parsemonkeys(monkeys, "input.txt");
    assert(monkeycount == NUMMONKEYS);
    
    // get a number which is divisible by all the divisors
    // optimally, would be their LCM, but this will do
    for (int m = 0; m < NUMMONKEYS; ++m) {
        if (DIVISORMULTIPLE % monkeys[m].divisor != 0)
            DIVISORMULTIPLE *= monkeys[m].divisor;
    }
    printf("DIVISORMULTIPLE=%d\n", DIVISORMULTIPLE);


    printmonkeys(monkeys, NUMMONKEYS);
    for (int i = 0; i < NUMROUNDS; ++i) {
        printf("round %d\n", i);
        playround(monkeys, NUMMONKEYS);
        printf("post round %d items and inspections: \n", i);
        for (int m = 0; m < NUMMONKEYS; ++m) {
            printf("  monkey %d: ", m);
            printarray(monkeys[m].items, monkeys[m].numitems);
            printf("; inspected %d times\n", monkeys[m].inspections);
        }
    }
    printmonkeys(monkeys, NUMMONKEYS);

    ITEM top1=0, top2=0;
    printf("inspections: ");
    for (int i = 0; i < NUMMONKEYS; ++i) {
        printf("%d, ", monkeys[i].inspections);
        if (monkeys[i].inspections >= top1) {
            top2 = top1;
            top1 = monkeys[i].inspections;
        } else if (monkeys[i].inspections >= top2) {
            top2 = monkeys[i].inspections;
        }
    }
    printf("\ntop 2 are %ld and %ld\n", top1, top2);
    printf("product is %ld\n", top1*top2);
}
