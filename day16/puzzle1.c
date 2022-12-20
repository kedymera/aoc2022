#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 128

struct Valve {
    char id[3];
    int rate;
    struct Valve **tunnels;
    int numtunnels;
};

void PrintValves(struct Valve *valves, int numvalves) {
    for (int i = 0; i < numvalves; ++i) {
        printf("Valve %s has rate %d and leads to ", valves[i].id, valves[i].rate);
        for (int j = 0; j < valves[i].numtunnels; ++j) {
            printf("%s", valves[i].tunnels[j]->id);
            if (j != valves[i].numtunnels - 1)
                printf(", ");
        }
        printf("\n");
    }
}

void FreeValves(struct Valve *valves, int numvalves) {
    for (int i = 0; i < numvalves; ++i) {
        free(valves[i].tunnels);
    }
    free(valves);
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
        (*valves)[i].tunnels = NULL;
        (*valves)[i].numtunnels = 0;
        ++i;
    }
    *numvalves = i;
    rewind(file);
    char temp[2];
    i = 0;
    while (fgets(buff, BUFFSZ, file)) {
        // skip 45 to jump roughly to where the tunnels are specified
        for (p = buff+45; *p; ++p) {
            if (*p >= 'A' && *p <= 'Z') {
                temp[0] = *p++; temp[1] = *p++;
                (*valves)[i].tunnels = realloc((*valves)[i].tunnels, ++(*valves)[i].numtunnels * sizeof(struct Valve *));
                for (int v = 0; v < *numvalves; ++v) {
                    if (temp[0] == (*valves)[v].id[0] && temp[1] == (*valves)[v].id[1]) {
                        (*valves)[i].tunnels[(*valves)[i].numtunnels-1] = &(*valves)[v];
                        break;
                    }
                }
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
    FreeValves(valves, numvalves);
}
