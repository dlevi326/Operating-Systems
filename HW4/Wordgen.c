// David Levi
// Operating Systems
// HW 4 -- Wordgen

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

char* generateword(int nc){

	nc++;
	char* str = malloc(sizeof(char) * nc);
	const char charlist[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    nc--;
    int key;
    for (int i = 0; i < nc; i++) {
        key = rand() % (int) (sizeof (charlist) - 1);
        str[i] = charlist[key];
    }
    str[nc] = '\0';

    char* ans = str;
    free(str);
    
    return ans;
}


int main(int argc, char** argv){
	int nc = 6; // Standard word size
	int numwords = -1;

	if(argc!=1){
		if(argv[1]!=0){
			numwords = atoi(argv[1]);
		}
	}

	srand(time(0));
	int sizeword = nc;
	while(numwords!=0){

		sizeword = (rand()%nc)+3;
		printf("%s\n",generateword(sizeword));
		numwords--;
	}
	







	return 0;
}