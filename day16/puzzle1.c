#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 128
#define MAXTUNNELS 5

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

struct Valve {
    char id[3];
    int rate;
    int tunnels[MAXTUNNELS+1];
    bool open;
};

void PrintValves(struct Valve *valves, int numvalves) {
    printf("\nValves:\n");
    for (int i = 0; i < numvalves; ++i) {
        printf("Valve %s has rate %d and leads to ", valves[i].id, valves[i].rate);
        for (int j = 0; valves[i].tunnels[j] != -1; ++j) {
            printf("%s,", valves[valves[i].tunnels[j]].id);
        }
        printf("\n");
    }
}

void PrintDists(int **dists, int numvalves) {
    printf("\nDistances:\n");
    for (int j = 0; j < numvalves; ++j) {
        for (int i = 0; i < numvalves; ++i) {
            if (dists[i][j] < 10)
                printf("%d ", dists[i][j]);
            else
                printf(". ");
        }
        printf("\n");
    }
}

int **InitDists(struct Valve *valves, int numvalves) {
    int **dists = malloc(numvalves * sizeof(int *));
    for (int i = 0; i < numvalves; ++i) {
        dists[i] = malloc(numvalves * sizeof(int));
        for (int j = 0; j < numvalves; ++j) {
            dists[i][j] = 999; // bigger than any possible path
        }
    }

    // Floyd-Warshall
    for (int i = 0; i < numvalves; ++i) {
        dists[i][i] = 0;
        for (int j = 0; valves[i].tunnels[j] != -1; ++j) {
            dists[i][valves[i].tunnels[j]] = 1;
        }
    }
    for (int k = 0; k < numvalves; ++k)
        for (int i = 0; i < numvalves; ++i)
            for (int j = 0; j < numvalves; ++j)
                dists[i][j] = min(dists[i][j], dists[i][k] + dists[k][j]);

    return  dists;
}

void FreeDists(int **dists, int numvalves) {
    for (int i = 0; i < numvalves; ++i)
        free(dists[i]);
    free(dists);
}

void ParseValves(const char *filename, struct Valve **valves, int *numvalves) {
    char buff[BUFFSZ], *p;
    FILE *file = fopen(filename, "r");
    int i = 0;
    while (fgets(buff, BUFFSZ, file)) {
        printf("%s", buff);

        *valves = realloc(*valves, (i+1) * sizeof(struct Valve));
        // get current valve's name by scanning until capital (rest of text is lowercase)
        for (p = buff+1; *p; ++p) {
            if (*p >= 'A' && *p <= 'Z') {
                (*valves)[i].id[0] = *p++;
                (*valves)[i].id[1] = *p++;
                (*valves)[i].id[2] = 0;
                break;
            } 
        }
        for (; *p; ++p) {
            if (*p >= '0' && *p <= '9') {
                (*valves)[i].rate = strtol(p, &p, 10);
            }
        }
        for (int t = 0; t < MAXTUNNELS+1; ++t)
            (*valves)[i].tunnels[t] = -1;
        (*valves)[i].open = false;
        ++i;
    }
    *numvalves = i;
    rewind(file);
    char temp[2];
    i = 0;
    int t;
    while (fgets(buff, BUFFSZ, file)) {
        // skip 45 to jump roughly to where the tunnels are specified
        t = 0;
        for (p = buff+45; *p; ++p) {
            if (*p >= 'A' && *p <= 'Z') {
                temp[0] = *p++; temp[1] = *p++;
                for (int v = 0; v < *numvalves; ++v) {
                    if (temp[0] == (*valves)[v].id[0] && temp[1] == (*valves)[v].id[1]) {
                        (*valves)[i].tunnels[t] = v;
                        break;
                    }
                }
                ++t;
            }
        }
        ++i;
    }
    fclose(file);
}

int MaxPressure(struct Valve *valves, int numvalves, int **dists, int start, int time) {
    if (time <= 1) return 0;

    int maxpressure = 0;
    for (int i = 0; i < numvalves; ++i) {
        // do we want to go to this valve?
        if (valves[i].rate == 0) continue;
        if (valves[i].open) continue;

        valves[i].open = true;
        int timeaftermove = time - dists[start][i] - 1; // -1 for time to open valve at dest
        maxpressure = max(maxpressure, timeaftermove * valves[i].rate + MaxPressure(valves, numvalves, dists, i, timeaftermove));
        valves[i].open = false;
    }
    return maxpressure;
}

int main() {
    struct Valve *valves = NULL;
    int numvalves = 0;
#ifdef SMOL
    ParseValves("inputsmol.txt", &valves, &numvalves);
#else
    ParseValves("input.txt", &valves, &numvalves);
#endif
    PrintValves(valves, numvalves);

    int **dists = InitDists(valves, numvalves);
    PrintDists(dists, numvalves);

    int start = 0;
    while (valves[start].id[0] != 'A' || valves[start].id[1] != 'A')
        ++start;
    printf("max pressure: %d\n", MaxPressure(valves, numvalves, dists, start, 30));

    FreeDists(dists, numvalves);
    free(valves);
}
