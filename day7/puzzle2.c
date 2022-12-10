#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSZ 128
#define MAXNAME 64

#define FSCAP 70000000
#define UPDATESZ 30000000
int PUZZLE1ANSWER = 0;
int PUZZLE2ANSWER = 2000000000;

int conv_next_int_until_char(char *str, char ch, int *i) {
    int start = *i;
    for (; str[*i]; ++*i)
        if (str[*i] == ch)
            break;
    str[*i] = 0;
    ++*i;
    return atoi(str+start);
}

struct Directory {
    struct Directory *parent;
    struct Directory **subdirectories;
    size_t subdir_count;
    char *name;
    int size_of_immediate_files;
};

void AddDirectory(struct Directory *parent, struct Directory *child) {
    ++parent->subdir_count;
    parent->subdirectories = realloc(parent->subdirectories, parent->subdir_count * sizeof(struct Directory *));
    parent->subdirectories[parent->subdir_count - 1] = child;
}

struct Directory *CreateDirectory(struct Directory *parent, char *name) {
    struct Directory *dir = malloc(sizeof(struct Directory));
    dir->parent = parent;
    dir->subdirectories = NULL;
    dir->subdir_count = 0;
    dir->name = calloc(MAXNAME, sizeof(char));
    memcpy(dir->name, name, MAXNAME);
    dir->size_of_immediate_files = 0;
    if (parent) {
        AddDirectory(parent, dir);
    }
    return dir;
}

void ChangeDirectory(struct Directory *root, struct Directory **cwd, char *name) {
    if (strcmp(name, "/") == 0) {
        *cwd = root;
        return;
    }
    if (strcmp(name, "..") == 0) {
        *cwd = (*cwd)->parent;
        return;
    }
    for (int i = 0; (*cwd)->subdirectories[i]; ++i) {
        if (strcmp(name, (*cwd)->subdirectories[i]->name) == 0) {
            *cwd = (*cwd)->subdirectories[i];
            return;
        }
    }
    fprintf(stderr, "DO NOT REACH\n");
}

void printwd(struct Directory *cwd) {
    if (!cwd->parent) {
        printf("/");
        return;
    }
    printwd(cwd->parent);
    printf("%s/", cwd->name);
}

void printtree(struct Directory *root, int level) {
    int i;
    for (i = 0; i < level-1; ++i)
        printf("│   ");
    if (i == level-1)
        printf("└── ");
    printf("%s (%d)\n", root->name, root->size_of_immediate_files);
    for (i = 0; i < root->subdir_count; ++i) {
        printtree(root->subdirectories[i], level+1);
    }
}

int CountSmallDirectories(struct Directory *root) {
    int size = root->size_of_immediate_files;
    int i;
    for (i = 0; i< root->subdir_count; ++i) {
        size += CountSmallDirectories(root->subdirectories[i]);
    }
    printf("folder '"); printwd(root); printf("' has total size %d\n", size);
    if (size <= 100000) PUZZLE1ANSWER += size;
    return size;
}

int FindMinimalDeletable(struct Directory *root, int to_free) {
    int size = root->size_of_immediate_files;
    int i;
    for (i = 0; i< root->subdir_count; ++i) {
        size += FindMinimalDeletable(root->subdirectories[i], to_free);
    }
    printf("folder '"); printwd(root); printf("' has total size %d; to beat is %d\n", size, PUZZLE2ANSWER);
    if (size >= to_free && size < PUZZLE2ANSWER) PUZZLE2ANSWER = size;
    return size;
}

int main() {
    char buff[BUFFSZ];
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;
    struct Directory *root = CreateDirectory(NULL, "/");
    struct Directory *cwd = root;
    char c;
    int i;
    size_t len;
    while (fgets(buff, BUFFSZ, file)) {
        len = strnlen(buff, BUFFSZ);
        buff[len-1] = '\0';
        printf(">>>>> %s\n", buff);
        switch (buff[0]) {
            case '$':
                if (buff[2] == 'c') {
                    ChangeDirectory(root, &cwd, buff+5);
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                i = 0;
                cwd->size_of_immediate_files += conv_next_int_until_char(buff, ' ', &i);
                break;
            case 'd':
                CreateDirectory(cwd, buff+4);
                break;
            default:
                fprintf(stderr, "UNEXPECTED: c is %c\n", c);
        }
        printf("cwd is "); printwd(cwd); printf("\n");
        printtree(root, 0);
    }

    int rootsz = CountSmallDirectories(root);
    
    printf("sum of small directories is %d\n", PUZZLE1ANSWER);

    int to_free = rootsz - FSCAP + UPDATESZ;
    FindMinimalDeletable(root, to_free);

    printf("minimal deletable is of size %d\n", PUZZLE2ANSWER);
}
