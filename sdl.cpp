#include "visualization.h"

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
			for( int i = 0; i < visualBuffers; i ++ ){
				rect[i].x = xlen;
				rect[i].y = 310;
				rect[i].w = 10;
				rect[i].h = -100;
				xlen += 11;
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

void SDL::update( int max_magnitude_index, int cnt){
	if( rect[max_magnitude_index].h > -100 ){
		rect[max_magnitude_index].h -= 1;
	}
	if( cnt % 10 == 0 ){
		for( int i = 0; i < visualBuffers; i ++ ){
			if( rect[i].h < 10 ){
				rect[i].h ++;
			}
		}
	}
}


void SDL::render(){
	SDL_RenderClear( renderer );
	//stuff to render
	SDL_SetRenderDrawColor( renderer, 10, 50, 50, 0 );
	//draws every rect
	for( int i = 0; i < visualBuffers; i ++ ){
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

