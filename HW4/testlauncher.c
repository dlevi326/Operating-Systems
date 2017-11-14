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
	
	char* msg;

	if(argc>1){
		msg = argv[1];
	}
	else{
		msg = argv[0];
	}

	printf("%s\n",msg);



	return 0;
}