#ifndef PRINCESS
#define PRINCESS

#include "Globale.h"
#include "LebeEntity.h"

class Princess : public LebeEntity {

public:
	//Bewegung
	static const string PRINCESS_ANIM_OBEN;
	static const string PRINCESS_ANIM_UNTEN;
	static const string PRINCESS_ANIM_LINKS;
	static const string PRINCESS_ANIM_RECHTS;

	//IDLE
	static const string PRINCESS_ANIM_IDLE_OBEN;
	static const string PRINCESS_ANIM_IDLE_UNTEN;
	static const string PRINCESS_ANIM_IDLE_LINKS;
	static const string PRINCESS_ANIM_IDLE_RECHTS;
	

	//SLASH-Angreifen
	static const string PRINCESS_SLASH_ANIM_LINKS;
	static const string PRINCESS_SLASH_ANIM_RECHTS;
	

	//Sterben
	static const string PRINCESS_ANIM_STERBEN;

	
		
	static const int PRINCESS_STATE_IDLE;
	static const int PRINCESS_STATE_BEWEGUNG;
	static const int PRINCESS_STATE_SLASH;
	static const int PRINCESS_STATE_STERBEN;


	Princess(AnimationSet* animSet);
	void update();
	void slash(bool check);
	void obenBewegung(bool check);
	void untenBewegung(bool check);
	void linksBewegung(bool check);
	void rechtsBewegung(bool check);
	void sterben();
	void beleben();
	void wechselAnimation(int neuerStatus, bool neustartFrameAnfang);
	void updateAnimation();
	void updateSchaeden();
};



#endif 

