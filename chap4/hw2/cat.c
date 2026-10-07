#include <unistd.h>
#include <fcntl.h>

void put_num(int n) {
    char buf[32];
    int i = 0;
    if (n == 0) {
        write(1, "0", 1);
        return;
    }
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    while (--i >= 0) {
        write(1, &buf[i], 1);
    }
}

void cat_file(int fd, int n_flag, int *line_num, int *new_line) {
    char c;
    while (read(fd, &c, 1) > 0) {
        if (n_flag && *new_line) {
            put_num((*line_num)++);
            write(1, " ", 1);
            *new_line = 0;
        }
        write(1, &c, 1);
        if (c == '\n') {
            *new_line = 1;
        }
    }
}

int main(int argc, char *argv[]) {
    int start = 1;
    int n_flag = 0;
    int line_num = 1;
    int new_line = 1;
    int fd, i;

    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n') {
        n_flag = 1;
        start = 2;
    }

    if (start >= argc) {
        cat_file(0, n_flag, &line_num, &new_line);
    } else {
        for (i = start; i < argc; i++) {
            fd = open(argv[i], O_RDONLY);
            if (fd >= 0) {
                cat_file(fd, n_flag, &line_num, &new_line);
                close(fd);
            }
        }
    }
    return 0;
}
