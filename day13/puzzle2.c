#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFFSZ 256
#define NUMPACKETS 302
#define MIN(a,b) (a) < (b) ? (a) : (b)

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
    for (char *p = buff+1; *p; ++p) {
        if (*p >= '0' && *p <= '9')
            AppendNumber(packet, strtol(p, &p, 10));
        if (*p == '[') {
            AppendList(packet);
            packet = packet->list + packet->listsz-1;
        }
        if (*p == ']') {
            if (packet->parent == NULL)
                return packet;
            packet = packet->parent;
        }
    }
    return NULL;
}

void NumberToSingleton(struct PacketData *numberdata) {
    assert(numberdata->number >= 0 && numberdata->list == NULL);
    numberdata->list = malloc(sizeof(struct PacketData));
    numberdata->listsz = 1;

    numberdata->list->parent = NULL;
    numberdata->list->list = NULL;
    numberdata->list->listsz = 0;
    numberdata->list->number = numberdata->number;

    numberdata->number = -1;
}


int PacketCompare(struct PacketData *left, struct PacketData *right) {
    size_t minlen = MIN(left->listsz, right->listsz);
    for (size_t i = 0; i < minlen; ++i) {
        long l = left->list[i].number, r = right->list[i].number;
        if (l >= 0 && r >= 0) { // 'number' is nonneg if packetdata is number (-1 if list)
            if (l < r) return 1;
            else if (l > r) return -1;
            else continue;
        }

        // know at least one is a list; convert the other if need be
        if (l >= 0)
            NumberToSingleton(left->list + i);
        if (r >= 0)
            NumberToSingleton(right->list + i);

        int subres = PacketCompare(left->list + i, right->list + i);
        if (subres) // was conclusive
            return subres;
    }
    return right->listsz - left->listsz; // accounts for 'running out' clause
}
int PacketCompareVoidWrapper(const void *LEFT, const void *RIGHT) {
    return PacketCompare(*(struct PacketData **) LEFT, *(struct PacketData **) RIGHT);
}

int main() {

    struct PacketData **packets = malloc(NUMPACKETS * sizeof(struct PacketData *));
    // add divider packets
    int index = 0;
    packets[index] = CreateList();
    AppendList(packets[index]);
    AppendNumber(packets[index]->list, 2);
    printf("packet %d: ", index);
    PrintPacket(packets[index]);
    printf("\n");
    ++index;
    packets[index] = CreateList();
    AppendList(packets[index]);
    AppendNumber(packets[index]->list, 6);
    printf("packet %d: ", index);
    PrintPacket(packets[index]);
    printf("\n");
    ++index;
    
    struct PacketData *divider2 = packets[0];
    struct PacketData *divider6 = packets[1];

    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;

    while (fgets(buff, BUFFSZ, file)) {
        packets[index] = ParsePacket(buff);
        printf("packet %d: ", index);
        PrintPacket(packets[index]);
        printf("\n");
        ++index;

        fgets(buff, BUFFSZ, file);
        packets[index] = ParsePacket(buff);
        printf("packet %d: ", index);
        PrintPacket(packets[index]);
        printf("\n");
        ++index;

        fgets(buff, BUFFSZ, file); // waste the empty line
    }
    assert(index == NUMPACKETS);

    // sort!
    qsort(packets, NUMPACKETS, sizeof(struct PacketData *), &PacketCompareVoidWrapper);
    printf("SORTED!\n");
    
    int index2, index6;
    for (int i = 1; i <= NUMPACKETS; ++i) {
        // iterate in reverse because the comparator is backwards x)
        if (packets[NUMPACKETS-i] == divider2) index2 = i;
        if (packets[NUMPACKETS-i] == divider6) index6 = i;
    }

    printf("[[2]] divider at %d\n", index2);
    printf("[[6]] divider at %d\n", index6);
    printf("product is %d\n", index2*index6);

    for (int i = 0; i < NUMPACKETS; ++i) {
        FreePacket(packets[i]);
    }
    free(packets);
}
