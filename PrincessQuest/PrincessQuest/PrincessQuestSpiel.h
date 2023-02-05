#ifndef PRINCESSQUESTSPIEL
#define PRINCESSQUESTSPIEL
#include "Globale.h"
#include "Princess.h"
#include "Wand.h"
#include "Monster.h"
#include "TastaturEingabe.h"
#include "drawing_functions.h"


class PrincessQuestSpiel {
public:

	AnimationSet* PrincessAnimSet;
	AnimationSet* MonsterAnimSet;
	AnimationSet* WandAnimSet;

	SDL_Texture* Hintergrund;
	SDL_Texture* StartBild;
	SDL_Texture* Overlay;
	

	SDL_Texture* PunktZahlTexture = nullptr;

	Princess *princess;
	TastaturEingabe princessEingabe;

	bool ZeigeStartBild;
	float OverlayDauer;
	
	list<bigentity*> feinde;
	list<bigentity*> waende;

	PrincessQuestSpiel();
	~PrincessQuestSpiel();

	void update();
	void zeichnen();
};

#endif 

