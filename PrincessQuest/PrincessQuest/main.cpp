
#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "SDL_mixer.h"
#include "Globale.h"
#include "PrincessQuestSpiel.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char** agv) {
	srand(static_cast<unsigned int>(time(nullptr)));

	
	//SETUP SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Error initialising SDL" << endl;
		return 1;
	}

	//SETUP WINDOW
	SDL_Window* window = SDL_CreateWindow("Princess Quest",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Globale::ScreenWidth*Globale::ScreenScale, Globale::ScreenHeight*Globale::ScreenScale, SDL_WINDOW_SHOWN );
	if (window == nullptr) {
		SDL_Quit();
		cout << "Window Error" << endl;
		return 1;
	}

	//SETUP RENDERER
	Globale::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globale::renderer == nullptr) {
		cleanup(window);
		SDL_Quit();
		cout << "Renderer Error" << endl;
		return 1;
	}

	SDL_RenderSetLogicalSize(Globale::renderer, Globale::ScreenWidth, Globale::ScreenHeight);

	//initialisiert PNGs
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_Quit();
		cout << "SDL_IMAGE did not initialise" << endl;
		return 1;
	}

	//intialisiert text
	if (TTF_Init() != 0) {
		SDL_Quit();
		cout << "SDL_TTF did not initialise" << endl;
		return 1;
	}

	//initialise SDL_Mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_Quit();
		cout << "SDL_Mixer did not initialise" << endl;
		return 1;
	}

	PrincessQuestSpiel PQS;
	PQS.update();


	cleanup(window);
	cleanup(Globale::renderer);

	SDL_Quit();
	return 0;


}