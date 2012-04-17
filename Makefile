
all:
	g++ -o anneal anneal.cpp -I.. -Wall -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
#	g++ -o anneal anneal.cpp -I.. -Wall -W -lm -ansi -pedantic -O3 -fno-tree-pre -Dcimg_use_vt100 -I/usr/X11R6/include -L/usr/X11R6/lib -lpthread -lX11
