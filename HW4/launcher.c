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


int main(int argc, char** argv){
	
	

	int pipefd1[2];
	int pipefd2[2];
	// fd[0] for reading fd[1] for writing

	pipe(pipefd1);
	pipe(pipefd2);
	char buf;

	pid_t childpid;
	pid_t childpid2;
	struct rusage ru;
	int status;


	if((childpid = fork())==-1){ // Fork for executing wordgen
		fprintf(stderr,"Error forking program\n");
		exit(-1);
	}

	if(childpid!=0){


		if(dup2(pipefd1[0],STDIN_FILENO)==-1){
			fprintf(stderr,"Error duping\n");
		}
		close(pipefd1[1]);


		if(execvp("/Users/davidlevi/documents/homework/programming/operating_systems/hw4/wordgen.exe",argv)==-1){
			fprintf(stderr,"Error executing\n");
		}
	}
	else{
		if(dup2(pipefd1[1],STDOUT_FILENO)==-1){
			fprintf(stderr,"Error duping\n");
		}
		close(pipefd1[0]);
		if(execvp("/Users/davidlevi/documents/homework/programming/operating_systems/hw4/Wordsearch.exe",argv)==-1){
			fprintf(stderr,"Error executing\n");
		}
		
	}

	/*if(execvp("/Users/davidlevi/documents/homework/programming/operating_systems/hw4/wordgen.exe",argv)==-1){
		fprintf(stderr,"Error executing\n");
	}*/
	















	/*if(dup2(pipefd1[0],STDIN_FILENO)==-1){
		fprintf(stderr,"Error duping\n");
	}*/


	/*if(childpid==0){
		close(pipefd1[1]);
		close(pipefd2[0]);

		if(dup2(pipefd1[0],STDIN_FILENO)==-1){
			fprintf(stderr,"Error duping\n");
		}
		if(dup2(pipefd2[1],STDOUT_FILENO)==-1){
			fprintf(stderr,"Error duping\n");
		}

		close(pipefd1[0]);
		close(pipefd2[1]);
		char** args;
		if(argc>1){
			args = malloc(sizeof(char)*2);
			args[0] = argv[0];
			args[1] = argv[1];

		}
		else{
			args = malloc(sizeof(char)*1);
			args[0] = argv[0];
		}
		
		if(execvp("/Users/davidlevi/documents/homework/programming/operating_systems/hw4/wordgen.exe",args)==-1){
			fprintf(stderr,"Error executing wordegn.exe\n");
		}
	}
	else{
		close(pipefd1[0]);
	}

	pid_t childpid2;
	if((childpid2 = fork())==-1){
		fprintf(stderr,"Error forking program\n");
	}

	if(childpid2==0){

		close(pipefd2[1]);
		if(dup2(pipefd2[0],STDIN_FILENO)==-1){
			fprintf(stderr,"Error duping second program\n");
		}
		close(pipefd2[0]);

		char** args2;
		if(argc>1){
			args2 = malloc(sizeof(char)*2);
			args2[0] = argv[0];
			args2[1] = argv[1];
		}
		else{
			args2 = malloc(sizeof(char)*1);
			args2[0] = argv[0];
		}

		execvp("/Users/davidlevi/documents/homework/programming/operating_systems/hw4/testlauncher.exe",args2);

	}
	else{
		close(pipefd2[0]);
		close(pipefd2[1]);
	}*/


	//char* buf = malloc(sizeof(char)*100);
	/*char* buf2;
	int count = 100;
	read(pipefd2[0],&buf2,100);
	printf("MESSAGE: %s from fd: %d\n",buf2, pipefd2[0]);*/
	/*FILE * fp;
	fp = fdopen(pipefd2[0], "r");
	char* buf;
	fread("\n", buf, 1, fp);*/

	

        
    /*if((childpid = fork()) == -1){
        fprintf(stderr,"Error during fork");
        exit(-1);
    }

    if(childpid==0){
    	close(pipefd1[1]);

    	
  		FILE *stream;
  		int c;
  		stream = fdopen (pipefd1[0], "r");
  		while ((c = fgetc (stream)) != EOF){
    		putchar (c);
  		}
  		fclose (stream);

  		return EXIT_SUCCESS;

    	

    }
    else{
    	close(pipefd1[0]);

    	FILE *stream;
  		stream = fdopen (pipefd1[1], "w");
  		fprintf (stream, "hello, world!\n");
  		fprintf (stream, "goodbye, world!\n");
  		fclose (stream);

    	return EXIT_SUCCESS;


    }*/

    



	return 0;
}