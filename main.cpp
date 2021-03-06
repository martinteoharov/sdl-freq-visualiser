#include "visualization.h"
#define FILE_PATH "audiosamples/deafkev.wav" //https://www.youtube.com/watch?v=qNf9nzvnd1k
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
     0  :     0  *  44100 /  4096  =        0.0 Hz
     1  :     1  *  44100 /  4096  =        21.5 Hz
     2  :     2  *  44100 /  4096  =        43 Hz
     3  :     3  *  44100 /  4096  =        64.5 Hz
     4  :     ...
     5  :     ...

   4096 :    4096 * 44100 /  4096  =        22.05 kHz
*/

SDL *visualization = nullptr;

Uint8* sampData;
SDL_AudioSpec wavSpec;
SDL_AudioSpec obtained;
Uint8* wavStart;
Uint32 wavLength;
SDL_AudioDeviceID aDevice;
int magnitude[4096], last_magnitude[4096];
double arrSamples[4096];
int cnt = 0;
int pos;
bool first = true;
int total_magnitude = 0;
std::string filepath;


struct AudioData {
	Uint8* filePosition;
	Uint32 fileLength;
};

void PlayAudioCallback(void* userData, Uint8* stream, int streamLength) {
	AudioData* audio = (AudioData*)userData;
	sampData = (Uint8*)stream;
	fftw_complex x[4096], y[4096];

	if (audio->fileLength == 0) {
		return;
	}

	Uint32 length = (Uint32)streamLength;
	length = (length > audio->fileLength ? audio->fileLength : length);
	if( visualization->left() ){
		audio->filePosition -= 20*length;
		audio->fileLength += 20*length;
	}
	if( visualization->right() ){
		audio->filePosition += 20*length;
		audio->fileLength -= 20*length;
	}
	if( visualization->isMouseClicked() ){
		if( visualization->getX() > 75 && visualization->getX() < 700 && visualization->getY() > 95 && visualization->getY() < 120 ){
			std::cout << visualization->getX() << " " << visualization->getY() << std::endl << std::flush;
		}
	}
	pos = audio->fileLength;
	//std::cout << pos << std::flush << std::endl;
	std::vector<double> samples (stream, stream + length);
	//std::cout << "vector size =" << samples.size() << std::endl;

	for( int i = 0; i < 4096; i ++ ){
	//	double multiplier = (1 - cos((2*PI*i)/4096));
	//	x[i][REAL] = multiplier * (double)samples[i];
		x[i][REAL] = (double)samples[i];
		x[i][IMAG] = 0.0;
	//	std::cout << i << " - " << x[i][REAL] << std::endl << std::flush;
	}

	fftw_plan plan = fftw_plan_dft_1d( 4096, x, y,  FFTW_FORWARD, FFTW_ESTIMATE );
	fftw_execute(plan);

	for( int i = 0; i < 4096; i ++ ){
		if( y[i][IMAG] < 0 ){
			magnitude[i] = sqrt( y[i][REAL] * y[i][REAL] + y[i][IMAG] * y[i][IMAG] );
		}
		else{
			magnitude[i] = sqrt( y[i][REAL] * y[i][REAL] + y[i][IMAG] * y[i][IMAG] );
		}
	}
	if( first ){
		for( int i = 0; i < 4096; i ++ ){
			last_magnitude[i] = magnitude[i];
		}
		first = false;
	}

//	SDL_memcpy(&in, sampData, sizeof(sampData));
	SDL_memcpy(stream, audio->filePosition, length);

	//std::cout << magnitude[0] << " -- " << magnitude[1] << " -- " << magnitude[2]<< std::endl;
	
	for( int i = 10; i < 1000; i ++ ){
		total_magnitude += magnitude[i];	
	}
	if( total_magnitude > 3000000 ){
		std::cout << total_magnitude << std::endl;
	}
	total_magnitude = 0;

	audio->filePosition += length;
	audio->fileLength -= length;

}

int main( int argc, char *argv[] ) {

	visualization = new SDL();
	visualization -> init( "", 100, 300, 1300, 400, false );
	SDL_Init(SDL_INIT_AUDIO);
	filepath = argv[1];


	// | O_NOCTTY
	int open_result = open("/dev/ttyUSB0", O_RDWR );

	if( open_result == 0 ){
		std::cout << "Port Opened Successfully" << std::endl;
	}
	else{
		printf("Port Failed to Open: %s.\n", strerror(errno));
	}

	if (SDL_LoadWAV(argv[1], &wavSpec, &wavStart, &wavLength) == NULL) {
		std::cerr << "Couldnt load file: " << argv[0] << std::endl;
		getchar();
	}
	/*
	if (SDL_LoadWAV(FILE_PATH, &wavSpec, &wavStart, &wavLength) == NULL) {
		std::cerr << "Couldnt load file: " << FILE_PATH << std::endl;
		getchar();
	}
	*/
	std::cout << "Loaded " << argv[1] << std::endl;

	AudioData audio;
	audio.filePosition = wavStart;
	audio.fileLength = wavLength;
	wavSpec.samples = 1024;

	wavSpec.callback = PlayAudioCallback;
	wavSpec.userdata = &audio;


	aDevice = SDL_OpenAudioDevice( NULL, 0, &wavSpec, &obtained, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (aDevice == 0) {
		std::cerr << "Audio Device connection failed: " << SDL_GetError() << std::endl;
		getchar();
	}
	SDL_PauseAudioDevice(aDevice, 0);

	std::cout << obtained.samples << std::endl << std::flush;

	while( visualization -> running () ){
		visualization -> handleEvents();
		visualization -> render();
		cnt ++;
		visualization -> update(magnitude, cnt, pos);
		//std::cout << " - " << cnt;
		//std::cout << std::endl;
	}

	visualization->clean();

	return 0;
}

