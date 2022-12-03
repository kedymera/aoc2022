#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//easily big enough to store each line
#define BUFFSZ 128

// just needs to be big enough to store upper-/lowercase ascii
// (idc packing for low mem. it wastes space, deal with it)
#define TABLESZ 128

int main() {
    FILE *infile = fopen("input.txt", "r");
    if (!infile) return 1;
    char buff[BUFFSZ], table[TABLESZ], len;
    int i, priosum = 0;
    char which = 0;

    while (fgets(buff, BUFFSZ, infile)) {
        if (which == 0)
            memset(table, 0, TABLESZ); 
        len = strlen(buff) - 1; // -1 to strip newline
        // set bit 0,1,2 to track which elves in group have which item
        for (i = 0; i < len; ++i) {
            table[(int) buff[i]] |= (1<<which);
            if (table[(int) buff[i]] == 0b111) {
                if (buff[i] <= 'Z') priosum += buff[i] - 'A' + 27;
                else priosum += buff[i] - 'a' + 1;
                break;
            }
        }
        printf("priosum up to %d by line %s", priosum, buff);

        which = (which + 1) % 3;
    }
    fclose(infile);

    printf("priosum == %d\n", priosum);
}
