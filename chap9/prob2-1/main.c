#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int pid, child, status;
    printf("[%d] parental process strat\n", getpid());
    pid = fork();
    if (pid == 0) {
        printf("[%d] child process strat\n", getpid());
        exit(1);
    }
    child = wait(&status);
    printf("[%d] child process %d end \n", getpid(), child);
    printf("end code %d\n", status>>8);
}
