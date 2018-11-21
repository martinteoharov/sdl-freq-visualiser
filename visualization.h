#ifndef visualization_h
#define visualization_h
#define REAL 0
#define IMAG 1

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <X11/Xlib.h>
#include <fftw3.h>

class SDL {
	public:
		SDL();
		~SDL();

		void init( const char* title, int x, int y, int width, int height, bool fullscreen );

		void handleEvents();
		void update(int bin);
		void render();
		void clean();

		bool running(){ return isRunning; }

	private:
		bool isRunning;
		SDL_Window *window;
		SDL_Renderer *renderer;
		Display* d = XOpenDisplay(NULL);
		Screen*  s = DefaultScreenOfDisplay(d);
		SDL_Rect rect[65];
		int xlen = 75;

};

#endif

SDL::SDL(){
}
SDL::~SDL(){
}

void SDL::init( const char* title, int x, int y, int width, int height, bool fullscreen ){
	int flags = 0;
	if( fullscreen ){
		std::cout << "Fullscreen mode: enabled" << std::endl;
		std::cout << "Window resolution: " << s->width << "x" << s->height<<std::endl;
		width = s->width;
		height = s->height;
	}
	else{
		std::cout << "Fullscreen mode: disabled" << std::endl;
		std::cout << "Window resolution: " << width << "x" << height<<std::endl;
	
	}

	if( SDL_Init( SDL_INIT_EVERYTHING ) == 0 ){
		std::cout << "Subsystems Initialised!..." << std::endl;
		window = SDL_CreateWindow ( title, x, y, width, height, flags );
		if( window ){
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer( window, -1, 0 );

		if( renderer ){
			SDL_SetRenderDrawColor( renderer, 10, 50, 50, 0 );
			std::cout << "Renderer created!" << std::endl;
			//starting x position of each rect
			for( int i = 0; i < 65; i ++ ){
				rect[i].x = xlen;
				rect[i].y = 310;
				rect[i].w = 8;
				rect[i].h = -20;
				xlen += 10;
			}
		}

		isRunning = true;
	}
	else{
		isRunning = false;
	}
}
void SDL::handleEvents(){
	SDL_Event e;
	SDL_PollEvent( &e );
	switch (e.type){
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

void SDL::update(int bin){
	bin /= 4;
	rect[bin].h = -100;
	for( int i = 0; i < 65; i ++ ){
		if( rect[i].h < -20 ){
			rect[i].h ++;
		}
	}
}


void SDL::render(){
	SDL_RenderClear( renderer );
	//stuff to render
	SDL_SetRenderDrawColor( renderer, 10, 50, 50, 0 );
	//draws every rect
	for( int i = 0; i < 65; i ++ ){
		SDL_RenderFillRect( renderer, &rect[i] );
	}
	SDL_SetRenderDrawColor( renderer, 10, 20, 30, 0 );
	SDL_RenderPresent( renderer );
}

void SDL::clean(){
	SDL_DestroyWindow( window );
	SDL_DestroyRenderer( renderer );
	SDL_Quit();
	std::cout << std::endl << "Window cleaned.. Renderer cleaned..Exiting.." << std::endl;
}

