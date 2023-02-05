#include "Animation.h"
#include "Globale.h" 
#include <list>
using namespace std;

Animation::Animation(string name) {
    this->name = name;
    frames = list<Frame>();
}

int Animation::getNeaechsteFrameNummer(int frameNummer) {
    if (frameNummer + 1 < frames.size())
        return frameNummer + 1;
    else
        return 0;
}

Frame* Animation::getNachsterFrame(Frame* frame) {
    return getFrame(getNeaechsteFrameNummer(frame->frameNummer));
}

int Animation::getEndFrameNummer() {
    return frames.size() - 1;
}

Frame* Animation::getFrame(int frameNummer) {
    if (frames.size() == 0)
        return nullptr;
    list<Frame>::iterator i = frames.begin();
    for (int counter = 0; counter < frameNummer && counter < frames.size() - 1; counter++) {
        i++;
    }
    return &(*i);
}

Animation::Animation(ifstream& file, list<DataGroupType>& groupTypes) {
    ladeAnimation(file, groupTypes);
}

void Animation::ladeAnimation(ifstream& file, list<DataGroupType>& groupTypes) {
	getline(file, name);
	string buffer;
	getline(file, buffer);
	stringstream ss;
	buffer = Globale::clipOffDataHeader(buffer);
	ss << buffer;
	int numberOfFrames;
	ss >> numberOfFrames;
	for (int i = 0; i < numberOfFrames; i++) {
		Frame newFrame;
		newFrame.loadFrame(file, groupTypes);
		frames.push_back(newFrame);
	}


}
