#ifndef ANIMATIONSET
#define ANIMATIONSET

#include "Animation.h"

using namespace std;

class AnimationSet {
public:
	string imageName;
	SDL_Texture* Skizze; //Prinzessin, Monster 
	SDL_Texture* weißeSkizze = nullptr; //für damage der Charaktere
	list<Animation> animationen;

	~AnimationSet(); 

	Animation* getAnimation(string name);


	void ladeAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey = false, int transparantPixelIndex = 0, bool createWhiteTexture = false);

};

#endif 