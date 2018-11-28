#include "visualization.h"
#define FILE_PATH "audiosamples/test1.wav" //https://www.youtube.com/watch?v=qNf9nzvnd1k
#define PI 3.14159265359

/*
freq = i * Fs / N;      (1)
where,
freq = frequency in Hertz,
i = index (position of DFT output or can also think of it as representing the number of cycles)
Fs = sampling rate of audio,
N = size of FFT buffer or array.

To explain further, lets say that:

N = 4096          //a buffer that holds 4096 audio data samples
Fs = 44100       //a common sample rate [frames per sec] for audio signals: 44.1 kHz

The spectral bin numbers aka frequency bins using equation (1) from above would be:

    bin:      i      Fs         N            freq
     0  :     0  *  44100 /  2048  =        0.0 Hz
     1  :     1  *  44100 /  2048  =        21.5 Hz
     2  :     2  *  44100 /  2048  =        43 Hz
     3  :     3  *  44100 /  2048  =        64.5 Hz
     4  :     ...
     5  :     ...

   1024 :    1024 * 44100 /  2048  =        22.05 kHz
*/

SDL *visualization = nullptr;

Uint8* sampData;
SDL_AudioSpec wavSpec;
Uint8* wavStart;
Uint32 wavLength;
SDL_AudioDeviceID aDevice;
double arrSamples[4096];
double max_magnitude_index;


struct AudioData {
	Uint8* filePosition;
	Uint32 fileLength;
};

void PlayAudioCallback(void* userData, Uint8* stream, int streamLength) {
	AudioData* audio = (AudioData*)userData;
	sampData = (Uint8*)stream;
	double magnitude[4096];
	fftw_complex x[4096], y[4096];
	double max_magnitude = 0;

	if (audio->fileLength == 0) {
		return;
	}

	Uint32 length = (Uint32)streamLength;
	length = (length > audio->fileLength ? audio->fileLength : length);
	std::vector<double> samples (stream, stream + length);
	
	for( int i = 0; i < 4096; i ++ ){
		double multiplier = 0.5 * (1 - cos(2*PI*i/4096));
		x[i][REAL] = (double)samples[i];
		x[i][IMAG] = 0.0;
	}
	
	fftw_plan plan = fftw_plan_dft_1d( 4096, x, y,  FFTW_FORWARD, FFTW_ESTIMATE );
	fftw_execute(plan);

	for( int i = 0; i < 4096; i ++ ){
		if( y[i][IMAG] < 0 ){
		//	std::cout << y[i][REAL] << " - " << abs(y[i][IMAG]) << std::endl;
			magnitude[i] = sqrt( y[i][REAL] * y[i][REAL] + y[i][IMAG] * y[i][IMAG] );
		}
		else{
			//std::cout << y[i][REAL] << " + " << abs(y[i][IMAG])  << std::endl;
			magnitude[i] = sqrt( y[i][REAL] * y[i][REAL] + y[i][IMAG] * y[i][IMAG] );
		}
	}
	for( int i = 1; i < 4096; i ++ ){
		if( magnitude[i] > max_magnitude ){
			max_magnitude = magnitude[i];
			max_magnitude_index = i;
		}
	}
	int freq = max_magnitude_index * ( 44100 / 4096 );
	std::cout << freq << std::endl << std::flush;
	

//	SDL_memcpy(&in, sampData, sizeof(sampData));
	SDL_memcpy(stream, audio->filePosition, length);


	audio->filePosition += length;
	audio->fileLength -= length;

}

int main() {
	int cnt = 0;

	visualization = new SDL();
	visualization -> init( "asd", 100, 0, 800, 400, false );
	SDL_Init(SDL_INIT_AUDIO);

	if (SDL_LoadWAV(FILE_PATH, &wavSpec, &wavStart, &wavLength) == NULL) {
		std::cerr << "Couldnt load file: " << FILE_PATH << std::endl;
		getchar();
	}
	std::cout << "Loaded " << FILE_PATH << std::endl;

	AudioData audio;
	audio.filePosition = wavStart;
	audio.fileLength = wavLength;

	wavSpec.callback = PlayAudioCallback;
	wavSpec.userdata = &audio;
//	std::cout << wavSpec.samples;
//	SDL_Delay( 300 );


	aDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (aDevice == 0) {
		std::cerr << "Audio Device connection failed: " << SDL_GetError() << std::endl;
		getchar();
	}
	SDL_PauseAudioDevice(aDevice, 0);


	while( visualization -> running () ){
		visualization -> handleEvents();
		visualization -> render();
		cnt ++;
		visualization -> update(max_magnitude_index, cnt);
	}

	visualization->clean();

	return 0;
}

