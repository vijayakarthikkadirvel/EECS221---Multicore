#ifndef H_RWLOCK_
#define H_RWLOCK_

#include "common.h"


class RWLock{
atomic<QNode*> tail;
QNode* mynode[NUMOFTHREADS];

public:
	RWLock();
	void readLock(int tid);
	void readUnlock(int tid);
	void writeLock(int tid);
	void writeUnlock(int tid);
};

#endif
