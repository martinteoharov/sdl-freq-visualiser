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
				rect[i].w = 7.5;
				rect[i].h = -100;
				xlen += 8;
			}
			//audio position bar
			audioPos.y = 99;
			audioPos.h = 3;
			audioPos.w = 10;
			audioPos.x = 77;

			audioPath.x = 75;
			audioPath.w = 675;
			audioPath.h = 1;
			audioPath.y = 103;
			 
			audioStart.x = 73;
			audioStart.w = 2;
			audioStart.h = 6;
			audioStart.y = 97;

			audioEnd.x = 750;
			audioEnd.w = 2;
			audioEnd.h = 6;
			audioEnd.y = 97;
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
		case SDL_KEYDOWN:
			key = SDL_GetKeyName( e.key.keysym.sym);
			if( key == "Left" ){
				isLeft = true;
			}
			if( key == "Right" ){
				isRight = true;
			}
			break;
		default:
			break;
	}
}

void SDL::update( int max_magnitude_index, int cnt, int pos){
	if( rect[max_magnitude_index].h > -100 ){
		rect[max_magnitude_index].h -= 1;
	}
	if( cnt % 10 == 0 ){
		for( int i = 0; i < visualBuffers; i ++ ){
			if( rect[i].h < 5 ){
				rect[i].h ++;
			}
		}
	}
	if( cnt % 1000 == 0 ){
		isLeft = false;
		isRight = false;
	}
	if( cnt == 1 ){
		startPos = pos;
		audioPath.w = pos / 100000;
		audioEnd.x = pos / 100000 + 76;
	}
	audioPos.w = ( startPos - pos ) / 100000;
	std::cout << pos / 100000 << '\r' << std::flush;
}


void SDL::render(){
	SDL_RenderClear( renderer );
	//stuff to render
	SDL_SetRenderDrawColor( renderer, 10, 50, 50, 0 );
	//draws every rect
	for( int i = 0; i < visualBuffers; i ++ ){
		SDL_RenderFillRect( renderer, &rect[i] );
	}
	SDL_SetRenderDrawColor( renderer, 40, 80, 80, 0 );
	SDL_RenderFillRect( renderer, &audioPos );
	SDL_SetRenderDrawColor( renderer, 10, 80, 60, 0 );
	SDL_RenderFillRect( renderer, &audioStart );
	SDL_RenderFillRect( renderer, &audioEnd );
	SDL_RenderFillRect( renderer, &audioPath );

	SDL_SetRenderDrawColor( renderer, 10, 20, 30, 0 );
	SDL_RenderPresent( renderer );
}

void SDL::clean(){
	SDL_DestroyWindow( window );
	SDL_DestroyRenderer( renderer );
	SDL_Quit();
	std::cout << std::endl << "Window cleaned.. Renderer cleaned..Exiting.." << std::endl;
}

