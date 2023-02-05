#ifndef TASTATUREINGABE
#define TASTATUREINGABE

#include "Princess.h"


class TastaturEingabe {
public:
	Princess* princess; 
	SDL_Scancode WalkingUp, WalkingDown, WalkingLeft, WalkingRight;
	SDL_Scancode Slash, Dash;
	 
	TastaturEingabe();
	void kollision();
	void update(SDL_Event* event); //SDL wird immer aktuallisiert wenn Spiel gerunnt wird
};


#endif 

