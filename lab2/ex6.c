#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

void GetDir(char *dirPath){
    DIR *dir;
    struct dirent *currDir;

    if ((dir = opendir(dirPath)) == NULL) {
        perror("Couldn't open current directory");
        return;
    }

    while ((currDir = readdir(dir)) != NULL)
        printf("%s\n", currDir->d_name);

    if (closedir(dir) != 0)
        perror("Couldn't close current directory");

}

int main(int argc, char *argv[]) {
    char currDirPath[256];
    printf("Current directory: \n");
    if (getcwd(currDirPath, 256) == NULL) {
        perror("Couldn't get the current directory");
    }

    GetDir(currDirPath);

    printf("\n\n\n");

    printf("Root directory:\n");
    GetDir("/");
    return 0;
}
