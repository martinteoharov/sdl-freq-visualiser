# SDL-FREQUENCY-VISUALISER

Takes in an .wav, breaks it down, does a fourier transform to values and outputs bass frequency amplitudes (by default 20Hz - 1kHz, but can be customised for different values) onto the time domain

# Requirements

`sudo apt-get install libfftw3-dev libsdl2-dev libx11-dev g++ build-essential`

Use only 4096 sample rate .wav files as others may cause problems. 

# Build

To build and run simply go to the main directory and use `make && ./visualisation ./audiosample.wav
