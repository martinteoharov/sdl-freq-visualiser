hellomake: main.cpp sdl.cpp
	g++ -o visualization main.cpp sdl.cpp -lSDL2 -lX11 -lfftw3
