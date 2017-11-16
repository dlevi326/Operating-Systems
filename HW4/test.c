#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>


/**
 * Executes the command "cat scores | grep Villanova | cut -b 1-10".
 * This quick-and-dirty version does no error checking.
 *
 * @author Jim Glenn
 * @version 0.1 10/4/2004
 */

int main(int argc, char **argv)
{
  int status;
  int i;
  struct rusage ru;

  // arguments for commands; your parser would be responsible for
  // setting up arrays like these

  char *cat_args = "./wordgen.exe";
  char *grep_args = "./wordsearch.exe";
  char *cut_args = "./pager.exe";

  // make 2 pipes (cat to grep and grep to cut); each has 2 fds

  int pipes[4];
  pipe(pipes); // sets up 1st pipe
  pipe(pipes + 2); // sets up 2nd pipe

  // we now have 4 fds:
  // pipes[0] = read end of cat->grep pipe (read by grep)
  // pipes[1] = write end of cat->grep pipe (written by cat)
  // pipes[2] = read end of grep->cut pipe (read by cut)
  // pipes[3] = write end of grep->cut pipe (written by grep)

  // Note that the code in each if is basically identical, so you
  // could set up a loop to handle it.  The differences are in the
  // indicies into pipes used for the dup2 system call
  // and that the 1st and last only deal with the end of one pipe.

  // fork the first child (to execute cat)
  
  if (fork() == 0)
    {
      // replace cat's stdout with write part of 1st pipe

      dup2(pipes[1], 1);

      // close all pipes (very important!); end we're using was safely copied

      close(pipes[0]);
      close(pipes[1]);
      close(pipes[2]);
      close(pipes[3]);

      execvp(cat_args, argv);
    }
  else
    {
      // fork second child (to execute grep)

      if (fork() == 0)
	{
	  // replace grep's stdin with read end of 1st pipe
	  
	  dup2(pipes[0], 0);

	  // replace grep's stdout with write end of 2nd pipe

	  dup2(pipes[3], 1);

	  // close all ends of pipes

	  close(pipes[0]);
	  close(pipes[1]);
	  close(pipes[2]);
	  close(pipes[3]);

	  execvp(grep_args, argv);
	}
      else
	{
	  // fork third child (to execute cut)

	  if (fork() == 0)
	    {
	      // replace cut's stdin with input read of 2nd pipe

	      dup2(pipes[2], 0);

	      // close all ends of pipes

	      close(pipes[0]);
	      close(pipes[1]);
	      close(pipes[2]);
	      close(pipes[3]);

	      execvp(cut_args, argv);
	    }
	}
    }
      
  // only the parent gets here and waits for 3 children to finish
  
  close(pipes[0]);
  close(pipes[1]);
  close(pipes[2]);
  close(pipes[3]);

  /*for (i = 0; i < 3; i++){
  	wait3(&status,0,&ru);
  }*/
  int pchild1,pchild2,pchild3;
  int status1,status2,status3;
  struct rusage ru1;
  struct rusage ru2;
  struct rusage ru3;
  	pchild1 = wait3(&status1,0,&ru1);
	printf("Child %d return with %d\n",pchild1,WEXITSTATUS(status1));
	pchild2 = wait3(&status2,0,&ru2);
	printf("Child %d return with %d\n",pchild2,WEXITSTATUS(status2));
	pchild3 = wait3(&status3,0,&ru3);
	printf("Child %d return with %d\n",pchild3,WEXITSTATUS(status3));
    
}