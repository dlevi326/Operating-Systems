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



void fifo_init(struct fifo *f){

}

void fifo_wr(struct fifo *f,unsigned long d){

}

unsigned long fifo_rd(struct fifo *f){
	return 0;
}

