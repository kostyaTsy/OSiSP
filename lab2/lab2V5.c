#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// lab var 5

// Going through the files and calculating size of subdirectory ...
// cntOfFiles - amount of files in subdirectory
// maxSizeFile - file with max size in subdirectory
// FILE - file to write output text
// Returning the size of full directory
long long GoThroughFiles(char dirName[1000], int *cntOfFiles, FILE *file) {
    DIR *currDir;
    struct dirent *dp;
    struct stat buf;
    // Name of directory to going recursively
    char dirDown[1000];
    int cntFilesInDir = 0;

    // Name of file with max size in the current directory
    char tmpMaxFileSize[256];
    strcpy(tmpMaxFileSize, "");

    long long sizeOfDir = 0;
    long long currDirSize = 0;

    // Trying to open the directory
    if ((currDir = opendir(dirName)) == NULL) {
        perror("Couldn't open the directory");
        return sizeOfDir;
    }

    long currMaxSize = 0;
    while ((dp = readdir(currDir)) != NULL) {
        // Checking for directory
        if (dp->d_type == DT_DIR && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy(dirDown, dirName);
            strcat(dirDown, "/");
            strcat(dirDown, dp->d_name);
            sizeOfDir += GoThroughFiles(dirDown, cntOfFiles, file);
        }
        // Checking for simple files
        else if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            // Get full path to file
            cntFilesInDir++;
            char filePath[1000] = "";
            strcpy(filePath, dirName);
            strcat(filePath, "/");
            strcat(filePath, dp->d_name);

            // Finding the size of file
            if (stat(filePath, &buf) == 0) {
                sizeOfDir += buf.st_size;
                currDirSize += buf.st_size;

                // Finding file with max size
                if (currMaxSize < buf.st_size) {
                    currMaxSize = buf.st_size;
                    strcpy(tmpMaxFileSize, dp->d_name);
                }
            }
        }
    }

    // Counting all files in parent directory
    *cntOfFiles += cntFilesInDir;
    printf("%s:     %d %lld %s\n", dirName, cntFilesInDir, currDirSize, tmpMaxFileSize);
    fprintf(file, "%s:     %d %lld %s\n", dirName, cntFilesInDir, currDirSize, tmpMaxFileSize);

    if (closedir(currDir) == -1) {
        perror("Couldn't close directory");
    }

    return sizeOfDir;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("First argument - name of directory\nSecond argument - output file");
        return 0;
    }

    char fileName[100] = "out.txt";
    int fileHandler = open(argv[2], O_WRONLY | O_CREAT, 438);
    if (fileHandler < 0) {
        perror("Couldn't open the out file");
        return 0;
    }

    // Open file to write output
    FILE *file = fdopen(fileHandler, "w");

    int cnt = 0;
    long long size = GoThroughFiles(argv[1], &cnt, file);
    printf("Size: %lld,\nAmount of files: %d\n", size, cnt);
    return 0;
}
