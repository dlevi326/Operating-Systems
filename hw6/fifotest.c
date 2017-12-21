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

	if(argc!=2){
		fprintf(stderr,"Error: Usage <./fifotest> <test number>\n");
		exit(-1);
	}
	

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

	int testnum = atoi(argv[1]);
	if(testnum==1){
		test1(f);
	}
	else if(testnum==2){
		test2(f);
	}
	else{
		fprintf(stderr,"Error, invalid test number\n");
	}

	return 0;
}

void test1(struct fifo* f){
	unsigned long comparison = 0;
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
				unsigned long numread = fifo_rd(f);
				fprintf(stderr,"Read: %lu\n",numread);
				if(numread == comparison){
					comparison++;
				}

			}


			break;
	}
	int status;
	wait(&status);
	if(comparison==2000){
		fprintf(stderr,"!!! SUCCESS !!!\n");
	}
	else{
		fprintf(stderr,"*** Failure, comparison should be 2000, but it is: %lu\n",comparison);
	}
	exit(0);
}

void test2(struct fifo* f){

	int numwriters = 60;
	int items = 20;
	unsigned long tracker[numwriters*items];
	for(int i=0;i<numwriters*items;i++){
		tracker[i] = 1;
	}

	unsigned long threadtracker[numwriters*items];
	for(unsigned long ww=0;ww<numwriters*items;ww++){
		threadtracker[ww] = 0;
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
				if(threadtracker[numread%numwriters]>numread){
					fprintf(stderr,"****** Read %lu which is less than %lu ********\n",numread,threadtracker[numread%numwriters]);
					exit(0);
				}
				else{
					threadtracker[numread%numwriters] = numread;
				}
				if(tracker[numread]){
					tracker[numread] = 0;
				}
				else{
					fprintf(stderr,"****** Error, duplicate or absent item ******** (%lu)\n",numread);
					exit(0);
				}
			}
			fprintf(stderr,"!!!! SUCCESS !!!!!!\n"); // If it gets here it worked
			break;
	}
	exit(0);

}


