#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

int opt_i = 0, opt_p = 0, opt_Q = 0;

char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);

int main(int argc, char **argv)
{
    DIR *dp;
    char *dir;
    struct stat st;
    struct dirent *d;
    char path[BUFSIZ+1];
    int opt;

    while ((opt = getopt(argc, argv, "ipQ")) != -1) {
        switch (opt) {
            case 'i':
                opt_i = 1;
                break;
            case 'p':
                opt_p = 1;
                break;
            case 'Q':
                opt_Q = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-i] [-p] [-Q] [directory]\n", argv[0]);
                exit(1);
        }
    }

    if (optind >= argc)
        dir = ".";
    else
        dir = argv[optind];

    if ((dp = opendir(dir)) == NULL) {
        perror(dir);
        exit(1);
    }

    while ((d = readdir(dp)) != NULL) {
        sprintf(path, "%s/%s", dir, d->d_name);
        if (lstat(path, &st) < 0) {
            perror(path);
            continue;
        }
        printStat(path, d->d_name, &st);
    }

    closedir(dp);
    exit(0);
}

void printStat(char *pathname, char *file, struct stat *st)
{
    char filename[BUFSIZ];
    strcpy(filename, file);

    if (opt_p && S_ISDIR(st->st_mode)) {
        strcat(filename, "/");
    }
	if (opt_Q) {
        char temp[BUFSIZ + 10];
        snprintf(temp, sizeof(temp), "\"%s\"", filename);
        strcpy(filename, temp);
    }
    if (opt_i) {
        printf("%lu ", (unsigned long)st->st_ino);
    }

    printf("%5ld ", (long)st->st_blocks);
    printf("%c%s ", type(st->st_mode), perm(st->st_mode));
    printf("%3lu ", (unsigned long)st->st_nlink);
    printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
    printf("%9ld ", (long)st->st_size);
    printf("%.12s ", ctime(&st->st_mtime)+4);
    printf("%s\n", filename);
}

char type(mode_t mode)
{
    if (S_ISREG(mode)) return('-');
    if (S_ISDIR(mode)) return('d');
    if (S_ISCHR(mode)) return('c');
    if (S_ISBLK(mode)) return('b');
    if (S_ISLNK(mode)) return('l');
    if (S_ISFIFO(mode)) return('p');
    if (S_ISSOCK(mode)) return('s');
    return('?');
}

char* perm(mode_t mode)
{
    static char perms[10];
    strcpy(perms, "---------");

    for (int i=0; i < 3; i++) {
        if (mode & (S_IRUSR >> (i*3))) perms[i*3] = 'r';
        if (mode & (S_IWUSR >> (i*3))) perms[i*3+1] = 'w';
        if (mode & (S_IXUSR >> (i*3))) perms[i*3+2] = 'x';
    }
    return(perms);
}
