#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFSZ 256

long next_number(char **p) {
    for (char *q = *p; *q; ++q) {
        if (*q >= '0' && *q <= '9') {
            long a = strtol(q, &q, 10);
            *p = q;
            return a;
        }
    }
    assert(0);
    return -1;
}

int main() {
    char buff[BUFFSZ];
#ifdef SMOL
    FILE *file = fopen("inputsmol.txt", "r");
#else
    FILE *file = fopen("input.txt", "r");
#endif
    if (!file) return 1;
    long id, ore_orecost, clay_orecost, obs_orecost, obs_claycost, geode_orecost, geode_obscost;
    while (fgets(buff, BUFFSZ, file)) {
        char *p = buff;
        id = next_number(&p);
        ore_orecost = next_number(&p);
        clay_orecost = next_number(&p);
        obs_orecost = next_number(&p);
        obs_claycost = next_number(&p);
        geode_orecost = next_number(&p);
        geode_obscost = next_number(&p);
        printf("bp %ld: orerobot costs %ld ore; clayrobot costs %ld ore; obsrobot costs %ld ore, %ld clay; geoderobot costs %ld ore, %ld obs\n", id, ore_orecost, clay_orecost, obs_orecost, obs_claycost, geode_orecost, geode_obscost);
    }
    printf("hello world\n");
}
