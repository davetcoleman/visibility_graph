#include "CImg.h"// Include CImg library header.
#include <iostream>

using namespace cimg_library;
using namespace std;

#undef min
#undef max

//-------------------------------------------------------------------------------
// Matrix Structure
//-------------------------------------------------------------------------------
class matrix
{
public:
	double * data;
	int rows;
	int cols;
	bool isInitialized;
	
	//----------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------	
	matrix(int row = 0, int col = 0)
	{
		isInitialized = false;

		// initialize it if chosen to
		if(row > 0)
			init(row, col);
	}
	//----------------------------------------------------------------------------
	// Deconstructor
	//----------------------------------------------------------------------------	
	~matrix()
	{
		// clear array
		if(isInitialized)
			delete data;
	}	
	//----------------------------------------------------------------------------
	// Allocate memory and save matrix size
	//----------------------------------------------------------------------------	
	void init( int row, int col = 0)
	{
		if( !col ) // Assume is square
			col = row;
		
		rows = row;
		cols = col;
		data = new double[ rows * cols];

		isInitialized = true;
	}
	//----------------------------------------------------------------------------
	// Allow array to work just like 2d matrix:
	//----------------------------------------------------------------------------	
	double operator() ( int row, int col )
	{
		return data[row * cols + col];
	}
	//----------------------------------------------------------------------------	
	// Copy a matrix using equal sign
	//----------------------------------------------------------------------------	
	matrix& operator=(const matrix& old )
	{
		init( old.rows, old.cols );
		
		// Copy over old data
		for( int i = 0; i < old.rows * old.cols; ++i)
			data[i] = old.data[i];
		
		return *this;
	}
	
