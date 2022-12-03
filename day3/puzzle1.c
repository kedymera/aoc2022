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

    while (fgets(buff, BUFFSZ, infile)) {
        memset(table, 0, TABLESZ); 
        len = strlen(buff) - 1; // -1 to strip newline
        // first half: tally occurrences of each char
        for (i = 0; i < len/2; ++i) {
            ++table[(int) buff[i]];
        }
        // second half: check if the char-to-add was counted in first half
        for (; i < len; ++i) {
            if (table[(int) buff[i]]) break;
        }
        // buff[i] must now be the duplicated char
        if (buff[i] <= 'Z') priosum += buff[i] - 'A' + 27;
        else priosum += buff[i] - 'a' + 1;
        
        printf("priosum up to %d by line %s", priosum, buff);
    }
    fclose(infile);

    printf("priosum == %d\n", priosum);
}
