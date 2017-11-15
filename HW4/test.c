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

/*close(pipefd1[0]);
close(pipefd1[1]);
close(pipefd2[0]);
close(pipefd2[1]);*/




int main(int argc, char** argv){

	char* comm1 = "/Users/davidlevi/documents/homework/programming/operating_systems/hw4/Wordgen.exe";
	char* comm2 = "/Users/davidlevi/documents/homework/programming/operating_systems/hw4/Wordsearch.exe";
	char* comm3 = "/Users/davidlevi/documents/homework/programming/operating_systems/hw4/Pager.exe";

	//execvp(comm1,argv);
	char** args = argv;
	
	
	
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

	pipe(pipefd1);
	pipe(pipefd2);

	int pid1,pid2,pid3;

	switch(pid1 = fork()){
		case -1:
			fprintf(stderr,"Error forking program\n");
			break;
		case 0:
			close(pipefd1[0]);
			//close(pipefd1[1]);
			close(pipefd2[0]);
			close(pipefd2[1]);

			dup2(pipefd1[1],1);
			execvp(comm1,argv);
			close(pipefd1[1]);
			
			exit(1);
			break;
		default:
			pchild1 = wait3(&status1,0,&ru1);

			switch(pid2=fork()){
				case 0:
					//close(pipefd1[0]);
					close(pipefd1[1]);
					close(pipefd2[0]);
					close(pipefd2[1]);

					dup2(pipefd1[0],0);
					execvp(comm3,argv);
					close(pipefd1[0]);
			
					//exit(1);
					break;
				default:
					pchild2 = wait3(&status2,0,&ru2);
					printf("DONE!");
			}
	}


	/*switch(fork()){
		case 0:
			//close(pipefd1[0]);
			close(pipefd1[1]);
			close(pipefd2[0]);
			//close(pipefd2[1]);
			dup2(pipefd1[0],0);
			dup2(pipefd2[1],1);
			execvp(comm2,argv);
			close(pipefd1[0]);
			close(pipefd2[1]);
			exit(1);
			break;
	}

	switch(fork()){
		case 0:
			close(pipefd1[0]);
			close(pipefd1[1]);
			//close(pipefd2[0]);
			close(pipefd2[1]);
			dup2(pipefd2[0],0);
			execvp(comm3,argv);
			close(pipefd2[0]);
			exit(1);
			break;
	}*/



		/*default:
			pchild1 = wait3(&status1,0,&ru1);

			switch(fork()){
				case 0:
					//close(pipefd1[0]);
					close(pipefd1[1]);
					close(pipefd2[0]);
					close(pipefd2[1]);
					dup2(pipefd1[0],0);
					execvp(comm2,argv);
					close(pipefd1[0]);
					exit(1);
					break;
				default:
					//pchild2 = wait3(&status2,0,&ru2);
					printf("********\n");
					if(wait(&status2)==-1){
						fprintf(stderr,"Error waiting\n");
					}
					printf("This is the parent\n");

			}*/

			//close(pipefd1[0]);
			/*close(pipefd1[1]);
			close(pipefd2[0]);
			close(pipefd2[1]);
			dup2(pipefd1[0],0);
			execvp(comm2,argv);
			close(pipefd1[0]);
			exit(1);*/

			

	

	




	return 0;
}
