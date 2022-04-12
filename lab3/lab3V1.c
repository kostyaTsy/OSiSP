#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <string.h>
#include <sys/wait.h>

// Counting factorial of num
unsigned long long int factorial(int num) {
    unsigned long long int res = 1;
    for (int i = 1; i <= num; i++) {
        res *= i;
    }
    return res;
}

// Power a to n
double myPow(double a, int n) {
    double res = 1;
    for (int i = 0; i < n; i++) {
        res *= a;
    }

    return res;
}

// Convert string to long
long StrToNum(char *str) {
    char *trash;
    errno = 0;
    long N = strtol(str, &trash, 10);

    if ((errno == ERANGE) || (errno != 0 && N == 0)) {
        perror("strtol");
        exit(-1);
    }

    if (trash == str) {
        fprintf(stderr, "No digits");
        exit(-1);
    }

    return N;
}

// Opening file
FILE* openFile(char *path, int oflag, char *modes) {
    int fileHandler = open(path, oflag|O_CREAT, 0644);
    if (fileHandler < 0) {
        perror("open");
        exit(-1);
    }

    errno = 0;
    FILE *file = fdopen(fileHandler, modes);
    if (errno != 0) {
        perror("Couldn't open file");
        exit(-1);
    }

    return file;
}

// Closing file
void closeFile(FILE *file) {
    if (fclose(file) != 0) {
        perror("Couldn't close file");
        exit(-1);
    }
}

// Finding term of Teylor's series (y[i])
double findTerm(FILE *file) {
    char str[60];
    double res = 0;
    while (fgets(str, 50, file) != NULL) {

        str[strlen(str)-1] = '\0';
        // split string by white space
        char *del = " ";
        char *tok = strtok(str, del);
        char *strNum;
        while (tok != NULL) {
            strNum = strtok(NULL, del);
            tok = strtok(NULL, del);
        }

        char *trash;
        double num = strtod(strNum, &trash);

        // Checking for correct convertation
        if (errno == ERANGE && (errno != 0 && num == 0)) {
            perror("strtof");
            exit(-1);
        }
        if (trash == strNum) {
            fprintf(stderr, "No digits");
        }

        res += num;
    }

    return res;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "First argument is N\nSecond argument is number of terms in expansion");
        return -1;
    }

    // Checking for correct numbers
    long N = StrToNum(argv[1]);
    long terms_num = StrToNum(argv[2]);

    // Checking if number is positive
    if (N <= 0 || terms_num <= 0) {
        fprintf(stderr, "N or n small numbers");
        return -1;
    }

    char *fName = "tmp.txt";

    // Opening out file with counted values of expression
    FILE *outFile = fopen("out.txt", "w");

    // Allocating a double array
    double *resArr = (double*)calloc(N, sizeof(double));
    // Start calculating expression
    for (int i = 0; i < N; i++) {
        // Open tmp file to write terms num
        FILE *fileWrite = openFile(fName,O_WRONLY,"w");
        errno = 0;
        // Rewrite the file
        freopen(NULL, "w+", fileWrite);
        // Checking for errors in freopen
        if (errno != 0) {
            perror("freopen");
            exit(-1);
        }
        closeFile(fileWrite);

        // Finding terms number in Teylor's series
        printf("%d:\n", i);
        for (int k = 0; k < terms_num; k++) {
            double numerator= 0, tmp_num = 0;
            unsigned long long int denominator = 0;

            // Creating a process to calculate term num of series
            pid_t child = fork();
            switch (child) {
                case 0:
                    fileWrite = openFile(fName, O_WRONLY | O_APPEND, "a");

                    numerator = myPow(((2*M_PI*i)/(double)N), (2*k+1));
                    denominator = factorial(2*k+1);
                    tmp_num = myPow(-1.0, k) * numerator / (double)denominator;

                    printf("%d %f\n", getpid(), tmp_num);
                    fprintf(fileWrite, "%d %f\n", getpid(), tmp_num);

                    closeFile(fileWrite);
                    return 0;
                case -1:
                    perror("Fork error");
                    return -1;
                default:
                    break;
            }
        }

        // Wait to complete all child
        while (1) {
            if (wait(NULL) == -1) {
                if (errno == ECHILD)
                    break;
            }
        }

        // Open file to parent fork could read
        FILE *fileRead = openFile(fName,O_RDONLY, "r");
        double num = findTerm(fileRead);
        resArr[i] = num;

        closeFile(fileRead);
    }

    for (int i = 0; i < N; i++) {
        fprintf(outFile, "y[%d] = %f\n", i, resArr[i]);
    }
    closeFile(outFile);
}