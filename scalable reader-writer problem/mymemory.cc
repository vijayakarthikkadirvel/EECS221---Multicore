#include "mymemory.h"
#include "qnode.h"

MyMemory::MyMemory(){
	QNode * nullvar = NULL;
	atomic_init(&readHead, nullvar);
	atomic_init(&readTail, nullvar);
	atomic_init(&writeHead, nullvar);
	atomic_init(&writeTail, nullvar);
}

QNode* MyMemory::newWriteNode(){
	QNode *head;
	while(true){
		head = atomic_load(&writeHead);
		if(head == NULL){
			return new QNode(false);
		}
		QNode *next = head->next;
		if(atomic_compare_exchange_strong(&writeHead, &head, next) )
			break;
	}
	return head;
}

QNode* MyMemory::newReadNode(){
	QNode *head;
        while(true){
                head = atomic_load(&readHead);
                if(head == NULL){
                        return new QNode(true);
                }
                QNode *next = head->next;
                if(atomic_compare_exchange_strong(&readHead, &head, next) )
                        break;
        }
        return head;
}

void MyMemory::deleteNode(QNode* node){
	node->next = NULL;
	node->locked = false;
	if(node->isRead){
		//TODO:increment node counter.
		QNode *prev = atomic_exchange(&readTail, node);
		if(prev != NULL){//Fixing bug with the head ...
			prev->next = node;
		}
	} else {
		QNode *prev = atomic_exchange(&writeTail, node);
		if(prev != NULL){
			prev->next = node;
		}
	}
}

