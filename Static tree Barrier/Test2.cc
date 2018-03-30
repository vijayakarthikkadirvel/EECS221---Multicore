#include <stdio.h>
#include "StaticTreeBarrier.h"
#include "librace.h"

#ifndef ENABLE_PTHREAD
void printing(void *args){
#else
void *printing(void *args){
#endif
	struct ThreadInfo *input = (struct ThreadInfo *) args;
        input->stb->await(input->tid);
        input->stb->await(input->tid);
}


#ifndef ENABLE_PTHREAD
int user_main(int argc,char** argv)
#else
int main(int argc,char** argv)
#endif
{
#ifndef ENABLE_PTHREAD
	thrd_t threads [NUMOFTHREADS];
#else
	pthread_t   threads [NUMOFTHREADS];
#endif
	int val[NUMOFTHREADS] = {0, 0, 0};
	StaticTreeBarrier stb(NUMOFTHREADS,RADIX);
	struct ThreadInfo ar1 = {&stb, val, 0};
	struct ThreadInfo ar2 = {&stb, val, 1};
	struct ThreadInfo ar3 = {&stb, val, 2};
#ifndef ENABLE_PTHREAD
	thrd_create(&threads[0],(thrd_start_t)&printing, &ar1);
	thrd_create(&threads[1],(thrd_start_t)&printing, &ar2);
	thrd_create(&threads[2],(thrd_start_t)&printing, &ar3);
#else
	pthread_create(&threads[0], NULL, printing, &ar1);	
	pthread_create(&threads[1], NULL, printing, &ar2);
	pthread_create(&threads[2], NULL, printing, &ar3);
#endif
        
#ifndef ENABLE_PTHREAD
	for(int i=0;i<NUMOFTHREADS;i++){
		thrd_join(threads[i]);
	}
#else
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
#endif
	
        return 0;
}

