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
		line[strlen(line)-1]='\0';
		printf("%s\n",line);
		if(count==23){
			char* byte = malloc(sizeof(char)*1);
    		write(1,"--Press RETURN for more--",25);
    		int fd = open("/dev/tty", O_RDWR);
    		ssize_t size = read(fd, byte, sizeof(byte));
    		close(fd);
    		byte[strlen(byte)-1] = '\0';

    		if(size==-1){
    			fprintf(stderr, "Error reading from terminal, exiting program\n");
    			exit(-1);
    		}
    		else if(strcmp(byte,"q")==0||strcmp(byte,"Q")==0||size==0){ // Size=0 indicates cntrl-D
    			fprintf(stderr,"EXITING\n");
    			exit(0);
    		}
    		else{
    			count = 0;
    		}	
		}
	}


	return 0;
}