/* 	my-grep.c
	Simple grep function.
	Uses getline() to get longer lines, than we'd want to buffer for.
	runs in the form of:
		sudo ./my-grep <substring_to_find> file1 file2

	

	Petteri Mäkelä
	0506167
	15/12/18

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *to_find = argv[1];

	if (argc==1) {
		printf("my-grep: searchterm [file ...]\n");
		return(1);
	}
	for (int i = 2; i<argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
	    	perror("my-grep: cannot open file\n");
	   		return(1);
	   	}
	   	while ((read = getline(&line, &len, fp)) != -1) {
	        if (strstr(line, to_find) != NULL) {
	        	printf("%s", line);
	        }
    	}
    	if (ferror(fp)) {
        	perror("Error with file");
    	}
    	free(line);
	   	fclose(fp);
	}

	if (argc==2) {
		printf("Reading from stdin\n");
	   	while ((read = getline(&line, &len, stdin)) != -1) {
	        if (strstr(line, to_find) != NULL) {
	        	printf("%s", line);
	        }
    	free(line);
    	}
	}
	return(0);
}
