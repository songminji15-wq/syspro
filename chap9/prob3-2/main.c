#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("parent process start\n");
    if (fork() == 0) {
        execl("/bin/echo", "echo", "hello", NULL);
        fprintf(stderr, "First failed");
        exit(1);
    }

    if (fork() == 0) {
        execl("/bin/date", "date", NULL);
        fprintf(stderr, "Second failed");
        exit(2);
    }

    if (fork() == 0) {
        execl("/bin/ls", "ls", "-l", NULL);
        fprintf(stderr, "Third failed");
        exit(3);
    }
    printf("parent process end\n");
}
