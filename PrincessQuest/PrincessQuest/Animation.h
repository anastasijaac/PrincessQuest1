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

	int getNeaechsteFrameNummer(int frameNummer); // gibt die n�chste frame nummer in unserer Liste zur�ck
	Frame* getNachsterFrame(Frame* frame);
	int getEndFrameNummer(); // gibt die letzte frame nummek zur�ck
	Frame* getFrame(int frameNummer); // gibt uns gew�nschtes frame mit der eingegebenen frame nummer zur�ck

	void ladeAnimation(ifstream &file, list<DataGroupType>& groupTypes);
};

#endif
