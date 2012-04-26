#include "CImg.h"// Include CImg library header.
#include <iostream>

using namespace cimg_library;
using namespace std;

#undef min
#undef max

const unsigned char white[] = { 255 };
const unsigned char grey[] = { 100 };
const int screen_size = 500;

//-------------------------------------------------------------------------------
//  Main procedure
//-------------------------------------------------------------------------------
int main()
{
	cout << endl << endl << "Visibility Graph by Dave Coleman ------- " << endl << endl;

	CImg<unsigned char> img(screen_size,screen_size,1,1,0);
	img.fill(20);
	
	// Line segments:
	int seg = 8;
	int segs[][4] =
		{ {10,200,100,215},
		  {50,50,50,100},
		  {100,100,300,90},
		  {100,300,110,490},
		  {350,350,350,450},
		  {400,400,450,200},
		  {200,450,300,450},
		  {250,50,450,100}
		};

	// Draw segments:
	for(int i = 0; i < seg; ++i)
	{
		img.draw_line(segs[i][0],segs[i][1],segs[i][2],segs[i][3],white);
		img.draw_circle(segs[i][0],segs[i][1],2,white);
		img.draw_circle(segs[i][2],segs[i][3],2,white);		
	}

	// Draw sweeper:
	img.draw_circle(screen_size / 2, screen_size / 2, 4, grey);
	img.draw_line(screen_size / 2, screen_size / 2, 450, 250, grey);

	// Sort the verticies:
	

	// Show window until user input:
	//disp.display(img);
	CImgDisplay disp(img, "Visibility Graph");      // Display the modified image on the screen	
	while (!disp.is_closed()) {
		if (disp.is_keyESC() )
			break;
		disp.wait();
	}
	
	return EXIT_SUCCESS;
}
