#ifndef H_MYMEMORY_
#define H_MYMEMORY_

#include "common.h"
#include "classlist.h"

class MyMemory{
public:
        atomic<QNode*> readHead;
        atomic<QNode*> readTail;
	atomic<QNode*> writeHead;
	atomic<QNode*> writeTail;
        MyMemory();
        QNode* newWriteNode();
        QNode* newReadNode();
        void deleteNode(QNode*);

};

#endif
