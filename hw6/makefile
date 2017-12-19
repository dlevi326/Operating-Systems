all: tastest fifotest cvtest

tastest: tastest.c TAS64.S spinlock.c spinlock.h
	gcc tastest.c TAS64.S spinlock.c -o tastest

fifotest: fifotest.c cv.c cv.h spinlock.c spinlock.h  fifo.c fifo.h
	gcc fifotest.c TAS64.S spinlock.c cv.c fifo.c -o fifotest

cvtest: cvtest.c cv.c cv.h spinlock.c spinlock.h 
	gcc cvtest.c TAS64.S spinlock.c cv.c -o cvtest