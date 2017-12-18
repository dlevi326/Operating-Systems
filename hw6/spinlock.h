#ifndef SPINLOCK_H
#define SPINLOCK_H

struct spinlock{

	char taschar;
	pid_t holderpid;
	int count;

};

void spinlock_init(struct spinlock* sp, char c, pid_t currpid);
void spin_lock(struct spinlock *l);
void spin_unlock(struct spinlock *l);



#endif