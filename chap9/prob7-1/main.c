#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARGS 64

void execute_command(char *cmd) {
    char *args[MAXARGS];
    char *infile = NULL;
    char *outfile = NULL;
    int background = 0;
    int i = 0;

    char *token = strtok(cmd, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            background = 1;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) outfile = token;
        } else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) infile = token;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;

    if (args[0] == NULL) return;

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
    } else if (pid == 0) {
        if (infile != NULL) {
            int fd_in = open(infile, O_RDONLY);
            if (fd_in < 0) {
                perror("open infile error");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (outfile != NULL) {
            int fd_out = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
            if (fd_out < 0) {
                perror("open outfile error");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        execvp(args[0], args);
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(1);
    } else {
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            printf("[Background PID %d]\n", pid);
        }
    }
}

int main() {
    char line[MAXLINE];
    char *cmds[MAXARGS];

    while (1) {
        printf("[shell] ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        int cmd_count = 0;
        char *cmd = strtok(line, ";\n");
        while (cmd != NULL) {
            cmds[cmd_count++] = cmd;
            cmd = strtok(NULL, ";\n");
        }

        for (int i = 0; i < cmd_count; i++) {
            execute_command(cmds[i]);
        }
    }
    return 0;
}
