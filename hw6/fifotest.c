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

void test1(struct fifo* f);
void test2(struct fifo* f);


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

	fifo_init(f);

	test2(f);

	//test1(f);

	return 0;
}

void test1(struct fifo* f){
	switch(fork()){
		case -1:
			fprintf(stderr,"Error forking\n");
			exit(-1);
		case 0:
			// Writer

			for(unsigned long i=0;i<2000;i++){
				fprintf(stderr,"Executing write\n");
				fifo_wr(f,i);
			}

			exit(0);
			break;
		default:
			// Reader
			
			for(int j=0;j<2000;j++){
				fprintf(stderr,"Executing read\n");
				fprintf(stderr,"Read: %lu\n",fifo_rd(f));
			}


			break;
	}
	sleep(2);
	printf("At the end, array has %lu at first index\n",f->buf[0]);
	exit(0);
}

void test2(struct fifo* f){

	int numwriters = 60;
	int items = 10;
	unsigned long tracker[numwriters*items];
	for(int i=0;i<numwriters*items;i++){
		tracker[i] = 1;
	}


	switch(fork()){
		case -1:
			fprintf(stderr,"Error forking\n");
			break;
		case 0:

			for(int i=0;i<numwriters;i++){
				int infork = 0;
				switch(fork()){
					case -1:
						fprintf(stderr,"Error forking\n");
						break;
					case 0:
						infork = 1;
						for(int k=0;k<items;k++){
							printf("EXECUTING WRITE:\n");
							fifo_wr(f,i+(numwriters*k));
						}
						exit(0);
						break;
					default:
						break;
				}
				if(infork){
					break;
				}
			}
			exit(0);
			break;
		default:
			for(int i=0;i<numwriters*items;i++){
				printf("EXECUTING READ:\n");
				unsigned long numread = fifo_rd(f);
				if(tracker[numread]){
					printf("Setting pos %lu to 0\n",numread);
					tracker[numread] = 0;
				}
				else{
					fprintf(stderr,"****** Error, duplicate or absent item ******** (%lu)\n",numread);
					exit(0);
				}
			}
			fprintf(stderr,"!!!! SUCCESS !!!!!!\n");
			break;
	}
	exit(0);

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











