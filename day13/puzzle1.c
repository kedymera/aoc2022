#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFSZ 256

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

void FreePacketRecursive(struct PacketData *packet) {
    if (packet->number < 0) {
        for (size_t i = 0; i< packet->listsz; ++i)
            FreePacketRecursive(packet->list+i);
        free(packet->list);
    }
}
void FreePacket(struct PacketData *packet) {
    FreePacketRecursive(packet);
    free(packet);
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
struct PacketData *CreateList() {
    struct PacketData *packet = malloc(sizeof(struct PacketData));
    packet->parent = NULL;
    packet->number = -1;
    packet->list = NULL;
    packet->listsz = 0;
    return packet;
}

struct PacketData *ParsePacket(char *buff) {
    struct PacketData *packet = CreateList();
    struct PacketData *curr = packet;
    int depth = 1;
    for (char *p = buff+1; *p; ++p) {
        if (*p >= '0' && *p <= '9')
            AppendNumber(curr, strtol(p, &p, 10));
        if (*p == '[') {
            ++depth;
            AppendList(curr);
            curr = curr->list + curr->listsz-1;
        }
        if (*p == ']') {
            --depth;
            curr = curr->parent;
        }

        if (!depth) break;
    }
    return packet;
}

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;

    while (fgets(buff, BUFFSZ, file)) {
        struct PacketData *packet1 = ParsePacket(buff);
        printf("packet1: ");
        PrintPacket(packet1);
        printf("\n");

        fgets(buff, BUFFSZ, file);
        struct PacketData *packet2 = ParsePacket(buff);
        printf("packet2: ");
        PrintPacket(packet2);
        printf("\n\n");

        FreePacket(packet1);
        FreePacket(packet2);

        fgets(buff, BUFFSZ, file); // waste the empty line
    }
    printf("hello world\n");
}
