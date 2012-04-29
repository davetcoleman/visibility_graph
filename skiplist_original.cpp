/* Skip List
   CSCI 5454 Algorithms
   Dave Coleman | david.t.coleman@colorado.edu

   2/2/2012

   Implementation of Skip Lists
*/

#include <math.h>
#include <iostream>
#include <cstdlib>

using namespace std;

//-------------------------------------------------------------------------------------------
// Node Structure
//-------------------------------------------------------------------------------------------
struct node{
	double value;
	node *below; // node below in tower
	node *next; // next node in skip list
	int level; // level of this current node
	int height; // full number of levels in tower
};
	
//-------------------------------------------------------------------------------------------
// Prototypes
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
// Skip List Class
//-------------------------------------------------------------------------------------------
class skiplist{
   	// used for testing
public:
	double find_op;
	double add_op;
    double remove_op;
	int maxLevel;
	
private:
	node *root;

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
	node* createNode(double value, int level, int height)
	{
		// Check if we are below level 0
		if(level < 0)
		{
			return NULL;
		}
		else // make a new node below
		{
			node *newNode = new node();			
			(*newNode).value = value;
			(*newNode).level = level;
			(*newNode).next = NULL;
			(*newNode).below = createNode(value, level - 1, height);
			(*newNode).height = height;
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
	void add(double x)
	{
		// Special Cases -------------------------------------------------------------------

		if(!root) // no root has been established yet
		{
			root = createNode(x, 0, 0);
			return;
		}

		if( (*root).value > x ) // new value goes before root
		{
			double temp_x = (*root).value;
			node *n = root;

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
			node *newRoot = new node();
			(*newRoot).value = (*root).value;
			(*newRoot).next = NULL;
			(*newRoot).below = root;
			(*newRoot).level = maxLevel;
			root = newRoot;
		}

		// Create the new node
		node *newNode = createNode(x, level, level);
					
		// Now add the node to the list
		bool found = false;
		node *i = root; 
		node *prevNode = NULL; // used for inserting between two nodes

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
		node *i = root;

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

			// Always move the i node pointer one level down:
			i = (*i).below;			
		}	

		return false;
	}
	//-------------------------------------------------------------------------------------------
	// REMOVE
	//-------------------------------------------------------------------------------------------
	bool remove(double x)
	{
		node *i = root;

		// Special case: remove root --------------------------------------------------------
		if( (*root).value == x)
		{
			// Get level 0 of root
			for(int l = (*root).level; l > 0; --l)
			{
				remove_op += 1;
				//cout << "Level " << l << endl;
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
			double root_x = (*root).value;
			node *n = root;
			node *nextNode = (*i).next;

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
		cout << endl << "LIST -----------------" << endl;

		// Special case: skiplist is empty
		if( !root )
		{
			cout << "----------------------" << endl;
			return;
		}

		node i = *root;
		
		// Get level 0 of root
		for(int l = (*root).level; l > 0; --l)
		{
			//cout << "Level " << l << endl;
			i = *(i.below);
		}
		//cout << "we are on level " << i.level << endl;

		// Hack: update root 0 level with maxLevel count, because we don't update this
		// when growing root level size
		i.height = maxLevel;
			
		int counter = 0;
		bool done = false;
		
		while(!done)
		{
			cout << counter;
			
			for(int l = i.height; l >= 0; --l)
			{
				cout << " | ";
			}
			cout << " " << i.value << endl;
			
			counter ++;

   			if( i.next )
   			{
				node *ii = i.next;
			   	i = *ii;
			}
			else
			{
				done = true;
			}
		}
	
		cout << "----------------------" << endl;
	}
};

//-------------------------------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------------------------------
int main(int argc, char ** argv)
{
	cout << endl << endl << endl << "Skiplist Implementation by Dave Coleman --------------- " << endl;

	if(true)
	{
		cout << endl << "Average atomic operation results" << endl << endl;
		cout << "Factor Adding Finding Removing" << endl;
	
		for(double factor = 1; factor <= 5; factor += .5)
		{		
			int n = pow(10,factor); // default input size
			int t = 120; // number of tests to run
			int t_run; // number of tests run so far

			// keep a running average of the operations for each type
			double total_add_op = 0;
			double total_find_op = 0;
			double total_remove_op = 0;

			// find a unique random number
			int random;

			// store the numbers we add
			double input_list[n];

			// Loop through test runs
			for(t_run = 1; t_run <= t; ++t_run)
			{
				skiplist list;

				for(int i = 0; i < n; ++i)
				{
					while(true)
					{
						random = rand() % (n*1000);
						if( !list.find(random) )
						{
							input_list[i] = random;
							list.add(random);
							break;
						}
						//cout << "found non-unique " << endl;
					}
				}
				// reset find_op bc we were using it for inserting unique values
				list.find_op = 0;

			
				for(int i = 0; i < n; ++i)
					list.find(input_list[i]);

				for(int i = 0; i < n; ++i)
					list.remove(input_list[i]);

				// Average the number of operations
				total_add_op += list.add_op / n;
				total_find_op += list.find_op / n;
				total_remove_op += list.remove_op / n;	   
			}
	
			cout << n << " " << (total_add_op/t_run) << " " << (total_find_op/t_run);
			cout << " " << (total_remove_op/t_run) << ";" << endl;

		}
		cout << endl;
	}
	else if(false)
	{
		skiplist list;
		int random;
		int n = 100;
		for(int i = 0; i < n; ++i)
		{
			while(true)
			{
				random = rand() % (n*1000);
				if( !list.find(random) )
				{
					//cout << "Adding value " << random << endl;					
					list.add(random);
					break;
				}
				cout << "found non-unique " << endl;
			}
		}
	
		list.printAll();

		cout << "Max level is " << list.maxLevel << endl;	
	}
	else
	{	
		// UNIT TESTING -------------------------------------------------
		cout << "Unit Testing" << endl;
		skiplist list;
		
		list.add(10);
		list.add(13);
		list.add(11);
		list.add(12);	
		list.add(9);
		list.add(14);
		list.printAll();
		
		list.find_op = 0; // reset
		// Find
		if(!list.find(9))
			cout << "ERROR FIND 9" << endl;
		list.printAll();		
		cout << "FIND COUNT =" << list.find_op << endl;
		

		if(!list.find(13))
			cout << "ERROR FIND 13" << endl;
		list.printAll();

		cout << endl << "FIND 14" << endl;
		list.find_op = 0; // reset
		if(!list.find(14))
			cout << "ERROR FIND 14" << endl;
		list.printAll();
		cout << "FIND COUNT =" << list.find_op << endl;

				
		if(!list.find(10))
			cout << "ERROR FIND 10" << endl;
		list.printAll();

		// Remove
		if(!list.remove(9))
			cout << "ERROR REMOVE 9" << endl;
		if(list.find(9))
			cout << "ERROR FIND REMOVE 9" << endl;

		list.printAll();
	
		if(!list.remove(11))
			cout << "ERROR REMOVE 11" << endl;	
		if(list.find(11))
			cout << "ERROR FIND REMOVE 11" << endl;

		list.printAll();
		
		if(!list.remove(14))
			cout << "ERROR REMOVE 14" << endl;	
		if(list.find(14))
			cout << "ERROR FIND REMOVE 14" << endl;

		list.printAll();
	
		if(list.remove(1000))
			cout << "ERROR REMOVE RETURNED FALSE POSITIVE" << endl;

		list.printAll();
	
		if(!list.remove(12))
			cout << "ERROR REMOVE 12" << endl;

		list.printAll();
	
		if(!list.remove(13))
			cout << "ERROR REMOVE 13" << endl;

		list.printAll();
	
		if(!list.remove(10))
			cout << "ERROR REMOVE 10" << endl;

		list.printAll();
	
		if(list.find(10))
			cout << "ERROR FIND REMOVE 10" << endl;	
	  
		list.printAll();
	}

	
	
	return EXIT_SUCCESS;
}


