// David Levi
// Operating Systems
// HW 4 -- Wordsearch


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
#include <signal.h>

int count=0;

void handler(){
	fprintf(stderr,"MATCHED %d WORD(S)\n",count);
	exit(0);
}

int main(int argc, char** argv){
	char* line;
	size_t n = 0;
	// Had to make dictionary shorter because it was way too long
	char* dict = "/Users/davidlevi/documents/homework/programming/operating_systems/hw4/shorterdict.txt"; 
	signal(SIGPIPE,handler);

	while(getline(&line,&n,stdin)!=-1){// Reading from stdin
		
		FILE* fp2;
		if((fp2 = fopen(dict,"r"))<0){
			fprintf(stderr,"Error opening %s: %s\n",dict,strerror(errno));
			exit(-1);
		}
		
		char* line2 = NULL;
		size_t n2 = 0;
		int check1 = 0;

		while(getline(&line2,&n2,fp2)!=-1){ // Reading from dictionary
			if(strcmp(line,line2)==0){
				printf("%s",line2);
				count++;
			}	
		}
		free(line2);
		fclose(fp2);
	}
	free(line);
	fprintf(stderr,"MATCHED %d WORD(S)\n",count);
	return 0;
}
