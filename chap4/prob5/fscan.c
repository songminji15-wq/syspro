#include <stdio.h>
struct student {
    int id;
    char name[20];
    short score;
};

int main(int argc, char* argv[]) {
    struct student rec;
    FILE *fp;
    if (argc != 2) {
        fprintf(stderr, "How to use: %s FileName\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "r");
    printf("%-9s %-7s %-4s\n", "StudentID", "Name", "Score");
    while (fscanf(fp,"%d %s %hd", &rec.id, rec.name, &rec.score)==3)
        printf("%10d %6s %6d\n", rec.id, rec.name, rec.score);
    fclose(fp);
    return 0;
}
