#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARG 64

int main()
{
    char line[MAXLINE];
    char *args[MAXARG];
    char *saveptr;
    char *token;
    int pid, child, status;
    int background;

    while (1) {
        printf("Pls input cmd : ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        int i = 0;
        background = 0;
        token = strtok_r(line, " \t\n", &saveptr);
        while (token != NULL && i < MAXARG - 1) {
            if (strcmp(token, "&") == 0) {
                background = 1;
            } else {
                args[i++] = token;
            }
            token = strtok_r(NULL, " \t\n", &saveptr);
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        printf("[%d] Parent process start\n", getpid());

        pid = fork();
        if (pid < 0) {
            perror("fork error");
            exit(1);
        } else if (pid == 0) {
            if (background) {
                printf("[%d] child process start\n", getpid());
            }
            execvp(args[0], args);
            exit(1);
        } else {
            if (background) {
                child = waitpid(pid, &status, 0);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    printf("[%d] child process end %d\n", getpid(), child);
                    printf("SUCCESS\n");
                } else {
                    printf("Parent process end\n");
                    printf("Exit\n");
                    break;
                }
            } else {
                child = wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    printf("SUCCESS\n");
                } else {
                    printf("Parent process end\n");
                    printf("Exit\n");
                    break;
                }
            }
        }
    }
    return 0;
}
