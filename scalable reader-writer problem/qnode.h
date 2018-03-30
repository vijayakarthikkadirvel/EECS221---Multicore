#ifndef H_QNODE_
#define H_QNODE_

#include "common.h"

class QNode{
public:
        bool locked;
        QNode *next;
        static bool isRead;

        explicit QNode(bool isRead);
        static QNode* parent;
        bool i;
        int c ;
        bool a;
        QNode() {
            c = 0;
            parent = NULL;
            i = false;
            a = false;

        }
        atomic<QNode*> X{};
        QNode* x{};
        QNode* Y{};
        void root();
        void addReq();
        void done();

};


#endif
