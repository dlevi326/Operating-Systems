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
    int numread;
    FILE* fp;
    int c;
	
	while((numread = getline(&line,&n,stdin))&&count<24){
        if(numread==-1){
            fprintf(stderr,"End of file, exiting pager\n");
            exit(0);
        }
		count++;
		line[strlen(line)-1]='\0';
		printf("%s\n",line);
		if(count==23){
            printf("--Press RETURN for more--");
    		if((fp = fopen("/dev/tty", "w+"))<0){
                fprintf(stderr,"Error opening /dev/tty: %s\n",strerror(errno));
            }
            c = getc(fp);
            fclose(fp);

    		if(c<0){
    			fprintf(stderr, "Error reading from terminal, exiting program\n");
    			exit(-1);
    		}
    		else if(c==81||c==113||c==0){ 
    			fprintf(stderr,"Exiting pager\n");
    			exit(0);
    		}
    		else{
    			count = 0;
    		}	
		}
	}
    free(line);

	return 0;
}