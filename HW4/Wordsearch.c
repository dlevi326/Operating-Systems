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


int main(int argc, char** argv){

	char* line;
	size_t n = 0;
	FILE* fp;
	fp = stdin;
	int count=0;
	char* dict = argv[1];

	
	while(getline(&line,&n,fp)!=-1){
		
		FILE* fp2;
		fp2 = fopen(dict,"r");
		if(!fp2){
			fprintf(stderr,"ERROR\n");
			break;
		}
		
		char* line2 = NULL;
		size_t n2 = 0;
		int check1 = 0;


		while(getline(&line2,&n2,fp2)!=-1){
			
			if(strcmp(line,line2)==0){
				count++;
			}

			
		}
		free(line2);
		fclose(fp2);

	}
	free(line);
	fclose(fp);

	fprintf(stderr,"MATCHED %d WORD(S)\n",count);






	return 0;
}