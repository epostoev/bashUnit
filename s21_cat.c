#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include "CatFlags.h"

typedef struct {
    bool numberNonBlank;
    bool markEndl;
    bool numberAll;
    bool squeeze;
    bool tab;
    bool printNonPrintable;
} Flags;

Flags CatReadFlags(int argc, char *argv[]) {
    struct option longOptions[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    int currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
    Flags flags = {false, false, false, false, false, false};
    for (; currentFlag != -1
         ; currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) {
        switch (currentFlag) {
            case 'b': flags.numberNonBlank = true; break;
            case 'e': flags.markEndl = true;
            case 'v': flags.printNonPrintable = true; break;
            case 'E': flags.markEndl = true; break;
            case 'n': flags.numberAll = true; break;
            case 's': flags.squeeze = true; break;
            case 't': flags.printNonPrintable = true;
            case 'T': flags.tab = true; break;
        }
    }
    return flags;
}

void CatFile(FILE *file, Flags flags, const char *table[256]) {
    int c = 0;
    int lineno = 0;
    int last;
    last = '\n';
    (void)flags;
    while ((c = fgetc(file)) != EOF) {
        if (last == '\n') {
            if (flags.squeeze && c == '\n')
                continue;
        
            if (flags.numberAll) {
                printf("%6i  ", ++lineno);
            }
            else if (flags.numberNonBlank) {
                if (c != '\n')
                    printf("%6i  ", ++lineno);
            }
        }
            printf("%s", table[c]);
            last = c;
        // else {
        //     putchar(c); // Если нет преобразования, выводим символ
        // }
    }
}

void Cat (int arge, char *argv[], Flags flags, const char *table[static 256]) {
    for (char **filename = &argv[1], **end = &argv[arge]; filename != end; ++filename) {
        if (**filename == '-')
        continue;
    FILE *file = fopen(*filename, "rb");
        if (errno) {
            fprintf(stderr, "%s", argv[0]);
            perror(filename);
            continue;
        }
        CatFile(file, flags, table);
        fclose(file);
    }
}

int main(int argc, char *argv[]) {
    Flags flags = CatReadFlags(argc, argv);
    const char *table[256] = {0};
    CatSetTable(table);

    if (flags.markEndl) {
        CatSetEndl(table);
    }
    if (flags.numberAll) {
        printf ("number All Lines\n");
    }
    if (flags.numberNonBlank) {
        printf ("number Non Blank\n");
    }
    if (flags.tab) {
        CatSetTab(table);
    }
    if (flags.printNonPrintable)
        CatSetNonPrintable(table);
    if (flags.squeeze)
        printf("squeeze\n");
    Cat(argc, argv, flags, table);
    return 0;
}
