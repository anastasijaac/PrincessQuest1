#include "frame.h"
#include "Globale.h"

void Frame::Draw(SDL_Texture* Skizze, float x, float y) {
	SDL_Rect dest;
	dest.x = static_cast<int>(x - offSet.x);
	dest.y = static_cast<int>(y - offSet.y);
	dest.w = clip.w;
	dest.h = clip.h;

	renderTexture(Skizze, Globale::renderer, dest, &clip);

}

void Frame::loadFrame(ifstream &file, list<DataGroupType>& groupTypes) {
	GroupBuilder::buildGroups(groupTypes, frameData);


	string buffer;
	//frame
	getline(file, buffer);
	//clip
	getline(file, buffer);
	stringstream ss; //um strings zu bauen und lesen
	buffer = Globale::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h;
	//offset/pivot
	getline(file, buffer);
	ss.clear();
	buffer = Globale::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offSet.x >> offSet.y;
	//duration
	getline(file, buffer);
	ss.clear();
	buffer = Globale::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;
	//index
	getline(file, buffer);
	ss.clear();
	buffer = Globale::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNummer;

	GroupBuilder::loadGroups(file, frameData);
}

