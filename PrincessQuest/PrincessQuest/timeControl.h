#ifndef TIMECONTROL
#define TIMECONTROL


#include <iostream>
#include "SDL.h"

class TimeControl {
public:

	//reference values
	static const int PLAY_STATUS, PAUSE_STATE;

	int timeStatus;
	Uint32 letztesUpdate; //Letzter Status der Anzahl unserer Ticks 
	float dT; //delta time (1dt = 0.5 sekunden). Zeitpunkt an dem der letzten Frame auf dem Bildschirm gerenderet wurde. 

	TimeControl();
	void updateTime();
	void pause();
	void fortfahren();
	void neustart();


	static TimeControl timeControl;



};

#endif




