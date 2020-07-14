/*  my-zip.c

	Program to zip a file.

    Petteri Mäkelä
    15/12/18
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int j = 0;
    uint32_t count = 1;
    int count_write = 4;
    char buffer [4096];
    char write_this[4096];

    if (argc==1) {
		printf("my-zip: file1 [file2 ...]\n");
		return(1);
	}

    for (int i = 1; i<argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("my-zip: cannot open file\n");
            exit(-1);
        }
        while(fgets(buffer, sizeof(buffer), fp)) {
            while(buffer[j] != '\0') {
                if (buffer[j+1] == buffer[j]) {
                    count++;
                } else {
                    *((int*)(write_this+count_write - 4)) = count;
                    write_this[count_write] = buffer[j];
                    count_write = count_write + 5;
                    count = 1;
                }
                j++;

            }
            j = 0;

        }
        fclose(fp);
    }
    write(1,write_this,count_write);
}
