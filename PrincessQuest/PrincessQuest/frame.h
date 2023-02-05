#ifndef FRAME
#define FRAME

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "Globale.h"
#include "groupBuilder.h"

using namespace std;

class Frame {
public:
	int frameNummer{};
	SDL_Rect clip{}; //wählt region von den png files (Monster.png/ Princess.png)
	float duration{}; //Zeitsdauer des Frames
	SDL_Point offSet{}; //Ankerpunkt für den einzelnen Frame

	list<Group*> frameData; 

	void Draw(SDL_Texture* Skizze, float x, float y);

	void loadFrame(ifstream& file, list<DataGroupType> &groupTypes);
	


};









#endif 

