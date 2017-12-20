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

/*char buf[MYFIFO_BUFSIZ];
	int next_write;
	int next_read;
	int item_count;
	struct cv* full;
	struct cv* empty;
	struct spinlock* mutex;*/



void fifo_init(struct fifo *f){

	spinlock_init(&f->mutex,0,0);
	cv_init(&f->full);
	cv_init(&f->empty);

	f->next_read = 0;
	f->next_write = 0;
	f->item_count = 0;


}

void fifo_wr(struct fifo *f,unsigned long d){
	
	
	spin_lock(&f->mutex);

	printf("Proccount of empty is: %d\n",f->empty.proccount);
	while(f->item_count>=MYFIFO_BUFSIZ){
		
		cv_wait(&f->full,&f->mutex);
	}
	
	f->buf[f->next_write++] = d;
	f->next_write%=MYFIFO_BUFSIZ;
	f->item_count++;
	
	printf("Sending signal that space has opened up to empty\n");
	cv_signal(&f->empty);
	/*if(!cv_broadcast(f->empty)){
		printf("No processes woken!!!\n");
	}*/
		
	spin_unlock(&f->mutex);
	


}

unsigned long fifo_rd(struct fifo *f){

	unsigned long ul;
	spin_lock(&f->mutex);
	
	while(f->item_count<=0){
		printf("Waiting for empty\n");
		cv_wait(&f->empty,&f->mutex);
		printf("Finished waiting for empty\n");
	}
	ul = f->buf[f->next_read++];
	f->next_read%=MYFIFO_BUFSIZ;
	f->item_count--;

	printf("Sending signal that space has opened up to full\n");
	cv_signal(&f->full);
		
		
	
	spin_unlock(&f->mutex);

	return ul;
	
}










