#ifndef visualization_h
#define visualization_h
#define REAL 0
#define IMAG 1
#define PI 3.14159265359

#include <SDL2/SDL.h>
#include <iostream>
#include <X11/Xlib.h>
#include <fftw3.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>


const int visualBuffers = 1200;
const int midRangeBuffer = 150;

class SDL {
	public:
		SDL();
		~SDL();

		void init( const char* title, int x, int y, int width, int height, bool fullscreen );

		void handleEvents();
		void update(int a[4096], int b, int c);
		void render();
		void clean();
		std::string key;
		bool left(){ return isLeft; }
		bool right(){ return isRight; }
		bool isMouseClicked(){ return mouseClicked; }
		int getX(){ return xcoord; }
		int getY(){ return ycoord; }


		bool running(){ return isRunning; }

	private:
		bool isRunning;
		bool isLeft;
		bool isRight;
		bool mouseClicked;
		int biggest = 0;
		int xcoord, ycoord;
		SDL_Window *window;
		SDL_Renderer *renderer;
		Display* d = XOpenDisplay(NULL);
		Screen*  s = DefaultScreenOfDisplay(d);
		SDL_Rect rect[visualBuffers];
		SDL_Rect reverseRect[visualBuffers];
		SDL_Rect midRangeRect[midRangeBuffer];
		SDL_Rect audioPos;
		SDL_Rect audioStart, audioEnd, audioPath;
		int xlen = 0;
		int startPos;

};

#endif
