#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFSZ 128
#define SEARCHMIN 0
#define SEARCHMAX 4000000
#define ABS(x) ((x) >= 0 ? (x) : -(x))

struct endpt {
    long x;
    bool left;
};
struct sensor {
    long x;
    long y;
    long bx;
    long by;
};

int EndPtCompare(const void *endpt1, const void *endpt2) {
    // sort by x value
    return (*(struct endpt *) endpt1).x - (*(struct endpt *) endpt2).x;
}

long l1metric(long x1, long y1, long x2, long y2) {
    return ABS(x1-x2) + ABS(y1-y2);
}

long ReadNextNumber(char **p) {
    for (; **p; ++*p) {
        if (**p == '-' || (**p >= '0' && **p <= '9')) {
            return strtol(*p, p, 10);
        }
    }
    return 0;
}

long FindIntersections(long cy, long r, long ly) {
    // returns the x-distance left and right of the circle's centre at which the line and circle intersect
    if (cy <= ly && ly <= cy+r) { // intersect on top half of circle
        return r + cy-ly;
    } else if (cy-r <= ly && ly <= cy) { // intersect on bottom half of circle
        return r + ly-cy;
    } else { // do not intersect
        return -1;
    }
}

void ReadSensor(char buff[BUFFSZ], struct sensor **sensors, int *numsensors) {
    *sensors = realloc(*sensors, ++*numsensors * sizeof(struct sensor));
    (*sensors)[*numsensors-1].x = ReadNextNumber(&buff);
    (*sensors)[*numsensors-1].y = ReadNextNumber(&buff);
    (*sensors)[*numsensors-1].bx = ReadNextNumber(&buff);
    (*sensors)[*numsensors-1].by = ReadNextNumber(&buff);
}

void AnalyseSensors(struct sensor *sensors, int numsensors, struct endpt **endpts, int *numendpts, long row) {
    for (int i = 0; i < numsensors; ++i) {
        long radius = l1metric(sensors[i].x,sensors[i].y,sensors[i].bx,sensors[i].by);

        long dx = FindIntersections(sensors[i].y, radius, row);
        if (dx != -1) {
            *numendpts += 2;
            *endpts = realloc(*endpts, *numendpts * sizeof(struct endpt));
            (*endpts)[*numendpts-2].x = sensors[i].x-dx;
            (*endpts)[*numendpts-2].left = true;
            (*endpts)[*numendpts-1].x = sensors[i].x+dx;
            (*endpts)[*numendpts-1].left = false;
        }

    }
}

int main() {
    char buff[BUFFSZ];
    struct sensor *sensors = NULL;
    int numsensors = 0;

    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        ReadSensor(buff, &sensors, &numsensors);
    }
    fclose(file);
    
    for (int i = 0; i < numsensors; ++i) {
        printf("sensors [%ld,%ld] -> beacon [%ld, %ld]\n", sensors[i].x, sensors[i].y, sensors[i].bx, sensors[i].by);
    }

    for (long row = 0; row <= 4000000; ++row) {
        printf("row==%ld\n", row);
        struct endpt *endpts = NULL;
        int numendpts = 0;

        AnalyseSensors(sensors, numsensors, &endpts, &numendpts, row);
        
        qsort(endpts, numendpts, sizeof(struct endpt), &EndPtCompare);

        int nesting = 0;
        long currleft;
        long lastright = INT_MIN;
        for (int i = 0; i < numendpts; ++i) {
            if (nesting == 0) {
                assert(endpts[i].left);
                currleft = endpts[i].x;
                if (lastright == currleft) ++currleft;
                if (lastright < currleft - 1
                    && SEARCHMIN <= currleft-1 && currleft-1 <= SEARCHMAX
                    && SEARCHMIN <= row && row <= SEARCHMAX) { // a clear gap
                    printf("unknown beacon at [%ld, %ld]\n", currleft - 1, row);
                    printf("tuning frequency is %ld\n", 4000000*(currleft-1) + row);
                    free(endpts);
                    free(sensors);
                    exit(0);
                }
            }

            if (endpts[i].left)
                ++nesting;
            else
                --nesting;

            if (nesting == 0) {
                lastright = endpts[i].x;
            }
        }
        free(endpts);
    }
    free(sensors);
}
