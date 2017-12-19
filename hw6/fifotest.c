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











