#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BUFFSZ 128

struct Valve {
    char id[3];
    int rate;
    int tunnels[5];
};

void PrintValves(struct Valve *valves, int numvalves) {
    for (int i = 0; i < numvalves; ++i) {
        printf("Valve %s has rate %d and leads to ", valves[i].id, valves[i].rate);
        for (int j = 0; valves[i].tunnels[j] != -1; ++j) {
            printf("%s,", valves[valves[i].tunnels[j]].id);
        }
        printf("\n");
    }
}

int **InitDists(struct Valve *valves, int numvalves) {
    int **dists = malloc(numvalves * sizeof(int *));
    for (int i = 0; i < numvalves; ++i) {
        dists[i] = malloc(numvalves * sizeof(int));
        for (int j = 0; j < numvalves; ++j) {
            dists[i][j] = INT_MAX;
        }
    }

    return dists;
}

void PrintDists(int **dists, int numvalves) {
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
        for (int t = 0; t < 5; ++t)
            (*valves)[i].tunnels[t] = -1;
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

int main() {
    struct Valve *valves = NULL;
    int numvalves = 0;
#ifdef SMOL
    ParseValves("inputsmol.txt", &valves, &numvalves);
#else
    ParseValves("input.txt", &valves, &numvalves);
#endif
    printf("\nparsed valves: \n");
    PrintValves(valves, numvalves);

    int **dists = InitDists(valves, numvalves);
    PrintDists(dists, numvalves);

    FreeDists(dists, numvalves);
    free(valves);
}
