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

const int visualBuffers = 90;

class SDL {
	public:
		SDL();
		~SDL();

		void init( const char* title, int x, int y, int width, int height, bool fullscreen );

		void handleEvents();
		void update(int a, int b);
		void render();
		void clean();

		bool running(){ return isRunning; }

	private:
		bool isRunning;
		SDL_Window *window;
		SDL_Renderer *renderer;
		Display* d = XOpenDisplay(NULL);
		Screen*  s = DefaultScreenOfDisplay(d);
		SDL_Rect rect[visualBuffers];
		SDL_Rect reverseRect[visualBuffers];
		int xlen = 75;

};

#endif
