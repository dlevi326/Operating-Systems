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

int tas(volatile char * lock);

void spinlock_init(struct spinlock* sp, char c, pid_t currpid){
	sp->taschar = c;
	sp->holderpid = currpid;
	sp->count = 0;
}

void spin_lock(struct spinlock *l){

	int flag = 1;
	while(flag){
		if(!tas(&(l->taschar))){
			l->taschar = 1;
			flag = 0;
		}
		else{
			while(tas(&(l->taschar)));
			flag = 0;
		}
	}
	


}

void spin_unlock(struct spinlock *l){
	l->taschar = 0;
}
