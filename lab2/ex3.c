#include <stdio.h>
#include <fcntl.h>

const int EXITCODE = 92; // Code of '\'
int main(int argc, char *argv[]) {

    if (argc != 2) {
        perror("First argument should be a name of file");
        return 0;
    }

    // File handler if < 0 this is error
    int fileHandler = open(argv[1], O_WRONLY | O_CREAT, 438);
    if (fileHandler < 0) {
        perror("Couldn't open the file");
        return 0;
    }

    FILE *file = fdopen(fileHandler, "w");

    int chr;
    while ((chr = getc(stdin)) != EXITCODE) {
        if (putc(chr, file) == EOF) {
            perror("Couldn't write to file");
            return 0;
        }
    }

    if (fclose(file) != 0) {
        perror("Couldn't close a file");
    }
    printf("Data has been written\n");
    return 0;
}
