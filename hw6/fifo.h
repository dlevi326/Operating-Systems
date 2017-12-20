#ifndef _FIFO_H
#define _FIFO_H

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

#define MYFIFO_BUFSIZ 1024

volatile struct fifo{

	unsigned long buf[MYFIFO_BUFSIZ];
	int next_write;
	int next_read;
	int item_count;
	struct cv full; // Made it regular, no pointer
	struct cv empty;
	struct spinlock mutex;




}fifo;

void fifo_init(struct fifo *f);
/* Initialize the shared memory FIFO *f including any required underlying
 * initializations (such as calling cv_init).  The FIFO will have a static
 * fifo length of MYFIFO_BUFSIZ elements.   #define this in fifo.h.
 * A value of 1K is reasonable.
*/
void fifo_wr(struct fifo *f,unsigned long d);
/* Enqueue the data word d into the FIFO, blocking unless and until the
 * FIFO has room to accept it. (i.e. block until !full)
 */
unsigned long fifo_rd(struct fifo *f);
/* Dequeue the next data word from the FIFO and return it.  Block unless
 * and until there are available words.  (i.e. block until !empty)
 */


#endif


