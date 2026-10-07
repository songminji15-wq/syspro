#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp1, *fp2;
    char buffer[1024];
    size_t bytesRead;

    if (argc != 3) {
        return 1;
    }

    fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        return 1;
    }

    fp2 = fopen(argv[2], "a");
    if (fp2 == NULL) {
        fclose(fp1);
        return 1;
    }

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp1)) > 0) {
        fwrite(buffer, 1, bytesRead, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}
