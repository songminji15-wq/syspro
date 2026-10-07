#include <stdio.h>
struct student {
    int id;
    char name[20];
    short score;
};

int main(int argc, char* argv[])
{
    struct student rec;
    FILE *fp;
    if (argc != 2) {
        fprintf(stderr, "How to use: %s FileName\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "w");
    printf("%-9s %-7s %-4s\n", "StudentID", "Name", "Score");
    while (scanf("%d %s %hd", &rec.id, rec.name, &rec.score)==3)
        fprintf(fp, "%d %s %d ", rec.id, rec.name, rec.score);
    fclose(fp);
    return 0;
}
