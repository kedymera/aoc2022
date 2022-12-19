#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFSZ 256
#define PRINTDEPTH for (int __i = depth-1; __i > 0; --__i) printf("  ");

struct PacketData {
    struct PacketData *parent;
    long number;
    struct PacketData *list;
    size_t listsz;
};

void PrintPacket(struct PacketData *packet) {
    if (packet->number >= 0) {
        printf("%ld", packet->number);
        return;
    }
    printf("[");
    for (size_t i = 0; i < packet->listsz; ++i) {
        PrintPacket(packet->list+i);
        if (i != packet->listsz - 1) printf(",");
    }
    printf("]");
}

void FreePacket(struct PacketData *packet) {
    if (packet->number < 0) {
        for (size_t i = 0; i< packet->listsz; ++i)
            FreePacket(packet->list+i);
        free(packet->list);
    }
}

void ExtendList(struct PacketData *packet) {
    packet->list = realloc(packet->list, ++packet->listsz * sizeof(struct PacketData));
}
void AppendNumber(struct PacketData *packet, long number) {
    ExtendList(packet);
    packet->list[packet->listsz-1].parent = packet;
    packet->list[packet->listsz-1].number = number;
    packet->list[packet->listsz-1].list = NULL;
    packet->list[packet->listsz-1].listsz = 0;
}
void AppendList(struct PacketData *packet) {
    AppendNumber(packet, -1);
}


int main() {
    struct PacketData *pack = malloc(sizeof(struct PacketData));
    pack->parent = NULL;
    pack->number = -1;
    pack->list = NULL;
    pack->listsz = 0;

    AppendNumber(pack, 54);
    AppendList(pack);
    AppendNumber(pack->list+pack->listsz-1, 5);
    AppendList(pack);
    AppendNumber(pack, 514);
    AppendNumber(pack, 525);

    PrintPacket(pack);
    printf("\n");


    FreePacket(pack);
    free(pack);

return 0;


//    char buff[BUFFSZ];
//    FILE *file = fopen("input.txt", "r");
//    if (!file) return 1;
//
//    struct PacketDataList packet1;
//    packet1.list = malloc(0);
//    packet1.sz = 0;
//    int depth = 1;
//    struct PacketDataList *curr = &packet1;
//
//    while (fgets(buff, BUFFSZ, file)) {
//        printf("string packet:\n");
//        printf("%s", buff);
//        for (char *p = buff; *p; ++p) {
//            if (*p >= '0' && *p <= '9') {
//                long a = strtol(p, &p, 10);
//                AppendNumber(curr, a);
//            }
//            if (*p == ',') {
//                // do nothing?
//            }
//            if (*p == '[') {
//                ++depth;
//                AppendList(curr);
//                curr = &curr->list[curr->sz-1].list;
//            }
//            if (*p == ']') {
//                --depth;
//                // TODO: go back to parent
//            }
//            if (!depth) break;
//        }
//        printf("parsed packet:\n");
//        PrintPacket(&packet1);
//        printf("\n\n\n");
//    }
//    printf("hello world\n");
}
