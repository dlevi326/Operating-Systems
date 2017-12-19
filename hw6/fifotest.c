#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include "spinlock.h"
#include "cv.h"

int tas(volatile char *lock);




int main(int argc, char ** argv) {

	

	void* addr = NULL;
    int prot = PROT_READ|PROT_WRITE;
    int flags = MAP_SHARED|MAP_ANON;

    off_t offset = 0;
    off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

    char* pm = mmap(addr, sizeof(int)*100, prot, flags, -1, pa_offset);

    if (pm == MAP_FAILED){
            fprintf(stderr,"Error mapping memory\n");
            exit(-1);
    }

    struct cv* newcv;
	newcv = (struct cv*)(pm+sizeof(int)*8);
	cv_init(newcv);
	

	int num;
	pid_t cpid;
	switch(cpid = fork()){
		case 0:
			printf("In child rn\n");

			cv_wait(newcv,newcv->mutex);
			fprintf(stderr,"Awoken!\n");

			exit(1);
			break;
		default:
			printf("In parent rn\n");
			

			for(int i=0;i<100000;i++){
		
			}
			cv_broadcast(newcv);
			//printf("Currently sending sig to %d\n",newcv->procs[newcv->proccount]);
			//kill(newcv->procs[0],SIGUSR1);
			printf("Still in parent thooooo\n");
			break;
	}


	int status;
	while(wait(&status)){

	}

	
	printf("Proccount is now %d\n",newcv->proccount);
	





	return 0;
}


// m is buffer (Same as a mutex)
/*void producer(){
	while(producer can still produce){
		x = produce();
		m.append(x);
	}
}

void consumer(){
	while(can still consume){
		x = m.take();
		consume(x);
	}
}

void main(){
	parallel launch (producer(),consumer());
}*/











