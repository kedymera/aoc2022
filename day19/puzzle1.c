#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFSZ 256

#define TOTTIME 24
int id, ore_orecost, clay_orecost, obs_orecost, obs_claycost, geode_orecost, geode_obscost, maxorecost;
int totquality = 0;

int next_number(char **p) {
    for (char *q = *p; *q; ++q) {
        if (*q >= '0' && *q <= '9') {
            int a = strtol(q, &q, 10);
            *p = q;
            return a;
        }
    }
    assert(0);
    return -1;
}

int max(int a, int b) {
    return a>b?a:b;
}

int MaximiseGeodes(int time,
        int ore, int clay, int obs, int geode,
        int orerobot, int clayrobot, int obsrobot, int geoderobot) {
    if (time <= 0) return geode;
    if (time <= 1) return geode+geoderobot;
    
    int maxgeodes = 0;
    // consider each possible purchase (or no purchase)
    // conditions are:
    //   can we afford it?
    //   do we already make enough of this thing per turn?
    if (ore >= geode_orecost && obs >= geode_obscost) {
        maxgeodes = max(maxgeodes, MaximiseGeodes(time-1, ore+orerobot-geode_orecost, clay+clayrobot, obs+obsrobot-geode_obscost, geode+geoderobot, orerobot, clayrobot, obsrobot, geoderobot+1));
    }
    if (ore >= obs_orecost && clay >= obs_claycost
            && obsrobot < geode_obscost) {
        maxgeodes = max(maxgeodes, MaximiseGeodes(time-1, ore+orerobot-obs_orecost, clay+clayrobot-obs_claycost, obs+obsrobot, geode+geoderobot, orerobot, clayrobot, obsrobot+1, geoderobot));
    }
    if (ore >= clay_orecost
            && clayrobot < obs_claycost) {
        maxgeodes = max(maxgeodes, MaximiseGeodes(time-1, ore+orerobot-clay_orecost, clay+clayrobot, obs+obsrobot, geode+geoderobot, orerobot, clayrobot+1, obsrobot, geoderobot));
    }
    if (ore >= ore_orecost
            && orerobot < maxorecost) {
        maxgeodes = max(maxgeodes, MaximiseGeodes(time-1, ore+orerobot-ore_orecost, clay+clayrobot, obs+obsrobot, geode+geoderobot, orerobot+1, clayrobot, obsrobot, geoderobot));
    }
    maxgeodes = max(maxgeodes, MaximiseGeodes(time-1, ore+orerobot, clay+clayrobot, obs+obsrobot, geode+geoderobot, orerobot, clayrobot, obsrobot, geoderobot));

    return maxgeodes;
}

int main() {
    char buff[BUFFSZ];
#ifdef SMOL
    FILE *file = fopen("inputsmol.txt", "r");
#else
    FILE *file = fopen("input.txt", "r");
#endif
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        char *p = buff;
        id = next_number(&p);
        ore_orecost = next_number(&p);
        clay_orecost = next_number(&p);
        obs_orecost = next_number(&p);
        obs_claycost = next_number(&p);
        geode_orecost = next_number(&p);
        geode_obscost = next_number(&p);
        maxorecost = max(ore_orecost, max(clay_orecost, max(obs_orecost, geode_orecost)));

        printf("bp %d: orerobot costs %d ore; clayrobot costs %d ore; obsrobot costs %d ore, %d clay; geoderobot costs %d ore, %d obs\n", id, ore_orecost, clay_orecost, obs_orecost, obs_claycost, geode_orecost, geode_obscost);

        int maxgeodes = MaximiseGeodes(TOTTIME, 0, 0, 0, 0, 1, 0, 0, 0);
        printf("max geodes from this blueprint is %d\n", maxgeodes);
        int quality = id * maxgeodes;
        printf("quality: %d\n", quality);
        totquality += quality;
    }
    printf("total quality: %d\n", totquality);
}
