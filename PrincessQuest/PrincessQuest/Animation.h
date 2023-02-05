#ifndef AMIMATION
#define ANIMATION

#include "Frame.h"

using namespace std;

class Animation {
public:
	Animation(ifstream& file, list<DataGroupType>& groupTypes);

	string name; //Name der Animation
	list<Frame> frames; //Liste unserer Frames

	Animation(string name = "");

	int getNeaechsteFrameNummer(int frameNummer); // gibt die nächste frame nummer in unserer Liste zurück
	Frame* getNachsterFrame(Frame* frame);
	int getEndFrameNummer(); // gibt die letzte frame nummek zurück
	Frame* getFrame(int frameNummer); // gibt uns gewünschtes frame mit der eingegebenen frame nummer zurück

	void ladeAnimation(ifstream &file, list<DataGroupType>& groupTypes);
};

#endif
