#include "readtree.h"
#include "qnode.h"
#include <assert.h>
void QNode::root() {

    do {
        x = atomic_load(&X);
        if (x->c == 0) {
            Y->c = 1;
            Y->a = true;
            printf("root is added\n");
        } else {
            Y->c = ((x->c) + 1);
            Y->a = x->a;
        }
    }while(atomic_compare_exchange_strong(&X,&x,Y));

}

void QNode::addReq(){
    bool success = false;
    int undoarr = 0;

    while(!success) {
        x = atomic_load(&X);
        Y->c = 1;
        if ((x->c) >= 1) {
            if (atomic_compare_exchange_strong(&X, &x, Y)) {
                success = true;
                printf("The child added successssssss\n");
            }
        } else if (x->c == 0) {
            Y->c = 1 / 2;
            if (atomic_compare_exchange_strong(&X, &x, Y)) {
                success = true;
                printf("child successfully added \n");
            }
        } else {
            parent->addReq();
            Y->c = 1;
            if (!atomic_compare_exchange_strong(&X, &x, Y)) {
                undoarr = undoarr + 1;
                printf("Check the state of parent \n");
            }
        }
    }
    while(undoarr>0){
        parent->done();
        undoarr = undoarr -1;
    }
}

void QNode::done(){
    while(true) {
        x = atomic_load(&X);
        Y->c = ((x->c) - 1);
        Y->a = false;
        if (atomic_compare_exchange_strong(&X, &x, Y)) {
            if (x->c >= 2) {
                printf("the lock is there \n");
                return;
            } else if (x->c == 1) {
                parent->done();
                printf("the parent is exiting \n");
            } else
                return;
        }
    }
}

