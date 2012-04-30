#include "line.h"

//-------------------------------------------------------------------------------------------
// Node Class
//-------------------------------------------------------------------------------------------
template <class T>
class node{
 public:
	node *below; // node below in tower
	node *next; // next node in skip list
	int level; // level of this current node
	int height; // full number of levels in tower
    T data;
};
	
