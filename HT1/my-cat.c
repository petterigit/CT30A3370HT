/* 	my-cat.c
	Suorittaa cat-toiminnot monella eri tiedostolla 
	sataan merkkin / rivi asti. Tehtävänannosta poiketen,
	tiedostonluku virheen yhteydessä tulostetaan myös itse
	virheen nimi, käyttäen perror() -funktiota.

	Petteri Mäkelä
	15/12/18
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	char buffer [100];
	for (int i = 1; i<argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
	    	perror("my-cat: cannot open fp\n");
	   		exit(-1);
	   	}
	   	while(fgets(buffer, 100, fp)) {
    		printf("%s\n", buffer);
		}
	   	fclose(fp);
	}
	return(0);
}
