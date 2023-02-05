#include "Wand.h"


Wand::Wand(AnimationSet* animSet) {
	this->animSet = animSet;

	solid = true;
	KollisionsBoxW = 32;
	KollisionsBoxH = 32;
	KollisionsBoxYOffset = -16;

	updateKollisionBox();
	wechselAnimation(0, false);

}
void Wand::update() {
	updateKollisionBox();

	if (aktuellFrame == nullptr || aktuellAnim == nullptr)
		return;
	frameTimer += TimeControl::timeControl.dT;
	if (frameTimer >= aktuellFrame->duration) {
		aktuellFrame = aktuellAnim->getNachsterFrame(aktuellFrame); 
		frameTimer = 0;
	}

}
void Wand::wechselAnimation(int neuerStatus, bool resetFrameZumAnfang) {
	aktuellAnim = animSet->getAnimation("wand");
	aktuellFrame = aktuellAnim->getFrame(0);
}
