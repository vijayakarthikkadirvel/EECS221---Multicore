#include "qnode.h"
#include "readtree.h"



QNode::QNode(bool read){
	static isRead=read;
	locked = false;
	next = NULL;
	if(isRead){
		tree = new ReadTree();
	}else {
		tree = NULL;
	}
}
