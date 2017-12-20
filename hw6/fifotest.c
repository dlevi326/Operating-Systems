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
#include "fifo.h"

int tas(volatile char *lock);

struct fifo* f;


int main(int argc, char ** argv) {
	

	void* addr = NULL;
    int prot = PROT_READ|PROT_WRITE;
    int flags = MAP_SHARED|MAP_ANON;

    off_t offset = 0;
    off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

    f = mmap(addr, sizeof(struct fifo), prot, flags, -1, pa_offset);

    if (f == MAP_FAILED){
            fprintf(stderr,"Error mapping memory\n");
            exit(-1);
    }

    //struct fifo* f;
	//f = (struct fifo*)(pm+sizeof(struct fifo)*8);
	fifo_init(f);

	/*switch(fork()){
		case 0:

			sleep(1);
			printf("Before writing, empty has proccount of: %d\n",f->empty.proccount);
			printf("EXECUTING WRITE\n");
			fifo_wr(f,11);

			exit(0);
			break;
		default:

			printf("EXECUTING READ\n");
			printf("Fifo reads: %lu\n",fifo_rd(f));

			break;
	}*/


	/*printf("Fifo reads: %lu\n",fifo_rd(f));
	fifo_wr(f,11);
	printf("Fifo reads: %lu\n",fifo_rd(f));*/


	switch(fork()){
		case -1:
			fprintf(stderr,"Error forking\n");
			exit(-1);
		case 0:
			// Writer
			for(unsigned long i=0;i<5;i++){
				fprintf(stderr,"Executing write\n");
				//printf("Empty has proccount %d -- Full has proccount %d\n",f->empty->proccount,f->full->proccount);
				fifo_wr(f,i);
			}
			sleep(1);
			for(unsigned long i=5;i<10;i++){
				fprintf(stderr,"Executing write\n");
				//printf("Empty has proccount %d -- Full has proccount %d\n",f->empty->proccount,f->full->proccount);
				fifo_wr(f,i);
			}


			exit(0);
			break;
		default:
			// Reader
			
			for(int j=0;j<10;j++){
				fprintf(stderr,"Executing read\n");
				fprintf(stderr,"Read: %lu\n",fifo_rd(f));
			}

			break;
	}






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











