#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        perror("First argument - name of source file\nSecond argument - name of destination file");
        return 0;
    }

    // Checking first source file
    int fileHandlerSrc = open(argv[1], O_RDONLY);
    if (fileHandlerSrc < 0) {
        perror("Couldn't open source file");
        return 0;
    }

    // struct to get info about file mode
    struct stat buf;
    if (stat(argv[1], &buf) == -1) {
        perror("Error of getting stat of source file");
        return 0;
    }

    // Checking for destination file
    int fileHandlerDest = open(argv[2], O_WRONLY | O_CREAT, buf.st_mode);
    if (fileHandlerDest < 0) {
        perror("Couldn't open destination file");
        return 0;
    }

    if (chmod(argv[2], buf.st_mode) != 0) {
        perror("Error of setting mode to destination file");
    }

    //Opening source and dest files
    FILE *srcFile = fdopen(fileHandlerSrc, "r");
    FILE *destFile = fdopen(fileHandlerDest, "w");

    // Trying to copy data from src to dest file
    int chr;
    while ((chr = fgetc(srcFile)) != EOF) {
        if (fputc(chr, destFile) == EOF) {
            perror("Error of writing to file");
        }
    }

    // Closing files
    if (fclose(srcFile) != 0) {
        perror("Couldn't close the source file");
    }

    if (fclose(destFile) != 0) {
        perror("Couldn't close the destination file");
    }

    printf("All done\n");
    return 0;
}
