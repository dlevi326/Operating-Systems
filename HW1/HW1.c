#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <time.h>

int main ( int argc, char *argv[] ){

	clock_t begin = clock();
	

	int buffer = 4096;
	char *infile = NULL;
	char *outfile = NULL;
	int intRead = 0;
	int fdout;

	for(int i=1;i<argc;i++){
		if(strcmp(argv[i],"-b")==0){
			buffer = atoi(argv[i+1]);
			if(buffer<=0){
				perror("Buffer size can not be less than or equal to 0");
				return -1;
			}
		}
		if(strcmp(argv[i],"-o")==0){
			outfile = argv[i+1];
		}
	}

	if(outfile == NULL){
		fdout = 1;
	}
	else{
		fdout = open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0666);
	}
	
	
	char buf[buffer];
	int filesExist = 0;
	int num;
	int temp = buffer;


	for(int i=1;i<argc;i++){
		if(intRead>=sizeof(buf)){
			break;
		}
		if(strcmp(argv[i],"-o")==0){
			//outfile = argv[i+1];
			i++;
			continue;
		}
		else if(strcmp(argv[i],"-b")==0){
			i++;
			continue;
		}
		else if(strcmp(argv[i],"-")==0){
			/*num = read(0,buf+intRead,sizeof(buf));
			intRead+=num;
			while(num!=0){
				num = read(0,buf+intRead,sizeof(buf));
				intRead+=num;
			}*/
			//intRead += read(0,buf+intRead,sizeof(buf));
			//while((intRead = read(0,buf,sizeof(buf)))>0){
			//	write(fdout,buf,intRead);
			//}
			int num=1;
			while(temp>0||(num>0)){
					printf("%d ",temp);
					num = read(0,buf,temp);
					temp -= num;
					write(fdout,buf,num);
			}
			filesExist = 1;
		}
		else{
			
			filesExist = 1;
			int fd = open(argv[i], O_RDONLY);
			if(fd<0){
				perror("Error opening file");
				return -1;
			}
			else{
				while(temp>0){
					//printf("%d ",temp);
					int num = read(fd,buf,temp);
					temp -= num;
					write(fdout,buf,num);
				}
				//intRead += read(fd,buf+intRead,sizeof(buf));
			}
			if(close(fd)==-1){
				perror("Error closing file");
			}
		}
	}
	close(fdout);


	if(filesExist == 0&&intRead<sizeof(buf)){
		/*while((num = read(0,buf+intRead,sizeof(buf)))!=0){
			intRead+=num;
		}*/
		intRead += read(0,buf+intRead,sizeof(buf));
	}

	/*int fdout;
	if(outfile!=NULL){
		fdout = open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0666);
		if(fdout<0){
			perror("Error opening out file");
		}
		int wj=write(fdout,buf,intRead);
		if(wj!=intRead){
			perror("Write Error");
		}
		if(close(fdout)==-1){
			perror("Error closing file");
		}
	}
	else{

		int wj = write(1,buf,intRead);
		if(wj!=intRead){
			perror("Write Error");
		}

	}*/
	
	
    clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time taken: %f\n",time_spent);
	
	return 0;

}