#ifndef SPINLOCK_H
#define SPINLOCK_H

struct spinlock{

	char taschar;
	int count;

};

void spinlock_init(struct spinlock* sp, char c);
void spin_lock(struct spinlock *l);
void spin_unlock(struct spinlock *l);
int tas(volatile char *lock);



#endif