#include "rwlock.h"
#include "mymemory.h"

MyMemory *mymemory;

void *increment(void *args){
        struct ThreadInfo *input = (struct ThreadInfo *) args;
	input->lock->writeLock(input->tid);
	*input->val=*input->val + 1;
	printf("thread %d: incremented val=%d\n",input->tid, *input->val);
	input->lock->writeUnlock(input->tid);
}
void *read(void *args){
	struct ThreadInfo *input = (struct ThreadInfo *) args;
	input->lock->readLock(input->tid);
	printf("thread %d: val=%d\n", input->tid, *input->val);
	input->lock->readUnlock(input->tid);
}

int main(int argc,char** argv)
{
        pthread_t   threads [NUMOFTHREADS];
	mymemory = new MyMemory();
        int val = 0;
        RWLock lock;
        struct ThreadInfo ar1 = {&lock, &val, 0};
        struct ThreadInfo ar2 = {&lock, &val, 1};
        struct ThreadInfo ar3 = {&lock, &val, 2};
        struct ThreadInfo ar4 = {&lock, &val, 3};
        pthread_create(&threads[0], NULL, &read, &ar1);
        pthread_create(&threads[1], NULL, &read, &ar2);
	pthread_create(&threads[2], NULL, &increment, &ar3);
        pthread_create(&threads[3], NULL, &increment, &ar4);
                for(int i=0;i<NUMOFTHREADS;i++){
        	pthread_join(threads[i], NULL);
        }
	delete mymemory;
        return 0;
}

