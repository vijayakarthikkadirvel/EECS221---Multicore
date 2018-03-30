
/* 
 * File:   StaticTreeBarrier.h
 * Author: hamed
 *
 * Created on October 25, 2017, 9:50 AM
 */

#ifndef STATICTREEBARRIER_H
#define STATICTREEBARRIER_H

#include <atomic>

//#define ENABLE_PTHREAD

#ifndef ENABLE_PTHREAD
#include "threads.h"
#else
#include "pthread.h"
#endif

using namespace std;

#define NUMOFTHREADS 3
#define RADIX 2

class Node;

class StaticTreeBarrier {
private:
        int radix;
        int nodes;
	Node* n[NUMOFTHREADS];
	
public:
	atomic_bool sense;
	StaticTreeBarrier(int size, int myRadix);
	void build (Node* parent, int depth);
	void await(int tid);
};

class Node{
private:
	StaticTreeBarrier* barrier;
	bool threadsense;
	Node* parent;
	int children;
	atomic_int childcount;
public:
	Node(StaticTreeBarrier* _barrier, Node* myParent,int count);
	void wait();
	void childDone(){
		atomic_fetch_sub_explicit(&childcount, 1, memory_order_acq_rel);
	}
};

struct ThreadInfo {
	StaticTreeBarrier* stb;
	int* val;
	int tid;
};





Node::Node(StaticTreeBarrier* _barrier, Node* myParent,int count){
	atomic_init(&childcount,count);
	children = count;
	parent = myParent;
	barrier = _barrier;
	threadsense= !atomic_load_explicit(&barrier->sense, memory_order_acquire);
}

void Node::wait(){
	while(1){
		if(atomic_load_explicit(&childcount, memory_order_acquire)==0){
			break;
		}
#ifndef ENABLE_PTHREAD
		thrd_yield();
#endif
	}
	atomic_store_explicit(&childcount, children, memory_order_release);
	if(parent != NULL){
		parent->childDone();
		while(1){
			if( atomic_load_explicit(&barrier->sense, memory_order_acquire) == threadsense){
				break;
			}else{
#ifndef ENABLE_PTHREAD
				thrd_yield();
#endif
			}
		}
	} else{
		bool oldSense = atomic_load_explicit(&barrier->sense, memory_order_acquire);
		atomic_store_explicit(&barrier->sense, !oldSense , memory_order_release);
	}
	threadsense= !threadsense;
}



StaticTreeBarrier::StaticTreeBarrier(int size, int myRadix){
        radix = myRadix;
        nodes = 0;
        atomic_init(&sense, false);
        int depth = 0;
        while (size > 1) {
           depth++;
           size = size / radix;
        }
        build(NULL, depth);

}


void StaticTreeBarrier::build (Node* parent, int depth){
        if (depth == 0) {
                Node* mynod= new Node(this, parent,0);
                n[nodes]=mynod;
                nodes++;
        } else {
                Node* mynod = new Node(this, parent, radix);
                n[nodes]=mynod;
                nodes++;
                for (int i = 0; i < radix; i++) {
                        build(mynod, depth - 1);
                }
        }
}


void StaticTreeBarrier::await(int tid){
        n[tid]->wait();
}

#endif /* STATICTREEBARRIER_H */