	/*  double & operator() (const int row, const int col);
		friend std::ostream& operator<<(std::ostream&, const Matrix&);*/
	//----------------------------------------------------------------------------
	// Debugging Function
	//----------------------------------------------------------------------------
	void print()
	{
		cout << endl << "Matrix with " << rows << " rows, " << cols << " cols." << endl;
	
		for(int i = 0; i < rows; ++i)
		{
			for(int j = 0; j < cols; ++j)
			{
				cout << data[i*cols + j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	//----------------------------------------------------------------------------
	// Set matrix to zeros
	//----------------------------------------------------------------------------
	void zero()
	{
		for(int i = 0; i < rows*cols; ++i)
		{
			data[i] = 0;
		}
	}
	//----------------------------------------------------------------------------
	// Length of data array
   	//----------------------------------------------------------------------------
	int length()
	{
		return rows*cols;
	}
};
//-------------------------------------------------------------------------------
// Compute Energy (run once)
//-------------------------------------------------------------------------------
int compute_energy(matrix & S)
{
	bool verbose = false;
	int n = S.cols; // or rows, whichever

   	// Find energy score:
	int E = 0;
	int top, bottom, left, right;
	for(int x = 0; x < n; ++x) // loop through each site
	{
		for(int y = 0; y < n; ++y) // loop through each site
		{
			// find neighbor locations
			top = (y - 1 + n ) % n;
			bottom = (y + 1) % n;
			left = (x - 1 + n) % n;
			right = (x + 1) % n;

			// multiply to find energy for this particular site
			E += S(x,y) * (S(x,top) + S(x,bottom) + S(left,y) + S(right,y));

			if(verbose)
				cout << "Site x = " << x << " y = " << y << " has energy e = "
					 << S(x,y) * (S(x,top) + S(x,bottom) + S(left,y) + S(right,y)) << endl;
		}
	}

	return -E;
}
//-------------------------------------------------------------------------------
// Annealing Schedule - Temperatue
// returns t' < t input. is called cooling or annealing schedule.
//-------------------------------------------------------------------------------
double temperature(double t, double alpha)
{
	return alpha * t;
}
//-------------------------------------------------------------------------------
// Single-Flip Neighbor 
// use the single-flip neighbor function
// returns the new energy
//-------------------------------------------------------------------------------
int neighbor( matrix &S, matrix &newS, int oldE )
{
	
	// Copy current state S to new state newS
	for(int i = 0; i < S.length(); ++i)
		newS.data[i] = S.data[i];

	// Choose two random coordinates
	int x = rand() % S.rows;
	int y = rand() % S.cols;
	
	//cout << "CHANGING " << x << ", " << y << endl;
	
	// Flip between 1 and -1
	if( newS.data[x * S.cols + y] == -1 )
		newS.data[x * S.cols + y] = 1;
	else
		newS.data[x * S.cols + y] = -1;

	// Calculate delta E

	// find neighbor locations
	int n = S.rows;
	int top = (y - 1 + n ) % n;
	int bottom = (y + 1) % n;
	int left = (x - 1 + n) % n;
	int right = (x + 1) % n;

	
	// Multiply to find energy for this particular site
	int deltaE = -4*newS(x,y) * (    newS(x,top) + newS(x,bottom) + newS(left,y) + newS(right,y)     );

	return oldE + deltaE; // new energy amount
}
//-------------------------------------------------------------------------------
// Use the Metropolis accept() function adapted to prefer decreases in energy rather than increases
// takes as input the scores of 2 states and a parameter T (temperature) and makes a probabilistic decision about
// whether to accept the proposed move
//-------------------------------------------------------------------------------
double accept( int E, int newE, double T )
{
	bool verbose = false;
	// satisfies the detailed balance criterion for MCMC and maximized acceptance probability...
	// adapted to prefer decreases in energy

	if(verbose)
	{
		cout << endl << "Accept Function:" << endl;
		cout << "\tE: " << E << "\n\tnewE: " << newE << "\n\tT: " << T << endl;
	}
		
	if( newE < E )
	{
		if(verbose)	
			cout << "\t ACCEPTED IN IF" << endl;
		return 1;		
	}
    else
	{
		if(verbose)	
			cout << "\taccept in else" << endl;
		return exp( -1 * abs( E - newE ) / T );
	}
}
//-------------------------------------------------------------------------------
// Probabilistic Random Flip
//-------------------------------------------------------------------------------
bool nextBool(double probability)
{
	return rand() <  probability * RAND_MAX;
}
//-------------------------------------------------------------------------------
// The Simulate Anneal Function
//-------------------------------------------------------------------------------
int simulate_anneal(matrix &S, double T0, double alpha)
{
	bool verbose = false;
	
	// Problem size
	int n = S.rows;
	
	// Current state and energy
	if(verbose)
	{
		cout << "Initial state:";
		S.print();
	}
	
	int E = compute_energy( S );
	cout << "Original energy E = " << E << endl;

	// Visual components:
	int screen_size = 600;
	if( screen_size % n )
	{
		cout << "Error: screen size must be evenly divisible by problem size!" << endl;
		return EXIT_FAILURE;
	}	
	int scale = screen_size / n; // visualize n as bigger
	CImg<unsigned char> img(screen_size,screen_size,1,1,0);

	CImgDisplay disp(img, "Simulated Annealing");      // Display the modified image on the screen
	
	cout << "-------------------------- " << endl;
		
	// Temporary holder for new solution:
	matrix newS(S.rows);
	int newE;
	
	// Create matrix to store best solution
	matrix bestS(S.rows,S.cols);
	bestS = S; // copy matrix
	int bestE = E;
	
	double T = T0; // holds temperature for each step

	// Used for defining convergence
	int rejectedProposals = 0;
	int maxRejectedProposals = 10*S.rows * S.cols;

	// Used for visualization/speed:
	int count = 0; // used for displaying at intervals
	int whitePercent; // used to calculate if 80% of sites have same state
	bool hasSaved = false;
	int paintInterval = 50000;
    double displayT = 200; // when to start doing IO
	bool stopFlag = false;
	int k = 0; // energy evaluation count

	unsigned char grey[] = { 200 };      // Define a color	
	unsigned char black[] = { 0 };      // Define a color					
	int color; // cache for looping

	double prob; // recieve value from accept function
	
	while( rejectedProposals < maxRejectedProposals && !stopFlag)
	{
		if(verbose)
			cout << "WHILE iteration " << k << ", rejected = "
				 << rejectedProposals << " of " << maxRejectedProposals << endl;
		
		T = temperature( T, alpha ); // temp calculation

		newE = neighbor( S, newS, E ); // pick some neighbor

		if(T < displayT && count > paintInterval)
		{
			cout << "T: " << T
				 << "\t E: " << newE << endl;			
		}
		
		if(verbose)
		{
			cout << "New neighbor:";
			newS.print();
		}
		
		// Should we move to new state?
		prob = accept( E, newE, T );

		if( nextBool(prob) ) 
		{
			if(verbose)
				cout << "!!! Trading for new S" << endl;
			S = newS;
			E = newE;

			rejectedProposals = 0;

			if(  (T < displayT && count > paintInterval)  ||
				 k == 1 )
			{
				count = 0;
				whitePercent = 0;

				// Convert problem to visualization
				
				for(int x = 0; x < n; ++x)
				{
					for(int y = 0; y < n; ++y)
					{
						if(S(x,y) == 1) // white
						{
							color = 255;
							whitePercent ++;
						}
						else
						{
							color = 0;
						}							
						// Fill in square with color
						for(int i = x*scale; i < x*scale+scale; ++i)
						{
							for(int j = y*scale; j < y*scale+scale; ++j)
							{
								img(j,i,0) = color;
							}
						}					
					}
				}

				// Calculate white percent
				whitePercent = whitePercent * 100.0 / ( n * n );
				cout << "White percent = " << whitePercent << endl;
				
				img.draw_text(0,0,"Temperature: %e",black,grey,1,13,T);
				img.draw_text(0,13,"Energy: %d",black,grey,1,13,E);
				if( whitePercent > 50 )
					img.draw_text(0,26,"White: %d/100",black,grey,1,13,whitePercent);
				else
					img.draw_text(0,26,"Black: %d/100",black,grey,1,13,100-whitePercent);
				
				// Update the window
				disp.display(img);
				//usleep(1*1000);


				// Save files as needed
				if( k == 1) // initial configuration
				{
					img.save("initial.png"); // save the image
				}
				else if( (whitePercent >= 80 || whitePercent <= 20) && !hasSaved  )
				{
					hasSaved = true;
					img.save("80percent.png"); // save the image
					cout << "SAVED" << endl;
					//sleep(3);
				}
			}
			else
			{
				count ++;
			}
		}
		else
		{
			if(verbose)
				cout << "!!! REJECTED " << endl;
			rejectedProposals++;

		}
		
		// Is this a new best?
		if( E < bestE )
		{
			cout << "New best =" << bestE << endl;
			
			//			bestS = newS;
			bestE = newE;
		}

		++k; // track how many intervals run

		if(verbose)
			cout << endl << "--------------------------" << endl;
		
		// End early if needed
		// Check if the user want to quit the explorer.
		if (disp.is_closed() || disp.is_keyQ() || disp.is_keyESC())
			stopFlag = true;		
	}
	cout << "Iterations = " << k << endl;
	cout << rejectedProposals << " rejected of " << maxRejectedProposals << endl;
	
	//Now continue to display until user presses ESC
	// Convert problem to visualization
	for(int x = 0; x < n; ++x)
	{
		for(int y = 0; y < n; ++y)
		{
			if(S(x,y) == 1)
			{
				color = 255;
			}
			else
			{
				color = 0;
			}			
			// Fill in square with color
			for(int i = x*scale; i < x*scale+scale; ++i)
			{
				for(int j = y*scale; j < y*scale+scale; ++j)
				{
					img(j,i,0) = color;

				}
			}
		}
	}
	
	// Update the window
	disp.display(img);
	img.save("final.png"); // save the image
		
	bool stopflag = false;
	while(!stopflag)
	{
		// Check if the user want to quit the explorer.
		if (disp.is_closed() || disp.is_keyQ() || disp.is_keyESC())
			stopflag = true;
		usleep(250*1000);
	}

	// Return the best solution state found, not the last one. These are usually the same:
	S = bestS;
	
	return bestE; // return best energy found
}

//-------------------------------------------------------------------------------
//  Main procedure
//-------------------------------------------------------------------------------
int main()
{
	// Problem data:
	int n = 50; // problem size
	double t0 = 1000; // initial temperature
	double alpha = 1 - pow(10.0,-5); // annealing parameter. when a = 0 schedule is very aggressive,
                                     // when a = 1 schedule is gentle and explores more of S
	cout << "Alpha = " << alpha << endl;
	sleep(2);
	
	//	double alpha = 1 - pow(10.0,-4); 
	
	matrix S(n,n); // create the matrix solution
	int E_stop;
	
	// Seed Random Numbers
	srand( time(NULL) ); 

	cout << endl << endl << "Simulated Annealing by Dave Coleman ------- " << endl << endl;

	// Initialize the solution randomly
	for(int x = 0; x < n; ++x)
	{
		for(int y = 0; y < n; ++y)
		{
			if( rand() % 2 )			
				S.data[x*n + y] = -1;
			else
				S.data[x*n + y] = 1;
		}
	}

	// Run simulated annealing
	E_stop = simulate_anneal(S, t0, alpha);

	// Output final state
	cout << endl << "RESULT:";
	S.print();	
	cout << endl << endl;
	cout << "************************" << endl;
	cout << "Annealing complete." << endl;
	cout << "Final E = " << E_stop << endl;
	cout << "************************" << endl;
	
	cout << endl;
	
	return EXIT_SUCCESS;
}
