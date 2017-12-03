#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

void handleBus(){
        printf("Bus error encountered, exiting\n");
        exit(7);
}

void handleSeg(){
        printf("Seg fault encountered, exiting\n");
        exit(11);
}

void handleBus2(){
        fprintf(stderr,"Bus error encountered when reading beyond a page but within 2 pages\n");
        exit(7);
}

void questionOne(){
        void* addr = NULL;
        int prot = PROT_READ;
        int flags = MAP_SHARED;

        char* buf = "This is initial text 1";
        int fd = open("text1.txt", O_RDWR|O_TRUNC|O_CREAT, 0666);
        write(fd,buf,strlen(buf));

        struct stat sb;
        if(fd==-1){
                fprintf(stderr,"Error opening initialtext.txt\n");
        }
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
        }
        int length = sb.st_size;

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping mem\n");
        }

        signal(SIGSEGV,handleSeg);
        signal(SIGBUS,handleBus);
        
        char* word = "Other appended message";
        /*for(int i=0;i<strlen(word);i++){
                signal(SIGBUS,handleBus);
                signal(SIGSEGV,handleSeg);
                pm[i] = word[i];
        }*/
        if(memcpy(pm,word,strlen(word))==NULL){
                fprintf(stderr,"Error copying memory\n");
        }


        
        if(msync(pm,length, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing to file\n");
        }

        printf("Wrote to memory succesfully\n");

}

void questionTwoAndThree(int question){
        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags;
        if(question==2){
                flags = MAP_SHARED;
        }
        else{
                flags = MAP_PRIVATE;
        }


        char* buf = "This is initial text 2";
        int fd = open("text2.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);

        if(write(fd,buf,strlen(buf))<0){
                fprintf(stderr,"Error writing\n");
        }

        struct stat sb;
        if(fd==-1){
                fprintf(stderr,"Error opening initialtext.txt\n");
        }
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
        }
        int length = sb.st_size;

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping mem\n");
                exit(1);
        }

        char letter = 'X';
        char temp = pm[0];
        pm[0] = letter;
        signal(SIGBUS,handleBus);
        signal(SIGSEGV,handleSeg);
                
        if(msync(pm,length, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing to file\n");
        }

        char* buf2 = malloc(sizeof(char)*1);
        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking\n");
        }
        if(read(fd,buf2,1)==-1){
                fprintf(stderr,"Error reading from file\n");
        }
        printf("Read: %s\n",buf2);
        if(*buf2==letter){
                printf("Exiting with 0: file changed\n");
                pm[0] = temp;
                exit(0);// File changed
        }
        else{
                printf("Exiting with 1: File stayed the same\n");
                exit(1);// File stayed the same
        }

}

void questionFour(){
        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags = MAP_SHARED;


        char* buf = "This is initial text 3";
        int fd = open("text3.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);

        int count=0;
        while(write(fd,"Y",1)&&count<5000){
                count++;
        }

        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking\n");
        }


        struct stat sb;
        if(fd==-1){
                fprintf(stderr,"Error opening initialtext.txt\n");
        }
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
        }
        int length = sb.st_size; // Length is 5523 --> "...from other 8-bit encodings.[3]"

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping mem\n");
        }

        char letter = 'X';
        pm[length+5] = letter; // To bring length to a theoretical 5530
        signal(SIGBUS,handleBus);
        signal(SIGSEGV,handleSeg);
                
        
        if(msync(pm,length+5, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing to file\n");
        }

        struct stat sb2;
        if(fstat(fd,&sb2)==-1){
                fprintf(stderr,"Error stating modified file\n");
        }
        int lengthmod = sb2.st_size;
        if(lengthmod==length){
                printf("File size stayed the same, exiting with 1\n");
        }
        else{
                printf("File size changed, exiting with 0\n");
        }

}

void questionFive(){
        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags = MAP_SHARED;


        char* buf = "This is initial text 4";
        int fd = open("text4.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);

        int count=0;
        while(write(fd,"Y",1)&&count<5000){
                count++;
        }

        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking\n");
        }


        struct stat sb;
        if(fd==-1){
                fprintf(stderr,"Error opening initialtext.txt\n");
        }
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
        }
        int length = sb.st_size; // Length is 5523 --> "...from other 8-bit encodings.[3]"

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping mem\n");
        }

        char letter = 'X';
        pm[length+5] = letter; // To bring length to a theoretical 5530
        signal(SIGBUS,handleBus);
        signal(SIGSEGV,handleSeg);
        if(msync(pm,length+5, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing to file\n");
        }
        if(lseek(fd,length+16,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking\n");
        }
        if(write(fd,"Y",1)==-1){
                fprintf(stderr,"Error writing\n");
        }
        if(lseek(fd,length+5,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking\n");
        }
        char* buf2 = malloc(sizeof(char)*1);
        if(read(fd,buf2,1)==-1){
                fprintf(stderr,"Error reading\n");
        }
        if(strcmp(buf2,"X")==0){
                printf("Can see X, exiting 0\n");
                exit(0);
        }
        else{
                printf("Cannot see X, exiting 1\n");
                exit(1);
        }
        
        

        

}

void questionSix(){
        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags = MAP_SHARED;


        char* buf = "This is initial text 5";
        int fd = open("text5.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);

        int count=0;
        while(write(fd,"Y",1)&&count<3000){
                count++;
        }

        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking\n");
        }


        struct stat sb;
        if(fd==-1){
                fprintf(stderr,"Error opening initialtext.txt\n");
        }
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
        }
        int length = sb.st_size; // Length is 5523 --> "...from other 8-bit encodings.[3]"

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, (8192)-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping mem\n");
        }

        signal(SIGBUS,handleBus);
        char letter = pm[4000];
        fprintf(stderr,"Read: %c\n",letter);

        signal(SIGBUS,handleBus2);
        char letter2 = pm[8000];
        fprintf(stderr,"Read: %c\n",letter2);
        
}

int main(int argc, char** argv){

        int questionnumber;
        printf("Please enter the question number: ");
        scanf("%d",&questionnumber);

        switch(questionnumber){
                case 1:
                        questionOne();
                        break;
                case 2:
                        questionTwoAndThree(2);
                        break;

                case 3:
                        questionTwoAndThree(3);
                        break;

                case 4:
                        questionFour();
                        break;

                case 5:
                        questionFive();
                        break;

                case 6:
                        questionSix();
                        break;
                default:
                        fprintf(stderr,"Error, invalid question number\n");
        }


        return 0;
}













