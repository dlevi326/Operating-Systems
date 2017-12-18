#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo){
    fprintf(stderr, "Signal recieved: %d\n", signo);
    exit(signo);
  }

int main(int argc, char ** argv){
  
  int j = 0;
  for(j = 0; j<32; j++){
    if (sigaction(j, handler) == SIG_ERR)
      fprintf(stderr,"signal: %d\n", j);
  }
  int fd;
  if((fd = open("testfile1.txt", O_RDWR|O_TRUNC|O_CREAT, 0666))<0){
    perror("Write error: ");
    exit(EXIT_FAILURE);
  }
  if(write(fd,"Hello world", 11)<0){
    perror("write error: ");
    exit(EXIT_FAILURE);
  }
  char *memory;
  if((memory = mmap(NULL, 4096, PROT_READ, MAP_SHARED,fd,0))<0){
    perror("Mmap error: ");
    exit(EXIT_FAILURE);
  }
  char letter = 'A';
  //if(write(fd, &letter,1)<0){
  //  perror("Write error: ");
  //  exit(255);
  //}
  *memory = 'A';
  if(*memory == 'H'){
    exit(255);
  }
  //while(1)
  //  sleep(1);

  return 0;
}
