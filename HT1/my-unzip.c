/*  my-unzip.c

    Petteri Mäkelä
    15/12/18
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    char buffer[5];

    if (argc==1) {
        printf("my-unzip: file1 file2 ...\n");
        return(1);
    }

    for (int i = 1; i<argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("my-unzip: cannot open file\n");
            exit(-1);
        }
        while((fread(buffer, 5, 1, fp))) {
            for(int i = 0; i<*(int*)buffer; i++) {
                printf("%c", buffer[4]);
            }
        }
        fclose(fp);
    }
    printf("\n");
    return(0);
}
