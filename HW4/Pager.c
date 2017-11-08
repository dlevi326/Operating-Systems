// David Levi
// Operating Systems
// HW 4 -- Pager

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
	char* line = NULL;
	size_t n=0;
	int count = 0;
	
	while(getline(&line,&n,stdin)&&count<24){
		count++;
		if(count==23){
			printf("--Press RETURN for more--\n");
			FILE* ttyfp = fopen("/dev/tty", "r");
			char* line2 = NULL;
			size_t n2=0;
			while(getline(&line2,&n2,ttyfp)){
				if(strcmp(line2,"q")==0||strcmp(line2,"Q")==0){
					printf("YOU JUST PRESS Q:::::\n");
					exit(1);
				}
				else if(strcmp(line2,"\n")==0){
					count = -1;
					break;
				}
			}
  			
		}
		
		printf("%s",line);

	}
	







	return 0;
}