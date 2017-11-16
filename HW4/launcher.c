// David Levi
// Operating Systems
// HW 4 -- Launcher

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
#include <setjmp.h>



int main(int argc, char** argv){
	
	char* comm1 = "./wordgen.exe";
	char* comm2 = "./wordsearch.exe";
	char* comm3 = "./pager.exe";	
	
	
	int pipefd1[2];
	int pipefd2[2];

	int status1;
	struct rusage ru1;
	int pchild1;

	int status2;
	struct rusage ru2;
	int pchild2;

	int status3;
	struct rusage ru3;
	int pchild3;

	if(pipe(pipefd1)<0){
		fprintf(stderr,"Error piping pipe 1\n");
	}
	if(pipe(pipefd2)<0){
		fprintf(stderr,"Error piping pipe 2\n");
	}

	int pid1,pid2,pid3;
	
	if((pid1=fork())==0){
		if(dup2(pipefd1[1],1)==-1){
			fprintf(stderr,"Error duping during execution of %s\n",comm1);
		}
		close(pipefd1[0]);
		close(pipefd1[1]);
		close(pipefd2[0]);
		close(pipefd2[1]);
		if(execvp(comm1,argv)<0){
			fprintf(stderr,"Error executing %s\n",comm1);
		}
	}
	else if(pid1<0){
		fprintf(stderr,"Error forking\n");
	}
	else{
		if((pid2=fork())==0){

			if(dup2(pipefd1[0],0)<0){
				fprintf(stderr,"Error duping during execution of %s\n",comm2);
			}
			if(dup2(pipefd2[1],1)<0){
				fprintf(stderr,"Error duping during execution of %s\n",comm2);
			}
			close(pipefd1[0]);
			close(pipefd1[1]);
			close(pipefd2[0]);
			close(pipefd2[1]);
			if(execvp(comm2,argv)<0){
				fprintf(stderr,"Error executing %s\n",comm2);
			}
		}
		else if(pid2==-1){
			fprintf(stderr,"Error forking\n");
		}
		else{
			
			if((pid3=fork())==0){
				if(dup2(pipefd2[0],0)<0){
					fprintf(stderr,"Error duping during execution of %s\n",comm3);
				}
				close(pipefd1[0]);
				close(pipefd1[1]);
				close(pipefd2[0]);
				close(pipefd2[1]);
				if(execvp(comm3,argv)<0){
					fprintf(stderr,"Error executing %s\n",comm3);
				}
			}
			else if(pid3<0){
				fprintf(stderr,"Error forking\n");
			}
		}
		
	}
	close(pipefd1[0]);
	close(pipefd1[1]);
	close(pipefd2[0]);
	close(pipefd2[1]);

	if((pchild1 = wait3(&status1,0,&ru1))<0){
		fprintf(stderr, "Error waiting\n");
	}
	printf("Child %d return with %d\n",pchild1,status1);
	if((pchild2 = wait3(&status2,0,&ru2))<0){
		fprintf(stderr, "Error waiting\n");
	}
	printf("Child %d return with %d\n",pchild2,status2);
	if((pchild3 = wait3(&status3,0,&ru3))<0){
		fprintf(stderr, "Error waiting\n");
	}
	printf("Child %d return with %d\n",pchild3,status3);





	return 0;
}
