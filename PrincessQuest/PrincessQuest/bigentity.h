#ifndef BIGENTITY
#define BIGENTITY 

#include "GLobale.h"
#include "timeControl.h"
#include "AnimationSet.h"


//Abstrakte Klasse; kann ein Objekt des Typs Entity nicht instanziieren
class bigentity {
public:
	//Referenzkonstanten
	static const int DIR_UNTEN = 0, DIR_OBEN = 1, DIR_LINKS = 2, DIR_RECHTS = 3, DIR_NICHTS = -1;

	//quick label f�r die entity
	int status{};

	float x, y{};
	int richtung{};
	bool solid = true; //k�nnen Dinge durch solid laufen
	bool KollidierenMitSolids = true; //k�nnen Dinge durch andere solids laufen
	bool aktiv = true; //entity an oder aus 
	string type = "entity"; //Welche Art von Entity haben wir ? Prinzessin, Monster etc.
	bool bewegung{}; //entity bewegt sich 
	float winkel{}; //Bewegungswinkel 
	float bewegungsGeschwindigkeit{};
	float bewegungsGeschwindigkeitMAx{};
	float gleitwinkel{}; //Push-Richtung
	float rutschbetrag{}; //H�he der Pushrichtigung 
	float bewegungsLerp = 4;
	float totaleXBewegeung, totaleYBewegung{};


	SDL_Rect KollisionsBOX{}; //gibt die Gr��e unsers Entitys an 
	SDL_Rect letzteKollisionsBox{}; //letzte Positions der Box
	int KollisionsBoxW, KollisionsBoxH{}; // urspr�ngliche Gr��e 
	float KollisionsBoxYOffset{}; //oritiert sich nach y Wert des Entitys, wo sich die Box oritiert


	AnimationSet* animSet{}; //Set aller Animationen des Entitys
	Animation* aktuellAnim{}; //aktuelle Animationen des Entitys (Nutzung)
	Frame* aktuellFrame{}; //aktueller Frame der genutzten Animation
	float frameTimer{}; //hilft beim Frames animieren

	//------------------------------------------------------------------------------------

	//Virtuelle Funktionen 

	virtual void update();
	virtual void zeichnen();

	virtual void bewegen(float Winkel);
	virtual void updateBewegung();
	virtual void updateKollisionBox();
	virtual void wechselAnimation(int neuerStatus, bool neustartFrameAnfang) = 0; //abstrakte funktion
	virtual void updateKollision();

	//------------------------------------------------------------------------------------

	//Unterst�tzungs Funktionen
	static float distanzZwischenZweiRects(const SDL_Rect &r1, const SDL_Rect &r2);
	static float distanzZwischenZweiEntities(const bigentity *e1, const bigentity *e2);
	static float distanzZwischenZweiPunkten(float cx1, float cy1, float cx2, float cy2);
	static float winkelZwischenZweiEntities(const bigentity *e1, const bigentity *e2);
	static bool checkKollision(const SDL_Rect cbox1, const SDL_Rect cbox2);
	static int winkelZuRichtung(float);
	static float winkelZwischenZweiPunkte(float cx1, float cy1, float cx2, float cy2);
	static float winkelZwischenZweiRects(const SDL_Rect &r1, const SDL_Rect &r2);


	//-------------------------------------------------------------------------------------
	
	//globale Entity-Liste
	static list <bigentity*> bigentities;

	static bool entityVergleich(const bigentity* const &a, const bigentity * const &b); //Vergleicht 2 Entitys (y-Werte) in der Liste
	static void entferneInaktiveEntitiesVonListe(list<bigentity*> *bigentityListe, bool loeschenEntitys);
	static void entferneAlleVonListe(list<bigentity*>* bigentityListe, bool loeschenEntitys);

};
#endif
