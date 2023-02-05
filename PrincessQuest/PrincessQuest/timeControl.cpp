#include "timeControl.h"


const int TimeControl::PLAY_STATUS = 0, TimeControl::PAUSE_STATE = 1;

TimeControl::TimeControl() {
	dT = 0;
	letztesUpdate = 0;
	timeStatus = PLAY_STATUS;

}


void TimeControl::updateTime() {
	if (timeStatus == PAUSE_STATE) {
		dT = 0;

	}
	else {
		Uint32 timeDiff = SDL_GetTicks() - letztesUpdate;
		dT = static_cast<float>(timeDiff / 1000.0);
	}
	letztesUpdate = SDL_GetTicks();
}


void TimeControl::pause() {
	timeStatus = PAUSE_STATE;


}


void TimeControl::fortfahren() {
	timeStatus = PLAY_STATUS;

}


void TimeControl::neustart() {
	dT = 0;
	letztesUpdate = SDL_GetTicks();
}

//Baut ein globales TimeControl Objekt  
TimeControl TimeControl::timeControl;

