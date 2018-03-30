#include "rwlock.h"
#include "qnode.h"
#include "readtree.h"
#include "mymemory.h"


//extern MyMemory* mymemory;

RWLock::RWLock(){
	QNode *nullvar = NULL;
	atomic_init(&tail, nullvar);
	for(int i=0; i<NUMOFTHREADS; i++){
		mynode[i] = NULL;
	}
}

void RWLock::writeLock(int tid){
	printf("(%d)Sending wirteLock\n", tid);
//        mynode[tid] = mymemory->newWriteNode();
	mynode[tid] = new QNode(false);
        QNode* n = mynode[tid];
        QNode* prev = atomic_exchange(&tail,n);
        if(prev != NULL){
                n->locked = true;
                prev->next = n;
                while(n->locked){
			printf("(%d)writeLock waiting to be unlocked\n", tid);
		}
		printf("(%d)Yes, In writeLock, It got unlocked\n", tid);
        }
}

void RWLock::writeUnlock(int tid){
	printf("(%d)Sending writeUnlock\n", tid);
        QNode* n = mynode[tid];
	QNode* next = n->next;
        if(next == NULL){
                if(atomic_compare_exchange_strong(&tail, &n, next))
                        return;
                while(n->next == NULL){
			printf("(%d)tail->next became non-null and waiting for next!\n", tid);
		}
        }
	printf("(%d) writeUnlock: unlocking the next QNode\n", tid);
        n->next->locked = false;
	mynode[tid] = NULL;
        //mymemory->deleteNode(n);
}

void RWLock::readLock(int tid){
        //QNode *n = mymemory->newReadNode();
	QNode *n = new QNode(true);
	n->root();
	while(true){
		printf("RWLock::readLock\n");
		QNode * last = atomic_load(&tail);
		if(last == NULL){
			if(atomic_compare_exchange_strong(&tail, &last, n) ){
				mynode[tid] = n;
				return;
			}
		}else if(last->isRead){
			last->addReq();
			return;
		}else {// Last node is write
			if(atomic_compare_exchange_strong(&tail, &last, n)){
				mynode[tid] = n;
				n->locked=true;
				last->next = n;
				while(n->locked);
				return;
			}
			
		}
	}
/*
        QNode *prev = atomic_exchange(&tail, n);
        if(prev != NULL){
                n->locked = true;
                prev->next = n;
                if(prev->isRead){
                        while(prev->parent == NULL);
                        n->parent = prev->parent;
                        ReadTree *tree = n->parent->addReq(tid);
                        if( tree != NULL) {
                                n->deleted = true;
                                mynode[tid] = n->parent;
                                tree->wait();
                        } else {
                                n->parent = n;
                                mynode[tid] = n;
                                ReadTree* tree = newReadTree();
                                tree->addReq(tid);
                                n->tree = tree;
                                n->tree->wait();
                        }
                }else { //Prev is write!
                        n->parent = n;
                        mynode[tid] = n;
                        ReadTree* tree = newReadTree();
                        tree->addReq(tid);
                        n->tree = tree;
                        n->tree->wait();
                }
        }
*/
}

void RWLock::readUnlock(int tid){
        QNode * n = mynode[tid];
        n->done();
        QNode *next = n->next;
		if(next == NULL){
			if(atomic_compare_exchange_strong(&tail, &n, next)){
				printf("(%d)RWLock::readUnlock: tail has made null\n", tid);
				//mymemory->deleteNode(n);
				mynode[tid] = NULL;
				return;
			}
			while( n->next == NULL){
				printf("(%d)RWLock::readUnlock n->next is null\n", tid);
			}
		}
		printf("(%d)RWLock::readUnlock:: realeasing the other lock!\n", tid);
		n->next->locked = false;
		//mymemory->deleteNode(n);
		mynode[tid] = NULL;
        }
}

