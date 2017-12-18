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

        int length = 50;
        int fd = -1;

        char* pm = mmap(addr, length, prot, flags, fd, 0);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping memory\n");
                exit(-1);
        }

        
        struct spinlock* sp = malloc(sizeof(struct spinlock));
        spinlock_init(sp,0,getpid());
        pm[0] = 0;
        for(int i=0;i<numchildren;i++){
                switch(fork()){
                        case -1:
                                fprintf(stderr,"Error forking\n");
                                break;
                        case 0:
                                spin_lock(sp);
                                pm
                                spin_unlock(sp);
                                break;
                        default:
                                break;

                }
        }
        printf("A = %d\n",a);

        

        return 0;
}


