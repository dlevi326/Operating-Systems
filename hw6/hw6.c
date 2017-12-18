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

int tas(volatile char *lock);




int main(int argc, char ** argv) {
        int numchildren = 1;

        if(!argv[1]){
                fprintf(stderr,"Please specify number of child processes\n");
        }
        else{
                numchildren = atoi(argv[1]);
        }

        int a=0;
        int b=0;
        int c=0;
        

        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags = MAP_ANON|MAP_SHARED;

        
        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        int length = 5000;
        int fd = -1;

        int* pm = mmap(addr, length, prot, flags, fd, 0);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping memory\n");
                exit(-1);
        }

        
        struct spinlock* sp = malloc(sizeof(struct spinlock));
        spinlock_init(sp,0,getpid());
        int retlock,retunlock;
        pm[0] = 0;
        printf("ORIGINALLY PM: %d\n",pm[0]);
        int count;
        int val = 0;

        if(!argv[2]){
                count = 2;
        }
        else{
                count = atoi(argv[2]);
        }
        int isfork = 0;
        for(int i=0;i<numchildren;i++){
                switch(fork()){
                        case -1:
                                fprintf(stderr,"Error forking\n");
                                //exit(-1);
                                break;
                        case 0:
                                isfork = 1;
                                
                                
                                
                                for(int k=0;k<100;k++){

                                }
                                for(int j=0;j<count;j++){
                                        spin_lock(sp);
                                        memcpy(&val,pm,sizeof(int));
                                        for(int k=0;k<100;k++){

                                        }
                                        val++;
                                        memcpy(pm,&val,sizeof(int));  
                                        spin_unlock(sp); 
                                }
                                
                                
                                exit(0);
                                break;
                        default:
                                break;

                }
                if(isfork){
                        break;
                }
        }
        struct rusage ru;
        int status;

        for(int i=0;i<numchildren;i++){
                wait3(&status,0,&ru);
        }
        memcpy(&val,pm,sizeof(int));
        printf("VALUE SHOULD BE: %d but \nA = %d\n",numchildren*count,val);

        

        return 0;
}


