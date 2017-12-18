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
#include <signal.h>

void handler(int signo){
        fprintf(stderr, "Signal recieved: %d\n", signo);
        exit(signo);
}

void questionOne(){
        for(int j = 0; j<32; j++){
                if (signal(j, handler) == SIG_ERR){
                        fprintf(stderr,"signal: %d\n", j);
                }
        }

        void* addr = NULL;
        int prot = PROT_READ;
        int flags = MAP_SHARED;

        char* buf = "This is initial text 1";
        int fd = open("text1.txt", O_RDWR|O_TRUNC|O_CREAT, 0666); // Creating first text file
        if(fd==-1){
                fprintf(stderr,"Error opening file\n");
                exit(-1);
        }
        if(write(fd,buf,strlen(buf))==-1){
                fprintf(stderr,"Error writing to text file\n");
                exit(-1);
        }

        struct stat sb;
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
                exit(-1);
        }
        int length = sb.st_size;

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping memory\n");
                exit(-1);
        }
        
        fprintf(stderr,"Attempting to write a string....\n");
        char* word = "Other message";
        for(int i=0;i<strlen(word);i++){
                pm[i] = word[i];
        }

        if(msync(pm,length, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing to file\n");
                exit(-1);
        }

        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error seeking file\n");
                exit(-1);
        }

        char* buf2 = malloc(sizeof(char)*1);

        if(read(fd,buf2,1)==-1){
                fprintf(stderr,"Error reading from file\n");
                exit(-1);
        }

        if(strcmp(buf2,&buf[0])==0){
                exit(255);
        }
        else{
                exit(0);
        }

        fprintf(stderr,"Wrote to memory succesfully\n");

}

void questionTwoAndThree(int question){

        for(int j = 0; j<32; j++){
                if (signal(j, handler) == SIG_ERR){
                        fprintf(stderr,"signal: %d\n", j);
                }
        }

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

        if(fd==-1){
                fprintf(stderr,"Error opening file\n");
                exit(-1);
        }

        if(write(fd,buf,strlen(buf))<0){
                fprintf(stderr,"Error writing to file\n");
                exit(-1);
        }

        struct stat sb;
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
                exit(-1);
        }
        int length = sb.st_size;

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping mem\n");
                exit(-1);
        }

        char letter = 'X';
        char temp = pm[0];
        pm[0] = letter;
        
                
        if(msync(pm,length, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing to file\n");
                exit(-1);
        }

        char* buf2 = malloc(sizeof(char)*1);
        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking\n");
                exit(-1);
        }
        if(read(fd,buf2,1)==-1){
                fprintf(stderr,"Error reading from file\n");
                exit(-1);
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
        for(int j = 0; j<32; j++){
                if (signal(j, handler) == SIG_ERR){
                        fprintf(stderr,"signal: %d\n", j);
                }
        }

        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags = MAP_SHARED;


        char* buf = "This is initial text 3";
        int fd = open("text3.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);

        if(fd==-1){
                fprintf(stderr,"Error opening file\n");
        }

        int count=0;
        int bytenum;
        while((bytenum = write(fd,"Y",1))&&count<5000){
                if(bytenum==-1){
                        fprintf(stderr,"Error writing to file\n");
                        exit(-1);
                }
                count++;
        }

        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking file\n");
                exit(-1);
        }


        struct stat sb;

        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
                exit(-1);
        }

        int length = sb.st_size; 

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping mem\n");
                exit(-1);
        }

        char letter = 'X';
        pm[length+5] = letter; // To bring length to a theoretical 5530
        
                
        
        if(msync(pm,length+5, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing back to file\n");
                exit(-1);
        }

        struct stat sb2;
        if(fstat(fd,&sb2)==-1){
                fprintf(stderr,"Error stating modified file\n");
                exit(-1);
        }
        int lengthmod = sb2.st_size;
        if(lengthmod==length){
                printf("File size stayed the same, exiting with 1\n");
                exit(1);
        }
        else{
                printf("File size changed, exiting with 0\n");
                exit(0);
        }

}

void questionFive(){
        for(int j = 0; j<32; j++){
                if (signal(j, handler) == SIG_ERR)
                fprintf(stderr,"signal: %d\n", j);
        }

        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags = MAP_SHARED;


        char* buf = "This is initial text 4";
        int fd = open("text4.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);
        if(fd==-1){
                fprintf(stderr,"Error opening file\n");
                exit(-1);
        }

        int count=0;
        int bytenum;
        while((bytenum = write(fd,"Y",1))&&count<5000){
                if(bytenum==-1){
                        fprintf(stderr,"Error writing to file\n");
                        break;
                }
                count++;
        }

        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking file\n");
                exit(-1);
        }


        struct stat sb;
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
        }
        int length = sb.st_size; // Length is 5523 --> "...from other 8-bit encodings.[3]"

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, length-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping memory\n");
                exit(-1);
        }

        char letter = 'X';
        pm[length+5] = letter; // To bring length to a theoretical 5530
        
        if(msync(pm,length+5, MS_SYNC)==-1){
                fprintf(stderr,"Error syncing to file\n");
                exit(-1);
        }
        if(lseek(fd,length+16,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking file\n");
                exit(-1);
        }
        if(write(fd,"Y",1)==-1){
                fprintf(stderr,"Error writing\n");
                exit(-1);
        }
        if(lseek(fd,length+5,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking file\n");
                exit(-1);
        }
        char* buf2 = malloc(sizeof(char)*1);
        if(read(fd,buf2,1)==-1){
                fprintf(stderr,"Error reading from file\n");
                exit(-1);
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
        for(int j = 0; j<32; j++){
                if (signal(j, handler) == SIG_ERR){
                        fprintf(stderr,"signal: %d\n", j);
                }
        }

        void* addr = NULL;
        int prot = PROT_READ|PROT_WRITE;
        int flags = MAP_SHARED;


        char* buf = "This is initial text 5";
        int fd = open("text5.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);
        if(fd==-1){
                fprintf(stderr,"Error opening file\n");
        }

        int count=0;
        int bytenum;
        while((bytenum = write(fd,"Y",1))&&count<3000){
                if(bytenum == -1){
                        fprintf(stderr, "Error writing to file\n");
                }
                count++;
        }

        if(lseek(fd,0,SEEK_SET)==-1){
                fprintf(stderr,"Error lseeking file\n");
                exit(-1);
        }


        struct stat sb;
        
        if (fstat(fd, &sb) == -1){
                fprintf(stderr,"Error stating file\n");
                exit(-1);
        }
        int length = sb.st_size; 

        off_t offset = 0;
        off_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        char* pm = mmap(addr, (8192)-pa_offset, prot, flags, fd, pa_offset);

        if (pm == MAP_FAILED){
                fprintf(stderr,"Error mapping memory\n");
                exit(-1);
        }

        
        char letter = pm[4000];
        fprintf(stderr,"No signal within one page and read: %c\n",letter);

        
        char letter2 = pm[8000];
        fprintf(stderr,"No signal within two pages and read: %c\n",letter2);



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













