// David Levi
// Operating Systems
// HW 3

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

int executeCommand(char* command,int argc, char** argv, int last_status);
char** getCommands(char* command,int count);



int main(int argc, char** argv){

	char* comm;
	FILE* fp = NULL;
	size_t len = 0;
	size_t read;


	// For interpreter
	if (argc == 1) {
        fp = stdin;
    } else {
        if((fp = fopen(argv[1], "r"))<0){
        	fprintf(stderr,"Could not open file %s: %s\n",argv[1],strerror(errno));
        }
        
    }

    int last_status = 0;

	while ((read = getline(&comm, &len, fp)) != -1) {
        if (comm[0] == '#'||comm[0]=='\n') {
            continue;
        }

        executeCommand(comm, argc, argv,last_status);
    }
    printf("End of file read, exiting shell with exit code %d\n",last_status);
    free(comm);
    return 0;

	

}

int executeCommand(char* command,int argc, char** argv, int last_status){

	
	char* copy = malloc(sizeof(char) * strlen(command));
	strcpy(copy,command);

	char* token = strtok(copy, "\t ");
	int count = 0;
	while(token!=NULL){
		++count;
		token = strtok(NULL," ");
	}
	free(copy);

	char** commlist = getCommands(command,count);

	struct timeval start_time;

	if (gettimeofday(&start_time, NULL) < 0) {
        fprintf(stderr, "Error getting start time: %s\n", strerror(errno));
    }

	if(strcmp(commlist[0],"cd")==0){
		// Change directory
		if(chdir(commlist[1])<0){
			fprintf(stderr,"Failure to change directory to %s: %s\n",commlist[1],strerror(errno));
		}

	}
	else if(strcmp(commlist[0],"exit")==0){
		// Exit with status provided
		if(commlist[1] != NULL){
			printf("End of file read, exiting shell with exit code %d\n",atoi(commlist[1]));
			exit(atoi(commlist[1]));
		}
		else{
			printf("End of file read, exiting shell with exit code %d\n",last_status);
			exit(last_status);
		}
	}

	else{

		
		char* filename;
		int fd,fout,commCount;


		commCount = 0;
			
		for(int i=0;i<count;i++){
			if(commlist[i][0]!='<'&&commlist[i][0]!='>'&&commlist[i][0]!='2'){
						commCount++;
			}
		}
		
    	struct timeval end_time;
    	struct rusage ru;
		pid_t pid = fork();

		switch(pid){

			case 0:
				// Child
				for(int i=commCount;i<count;i++){
					switch(commlist[i][0]){
						
						case '<': // Open filename and redirect stdin

							filename = commlist[i];
							filename++;
							fd = open(filename,O_WRONLY,0666);
							fout = 0;
							break;


						case '>':
							if(commlist[i][1]=='>'){ // Open/Create/Append filename and redirect stdout
								filename = commlist[i];
								filename+=2;
								fd = open(filename,O_WRONLY|O_CREAT|O_APPEND,0666);
								fout = 1;
							}
							else{ // Open/create/Truncate filename and redirect stdout
								filename = commlist[i];
								filename++;
								fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC,0666);
								fout = 1;
								
							}
							break;

						case '2':
							if(commlist[i][1]=='>'){
								if(commlist[i][2]=='>'){ // Open/Create/Append filename and redirect stderr
									filename = commlist[i];
									filename+=3;
									fd = open(filename,O_WRONLY|O_CREAT|O_APPEND,0666);
									fout = 2;
									
								}
								else{ // Open/Create/Truncate filename and redirect stderr
									filename = commlist[i];
									filename+=2;
									fd = open(filename,O_WRONLY|O_CREAT|O_TRUNC,0666);
									fout = 2;
									
								}

							}
							break;

						default:
							fd = 0;
							fout = 1;
							break;

					}
					if(fd<0){
						fprintf(stderr,"Error opening %s: %s\n",filename,strerror(errno));
					}

					if(dup2(fd,fout)<0){
						fprintf(stderr, "Unable to dup2 %s: %s\n",filename, strerror(errno));
					}
					close(fd);

					
					
				}


				char** commArg = malloc((commCount-1)*sizeof(char*)); 
				for(int i=0;i<commCount;i++){
					commArg[i] = commlist[i];
				}
				free(commlist);

				fprintf(stderr,"Executing command: %s -- With arguments: %s\n",commArg[0],commArg[1]);
				if(execvp(commArg[0],commArg)==-1){
					fprintf(stderr,"Error Executing %s: %s\n",commArg[0],strerror(errno));
				}
				free(commArg);
				//close(fout);
				//exit(1);

				break;

			case -1:
				// Error
				fprintf(stderr,"Cannot fork: %s\n",strerror(errno));
				break;

			default:
				// Parent
				
				wait3(&pid,0,&ru);
				gettimeofday(&end_time, NULL);
				int secdiff = end_time.tv_sec - start_time.tv_sec;
            	int microdiff = end_time.tv_usec - start_time.tv_usec;
				fprintf(stderr,"Command returned with return code %d,\n", WEXITSTATUS(pid));
				last_status = WEXITSTATUS(pid);
				fprintf(stderr,"consuming %01d.%03d real seconds, %lu.%d user, %lu.%03d system.\n", 
					secdiff, microdiff, ru.ru_utime.tv_sec, ru.ru_utime.tv_usec, ru.ru_stime.tv_sec, ru.ru_stime.tv_usec);


		}

	}
	return last_status;
	 

}

// Get commands for a given instruction to be executed
char** getCommands(char* command,int count){

	char** commlist = malloc(count*sizeof(char*));
	int ind = 0;
	command[strlen(command)-1] = '\0';
	char delimiters[] = "  \t";
	char* token = strtok(command, delimiters);
	while(token!=NULL){
		commlist[ind] = token;
		token = strtok(NULL,delimiters);
		++ind;

	}

	return commlist;

}








