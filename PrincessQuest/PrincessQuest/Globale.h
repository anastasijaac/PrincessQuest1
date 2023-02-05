#ifndef GLOBALE
#define GLOBALE

#include <string>
#include <iostream>
#include <SDL.h>
#include "randomNumber.h"

using namespace std;

class Globale {
public:

	static const float PI;

	static bool debugging;

	//SDL
	static int ScreenWidth, ScreenHeight, ScreenScale;
	static SDL_Renderer* renderer;

	//schneidet den header von fdest dateien
	static string clipOffDataHeader(string data);

};

#endif 

