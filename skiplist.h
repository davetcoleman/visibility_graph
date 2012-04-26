/* Skip List
   CSCI 5454 Algorithms
   Dave Coleman | david.t.coleman@colorado.edu

   2/2/2012

   Implementation of Skip Lists
*/

#include <math.h>
#include <iostream>
#include <cstdlib>
#include "node.h"

//using namespace std;

//-------------------------------------------------------------------------------------------
// Skip List Class
//-------------------------------------------------------------------------------------------
template <class T>
class skiplist{
   	// used for testing
public:
	double find_op;
	double add_op;
    double remove_op;
	int maxLevel;
	
private:
	node<T> *root;

	//-------------------------------------------------------------------------------------------
	// Get Random Level
	//-------------------------------------------------------------------------------------------	
	int getRandLevel()
	{
		int randResult = 1;
		int level = 0;
		while(randResult)
		{
			
			randResult = rand() % 2;
			if(randResult)
			{
				++level;
			}
			
			if(level > maxLevel)
			{
				randResult = 0; // to end the while loop
			}
		}
		return level;
			
	}
	//-------------------------------------------------------------------------------------------
	// Create New Node
	//-------------------------------------------------------------------------------------------	
	node<T>* createNode(double value, int level, int height, T data)
	{
		// Check if we are below level 0
		if(level < 0)
		{
			return NULL;
		}
		else // make a new node below
		{
			node<T> *newNode = new node<T>();			
			(*newNode).value = value;
			(*newNode).level = level;
			(*newNode).next = NULL;
			(*newNode).below = createNode(value, level - 1, height, data);
			(*newNode).height = height;
			(*newNode).data = data;
			return newNode;
		}
	}	
	
public:

