#ifndef LEBEENTITY
#define LEBEENTITY

#include "bigentity.h"


//Abstrakte Klasse

class LebeEntity : public bigentity {
public:


	int Lebenspunkte, LebenspunkteMax{};
	int Schaden = 0;
	SDL_Rect hitBox{}; //beschreibt den SchadenBereich

	float invincibleTimer = 0; // if < 0, kein Schaden möglich 

	virtual void updateHitbox();
	virtual void updateSchaeden() = 0; 
	virtual void sterben() = 0; 
	virtual void updateInvincibleTimer();

	void zeichnen();



};





#endif 
