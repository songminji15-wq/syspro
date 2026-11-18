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
    printf("parent process end\n");
}
