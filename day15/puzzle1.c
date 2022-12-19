#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFSZ 128
#define ROW 2000000
#define ABS(x) ((x) >= 0 ? (x) : -(x))

struct endpt {
    long x;
    bool left;
};

long l1metric(long x1, long y1, long x2, long y2) {
    return ABS(x1-x2) + ABS(y1-y2);
}

long ReadNextNumber(char **p) {
    for (; **p; ++*p) {
        if (**p == '-' || (**p >= '0' && **p <= '9')) {
            return strtol(*p, p, 10);
        }
    }
    printf("not reached\n");
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

void ReadSensor(char buff[BUFFSZ], struct endpt **endpts, int *numendpts) {
    long sx, sy, bx, by;
    sx = ReadNextNumber(&buff);
    sy = ReadNextNumber(&buff);
    bx = ReadNextNumber(&buff);
    by = ReadNextNumber(&buff);
    printf("s:[%ld,%ld], b:[%ld,%ld]\n", sx, sy, bx, by);

    long radius = l1metric(sx,sy,bx,by);
    printf("l1 disc radius: %ld\n", radius);

    long dx = FindIntersections(sy, radius, ROW);
    if (dx != -1) {
        printf("line and disc intersect at x=%ld and x=%ld\n", sx - dx, sx + dx);
        *numendpts += 2;
        *endpts = realloc(*endpts, *numendpts * sizeof(struct endpt));
        (*endpts)[*numendpts-2].x = sx-dx;
        (*endpts)[*numendpts-2].left = true;
        (*endpts)[*numendpts-1].x = sx+dx;
        (*endpts)[*numendpts-1].left = false;
    }
    else
        printf("line and disc don't intersect\n");
}

int main() {
    struct endpt *endpts = NULL;
    int numendpts = 0;

    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    while (fgets(buff, BUFFSZ, file)) {
        printf("%s", buff);
        ReadSensor(buff, &endpts, &numendpts);
        printf("\n");
    }
    
    printf("endpts:\n");
    for (int i = 0; i < numendpts; ++i) {
        printf("%ld - ", endpts[i].x);
        if (endpts[i].left) printf("l\n");
        else printf("r\n");
    }
    
    free(endpts);
}
