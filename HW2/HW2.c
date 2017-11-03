// David Levi
// Operating Systems
// HW 2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>

int isExact(char* path,char* origname, int filesize){ // Returns 0 if different.... Returns 1 if exact


	FILE *pfl1;					
	FILE *pfl2;
	pfl1 = fopen(path,"rw");
	pfl2 = fopen(origname,"rw");

	if(pfl1==NULL){
		printf("EEEERRRORRR");
		exit(1);
	}
	if(pfl2==NULL){
		printf("EEEERRRORRR");
		exit(1);
	}

	char buf1[filesize];
	char buf2[filesize];

	int bytesread1 = fread(buf1,sizeof(buf1),1,pfl1);
	int bytesread2 = fread(buf2,sizeof(buf1),1,pfl2);

	fclose(pfl1);
	fclose(pfl2);
						


	if(memcmp(buf1,buf2,filesize)==0){
		return 1;
	}
	

	return 0;
		

}

int isExactSym(char* path,char* origname, int filesize){ // Returns 0 if different.... Returns 1 if exact

					
	FILE *pfl2;
	pfl2 = fopen(origname,"rw");

	if(pfl2==NULL){
		printf("EEEERRRORRR");
		exit(1);
	}

	char buf1[filesize];
	char buf2[filesize];
	char *bufsyms;

	bufsyms = realpath(path,NULL);

	FILE *pfl1;
	pfl1 = fopen(bufsyms,"rw");

	if(pfl1==NULL){
		printf("ERROR opening %s\n",path);
		perror("");
		exit(1);
	}

	int bytesread2 = fread(buf2,sizeof(buf2),1,pfl2);
	int bytesread1 = fread(buf1,sizeof(buf2),1,pfl1);

	fclose(pfl2);
	fclose(pfl1);

	if(memcmp(buf1,buf2,1)==0){
		return 1;
	}
	
	return 0;
		

}


void pathname(char *argv, long long inodenum, long long filesize,char *origname,int flag){

	DIR *dirp;
	struct dirent *de;
	char *dirname = argv;
	struct stat statbuf;
	struct stat statbufsym;

	if(!(dirp = opendir(dirname))){
		printf("Can not open directory %s\n",dirname);
		perror("");
       	return;
	}

	int status;
	

	while((de = readdir(dirp))!=NULL){
		char path[1024];
		
		if(de->d_type==DT_DIR){


			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0){
				continue;
			}
			snprintf(path, sizeof(path), "%s/%s", dirname, de->d_name);
			pathname(path,inodenum,filesize,origname,flag);

		}
		else{
			snprintf(path, sizeof(path), "%s/%s", dirname, de->d_name);
			if((status = stat(path,&statbuf))==-1&&de->d_type!=DT_LNK){
				printf("Unable to stat %s\n",path);
				perror("");
			}
				
			switch(de->d_type){
				case DT_DIR:
					printf("-- This is a directory\n");
					break;
				case DT_REG:

					if(statbuf.st_ino == inodenum){

						printf("%s\t\t\tHARD LINK TO TARGET\t",path);
						printf("nlink = %d\t",statbuf.st_nlink);
						printf( (statbuf.st_mode & S_IROTH) ? "OK READ BY OTHER\n" : "NOT READABLE BY OTHER\n");
					}
					else if(statbuf.st_size==filesize){

						if(isExact(path,origname,filesize)==1){
							printf("%s\t\tDUPLICATE OF TARGET\t",path);
							printf("nlink = %d\t",statbuf.st_nlink);
							printf( (statbuf.st_mode & S_IROTH) ? "OK READ BY OTHER\n" : "NOT READABLE BY OTHER\n");
						}

					}
					break;
				case DT_LNK:
					stat(path,&statbuf);
					if(lstat(path,&statbufsym)==-1){
						printf("Unable to lstat %s\n",path);
						perror("");
					}
					if(statbuf.st_ino==inodenum){
						printf("%s\t\tSYMLINK RESOLVES TO TARGET\t\n",path);
					}
					else{
						if(isExactSym(path,origname,filesize)==1){ 
							char buf1[1024];
							if(flag){
								readlink(path,buf1,1024);  
							}
							else{
								realpath(path,buf1);
							}
							
							printf("%s\t\tSYMLINK (%s) RESOLVES TO DUPLICATE\n",path,buf1);
						}
					}
					break;
				default:
					printf("\n");
					break;
			}
		}
	}



	return;




}

int isExactSymAbs(char* path,char* origname, int filesize){ // Returns 0 if different.... Returns 1 if exact



					
	FILE *pfl2;
	pfl2 = fopen(origname,"rw");

	if(pfl2==NULL){
		printf("EEEERRRORRR");
		exit(1);
	}

	char buf1[filesize];
	char buf2[filesize];
	char *bufsyms;

	bufsyms = realpath(path,NULL);

	FILE *pfl1;
	pfl1 = fopen(bufsyms,"rw");

	if(pfl1==NULL){
		printf("EEEERRRORRR");
		exit(1);
	}

	int bytesread2 = fread(buf2,sizeof(buf2),1,pfl2);
	int bytesread1 = fread(buf1,sizeof(buf2),1,pfl1);

	fclose(pfl2);
	fclose(pfl1);

	if(memcmp(buf1,buf2,1)==0){
		return 1;
	}
	
	return 0;
		

}

void debug(char *filename, long long *filesize, long long* inodenum){
	struct stat statbuf;
	if(stat(filename, &statbuf)==-1){
		printf("Error: Cannot stat %s\n",filename);
		perror("");
		exit(1);
	}

	*filesize = statbuf.st_size;
	*inodenum = statbuf.st_ino;
	//printf("DEBUG: Target is %lld bytes long, dev %d, ino %llu\n",statbuf.st_size,statbuf.st_dev,statbuf.st_ino);
}


int main(int argc, char **argv) {
	char c = *argv[2];

	long long filesize;
	long long *pfilesize = &filesize;

	long long inodenum;
	long long *pinodenump = &inodenum;
	debug(argv[1],pfilesize,pinodenump);

    if(c=='/'){
    	pathname(argv[2],inodenum,filesize,argv[1],0);
    }
    else{
    	pathname(argv[2],inodenum,filesize,argv[1],1);
    }



    return 0;
}