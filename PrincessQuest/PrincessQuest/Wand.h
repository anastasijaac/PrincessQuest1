#ifndef WAND
#define WAND

#include "bigentity.h"

class Wand : public bigentity {
public:
	Wand(AnimationSet* animSet);
	void update();
	void wechselAnimation(int neuerStatus, bool neustartFrameAnfang);
	void updateKollisionen() {

	}
};
#endif 

