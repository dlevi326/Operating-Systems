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

void handler(int signum){
	
}


void cv_init(struct cv *cv){

	cv->mutex = malloc(sizeof(struct spinlock));
	spinlock_init(cv->mutex,0,0);
	
	for(int i=0;i<CV_MAXPROC;i++){
		cv->procs[i] = 0;
	}
	cv->proccount = 0;


}

void cv_wait(struct cv *cv, struct spinlock *mutex){
	if(cv->proccount>=CV_MAXPROC){
		fprintf(stderr,"Error, too many processes\n");
		exit(-1);
	}
	else{
		fprintf(stderr,"Setting pid of %d to %d\n",cv->proccount,getpid());
		cv->procs[cv->proccount] = getpid(); // Putting process to sleep
		cv->proccount++;
		fprintf(stderr,"Confirming position %d has pid %d\n",cv->proccount-1,cv->procs[cv->proccount-1]);
	}

	
	
	spin_lock(mutex);
	
	fprintf(stderr,"Going to sleep....\n");
	signal(SIGUSR1, handler);
    sigset_t old_mask, new_mask;
    sigfillset(&new_mask);
    sigdelset(&new_mask, SIGINT);
    sigdelset(&new_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);


	/*printf("Before for loop:\n");
    for(int i=0;i<100000;i++){

    }
    printf("After for loop:\n");*/
    sigsuspend(&new_mask);
    spin_unlock(mutex);
    if(cv->proccount>0){
    	fprintf(stderr,"Waking up!\n");
    	fprintf(stderr,"Setting %d to 0\n",cv->procs[cv->proccount-1]);
    	cv->procs[cv->proccount-1] = 0;// Waking up process
    	cv->proccount--;
    	return;
    }
    
    
    sigprocmask(SIG_UNBLOCK, &new_mask, NULL);

	
	


}

int cv_broadcast(struct cv *cv){
	/*spin_lock(cv->mutex);
	int result = 0;

	for(int i=0;i<CV_MAXPROC;i++){
		if(cv->procs[i]>0){
			result++;
			kill(cv->procs[i],SIGUSR1);
			cv->procs[i] = 0;
		}
	}
	spin_unlock(cv->mutex);*/
	int result = 0;
	fprintf(stderr,"PID IN PROCS IS: %d\n",cv->procs[0]);
	fprintf(stderr,"Killing process %d\n",cv->procs[cv->proccount-1]);
	kill(cv->procs[cv->proccount-1],SIGUSR1);


	return result;
}

int cv_signal(struct cv *cv){
	return 0;
}
