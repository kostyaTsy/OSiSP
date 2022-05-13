#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <string.h>

#define CHILD_COUNT 2

int pids[CHILD_COUNT];
int childInd = 1;
int msgNumber;


// Getting current time
void getCurrTime() {
    struct timeval te;
    errno = 0;
    gettimeofday(&te, NULL);
    if (errno != 0) {
        perror("gettimeofday");
    }

    long msec = te.tv_usec / 1000;
    long sec = te.tv_sec % 60;
    long min = (te.tv_sec / 60) % 60;
    long hours = (te.tv_sec / 3600 + 3) % 24;

    printf("Time is: %02ld:%02ld:%02ld:%03ld ", hours, min, sec, msec);
}

void getInfo(int ind, char *person, int sigInd, int childPid) {
    printf("%3d PID:%5d PPID:%5d ", msgNumber, getpid(), getppid());
    msgNumber++;
    getCurrTime();
    if (strcmp(person, "child")) {
        printf(" CHILD%d get SIGUSR%d\n", ind, sigInd);
    }
    else {
        printf(" PARENT get SIGUSR%d from %d\n", sigInd, childPid);
    }
}

int getChildInd(pid_t pid) {
    for (int i = 0; i < CHILD_COUNT; i++) {
        if (pids[i] == pid) {
            return i;
        }
    }
    return -1;
}

void parentSigAct(int sig, siginfo_t *sginf, void *_) {
    int chInd = getChildInd(sginf->si_pid);
    getInfo(chInd, "child", 2, sginf->si_pid);

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1e8;

    errno = 0;
    nanosleep(&ts, NULL);
    if (errno != 0) {
        perror("nanosleep");
        return;
    }

    kill(0, SIGUSR1);

}

void childSigAct(int sig) {
    getInfo(childInd, "parent", 1, 0);
    kill(getppid(), SIGUSR2);
}

int main() {

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = parentSigAct;
    int err_code = sigaction(SIGUSR2, &sa, NULL);

    if (err_code == -1) {
        perror("sigaction");
    }

    signal(SIGUSR1, SIG_IGN);

    pid_t pid = 1;
    for (int i = 0; i < CHILD_COUNT && pid > 0; i++) {
        switch (pid = fork()) {
            case 0:
                printf("Child %d, my pid is %d, my ppid is %d\n", childInd, getpid(), getppid());

                signal(SIGUSR1, childSigAct);
                break;
            case -1:
                perror("fork");
                exit(-1);
            default:
                pids[i] = pid;
                childInd++;
                break;
        }
    }

    if (pid > 0) {
        sleep(1);
        kill(0, SIGUSR1);
    }

    while (1);
}
