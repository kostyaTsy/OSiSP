#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int lineSize = 500;

// Reading str from file
int getFileStr(FILE *file, char *str) {
    int chr = fgetc(file);
    while ( chr != EOF && chr != '\n') {
        str[strlen(str)] = (char)chr;
        chr = fgetc(file);
    }

    if (chr == '\n')
        str[strlen(str)] = (char)chr;
    if (chr == EOF)
        return 0;
    else
        return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("First argument - should be a file,\nSecond argument should be a number (amount of output row per tap, 0 - is a plain text)");
        return 0;
    }

    char *ptr;
    long inNum = strtol(argv[2], &ptr, 10);
    if (inNum < 0) {
        perror("Second argument less than 0");
        return 0;
    }
    if (strcmp(argv[2], ptr) == 0 || strlen(ptr) != 0) {
        perror("Not a number");
        return 0;
    }

    int fileHandler = open(argv[1], O_RDONLY);
    if (fileHandler < 0) {
        perror("Couldn't open the file");
        return 0;
    }

    FILE *file = fdopen(fileHandler, "r");

    int cntLine = 0;
    char *str = (char*) calloc(lineSize, sizeof(char));
    while (getFileStr(file, str)) {
        printf("%s", str);
        cntLine++;
        if (cntLine == inNum && inNum != 0) {
            getc(stdin);
            cntLine = 0;
        }
        str = (char*) calloc(lineSize, sizeof(char));
    }

    if (fclose(file) != 0) {
        perror("Couldn't close the file");
        return 0;
    }
    return 0;
}
