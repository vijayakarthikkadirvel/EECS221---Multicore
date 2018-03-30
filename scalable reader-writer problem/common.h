#ifndef H_COMMON_
#define H_COMMON_

#include <atomic>
#include "classlist.h"
using namespace std;


#define NUMOFTHREADS 4
#define NUMREADTHREAD 2
#define NUMNODES 30
#define PTHRD

#ifdef PTHRD
        #include "pthread.h"
        #include <stdio.h>
#else
        #include "threads.h"
        #include "librace.h"
#endif

struct ThreadInfo {
        RWLock* lock;
        int* val;
        int tid;
};


#endif
