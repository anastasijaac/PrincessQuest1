#include "AnimationSet.h"
#include "Globale.h"
#include <list>
#include <fstream>
using namespace std;

AnimationSet::~AnimationSet() {
	cleanup(Skizze);
	if (weiﬂeSkizze != nullptr)
		cleanup(weiﬂeSkizze);
}

Animation* AnimationSet::getAnimation(string name) {
	for (auto animationIterator = animationen.begin(); animationIterator != animationen.end(); animationIterator++) {
		Animation* anim = &(*animationIterator);
		if (name == anim->name) {
			return anim;
		}
	}
	return nullptr;
}

void AnimationSet::ladeAnimationSet(string fileName, list<DataGroupType>& groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture) {
	ifstream file;
	string resPath = getResourcePath();
	file.open(resPath + fileName);
	if (!file.good())
		throw runtime_error("Fehlgeschlagen mit dem ˆffnen der Datei: " + fileName);
	getline(file, imageName);
	if (setColourKey)
	{
		SDL_Surface* skizzenSurface = loadSurface(resPath + imageName, Globale::renderer);
		SDL_Color* transparentPixel = &skizzenSurface->format->palette->colors[transparentPixelIndex];
		SDL_SetColorKey(skizzenSurface, 1, SDL_MapRGB(skizzenSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

		 Skizze = convertSurfaceToTexture(skizzenSurface, Globale::renderer, false);
        if (createWhiteTexture)
        {
            SDL_Surface* whiteSurface = loadSurface(resPath + "weiﬂeSkizze.png", Globale::renderer);
            surfacePaletteSwap(skizzenSurface, whiteSurface);
            SDL_SetColorKey(skizzenSurface, 1, SDL_MapRGB(skizzenSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
            weiﬂeSkizze = convertSurfaceToTexture(skizzenSurface, Globale::renderer, false);
            cleanup(whiteSurface);
        }
        else {
            weiﬂeSkizze = nullptr;
        }
        cleanup(skizzenSurface);
    }
    else
        Skizze = loadTexture(resPath + imageName, Globale::renderer);


	string buffer;
	getline(file, buffer);
	stringstream ss;
	buffer = Globale::clipOffDataHeader(buffer);
	ss << buffer;
	int numberOfAnimations;
	ss >> numberOfAnimations;

	for (int i = 0; i < numberOfAnimations; i++)
	{
		Animation newAnimation;
		newAnimation.ladeAnimation(file, groupTypes);
		animationen.push_back(newAnimation);

	//	animationen.push_back(Animation(file, groupTypes));
	}

	
	file.close();


}