	// Constructor:
	skiplist()
	{
		root = NULL;
		maxLevel = 0;

		// for testing
		find_op = 0;
		add_op = 0;
		remove_op = 0; 
		
		srand ( time(NULL) ); // seed the random generator
	}
	//-------------------------------------------------------------------------------------------
	// ADD
	//-------------------------------------------------------------------------------------------
	void add(double x, T data)
	{
		// Special Cases -------------------------------------------------------------------

		if(!root) // no root has been established yet
		{
			root = createNode(x, 0, 0, data);
			return;
		}

		if( (*root).value > x ) // new value goes before root
		{
			double temp_x = (*root).value;
			node<T> *n = root;

			for(int l = maxLevel; l >= 0; --l)
			{
				add_op += 1;
				// change the root to the new value
				(*n).value = x;
				n = (*n).below;
			}
			x = temp_x; // now we can resume regular insert operation with old root value
		}

		// Regular insert after root -------------------------------------------------------

		// Determine what level this new node will be at
		int level = getRandLevel();
	   
		// If new node is at whole new level, go ahead and update root node to be higher
		if(level > maxLevel)
		{
			maxLevel ++;
			node<T> *newRoot = new node<T>();
			(*newRoot).value = (*root).value;
			(*newRoot).next = NULL;
			(*newRoot).below = root;
			(*newRoot).level = maxLevel;
			root = newRoot;
		}

		// Create the new node
		node<T> *newNode = createNode(x, level, level, data);
					
		// Now add the node to the list
		//bool found = false;
		node<T> *i = root; 
		//node *prevNode = NULL; // used for inserting between two nodes

		// Loop down through all levels
		for(int l = maxLevel; l >= 0; --l)
		{
			add_op += 1;
			// move forward until we hit a value greater than ours
			while( (*i).next != NULL )
			{
				add_op += 1;
				if( (* (*i).next ).value > x ) // insert before i.next
				{
					break;
				}
				i = (*i).next;
			}

			// Check if we should add a pointer at this level
			if( l <= level )
			{
				(*newNode).next = (*i).next;
				(*i).next = newNode;
				
				// Now move the new node pointer one level down:
   				newNode = (*newNode).below;
			}

			// Always move the i node pointer one level down:
			i = (*i).below;			
		}	
	}
	//-------------------------------------------------------------------------------------------
	// FIND
	//-------------------------------------------------------------------------------------------
	bool find(double x)
	{
		node<T> *i = root;

		// Special case: skip list is empty
		if( !root )
		{
			return false;
		}

		// Special case: check root
		if( (*root).value == x)
		{
			return true;
		}

		for(int l = maxLevel; l >= 0; --l)
		{
			find_op += 1;
			// move forward until we hit a value greater than ours
			while( (*i).next != NULL )
			{
				find_op += 1;
				if( (* (*i).next ).value > x ) // x is not found on this level
				{
					break;
				}
				else if( (* (*i).next ).value == x ) // bingo!
				{
					return true;
				}
				
				i = (*i).next;
			}

			// Always move the i node<T> pointer one level down:
			i = (*i).below;			
		}	

		return false;
	}
	//-------------------------------------------------------------------------------------------
	// REMOVE
	//-------------------------------------------------------------------------------------------
	bool remove(double x)
	{
		node<T> *i = root;

		// Special case: remove root --------------------------------------------------------
		if( (*root).value == x)
		{
			// Get level 0 of root
			for(int l = (*root).level; l > 0; --l)
			{
				remove_op += 1;
				//std::cout << "Level " << l << std::endl;
				i = (*i).below;
			}

			// Check if there are any more nodes
			if( !(*i).next ) // the skip list is empty
			{
				root = NULL;
				maxLevel = 0;
					
				return true;
			}

			// Change value of root to next node
			//double root_x = (*root).value;
			node<T> *n = root;
			node<T> *nextNode = (*i).next;

			for(int l = maxLevel; l >= 0; --l)
			{
				remove_op += 1;
				// change the root to the new value
				(*n).value = (*nextNode).value;

				// update next pointer if the next next exists
				if( (*n).next )
				{
					(*n).next = (*(*n).next).next;					
				}

				// Move down to next level				
   				n = (*n).below;
			}

			return true;
		}

		// Normal case: remove after root -----------------------------------------------
		bool found = false;
		
		for(int l = maxLevel; l >= 0; --l)
		{
			remove_op += 1;
			// move forward until we hit a value greater than ours
			while( (*i).next != NULL )
			{
				remove_op += 1;
			    if( (* (*i).next ).value == x ) // remove this one
				{
					found = true;
					
					// pass through the pointer if exists
					if( (*i).next )
					{
						(*i).next = (*(*i).next).next;
					}
					else
					{
						(*i).next = NULL;
					}
					break;
				}
				else if( (* (*i).next ).value > x ) // x is not found on this level
				{
					break;
				}

				i = (*i).next;
			}

			// Always move the i node pointer one level down:
			i = (*i).below;
		}	

   		return found;
	}
	//-------------------------------------------------------------------------------------------
	// PRINT ALL
	//-------------------------------------------------------------------------------------------
	void printAll()
	{
		std::cout << std::endl << "LIST -----------------" << std::endl;

		// Special case: skiplist is empty
		if( !root )
		{
			std::cout << "----------------------" << std::endl;
			return;
		}

		node<T> i = *root;
		
		// Get level 0 of root
		for(int l = (*root).level; l > 0; --l)
		{
			//std::cout << "Level " << l << std::endl;
			i = *(i.below);
		}
		//std::cout << "we are on level " << i.level << std::endl;

		// Hack: update root 0 level with maxLevel count, because we don't update this
		// when growing root level size
		i.height = maxLevel;
			
		int counter = 0;
		bool done = false;
		
		while(!done)
		{
			std::cout << counter;
			
			for(int l = i.height; l >= 0; --l)
			{
				std::cout << " | ";
			}
			std::cout << " " << i.value << std::endl;
			
			counter ++;

   			if( i.next )
   			{
				node<T> *ii = i.next;
			   	i = *ii;
			}
			else
			{
				done = true;
			}
		}
	
		std::cout << "----------------------" << std::endl;
	}
};