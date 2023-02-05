#include "bigentity.h"
#include "Globale.h"

const int bigentity::DIR_UNTEN, bigentity::DIR_OBEN, bigentity::DIR_LINKS, bigentity::DIR_RECHTS, bigentity::DIR_NICHTS;

//Virtuelle Funktionen 

void bigentity::update() { ; }
void bigentity::zeichnen() { 
	
	//zeichnet aktuellen Frame
	if (aktuellFrame != nullptr && aktiv) {
		aktuellFrame->Draw(animSet->Skizze, x, y);
	}

	//zeichent KollisionsBox
	if (solid && Globale::debugging) {

		//setzt die aktuelle Farbe 
		SDL_SetRenderDrawColor(Globale::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globale::renderer, &KollisionsBOX);

	}


} 

void bigentity::bewegen(float Winkel) {
	bewegung = true;
	bewegungsGeschwindigkeit = bewegungsGeschwindigkeitMAx;
	this->winkel = winkel;

	int neueRichtung = winkelZuRichtung(winkel);

	//Wenn sich die Richtung ändert, update die aktuelle Richtung
	if (richtung != neueRichtung) {
		richtung = neueRichtung;
		wechselAnimation(status, false);
	}

}

void bigentity::updateBewegung(){
	updateKollisionBox();
	//lagere, bevor wir bewegen
	letzteKollisionsBox = KollisionsBOX;

	//neustart aller Bewegungen
	totaleXBewegeung = 0;
	totaleYBewegung = 0;

	if (bewegungsGeschwindigkeit > 0) {
		//berechnet die Bewegungsdistanz mit speed , dt(Zeitpunkt seit der letzten Schleife) und multipliziert mit lerp

		float bewegungdistanz = bewegungsGeschwindigkeit* (TimeControl::timeControl.dT)*bewegungsLerp;
		if (bewegungdistanz > 0)
		{
			//Mathe Formel: xBewegung = Distanz multipliziert mit Kosinus(Winkelradius)
			float xBewegung = bewegungdistanz*(cos(winkel*Globale::PI / 180));
			//Mathe Formel: yBewegung = Distanz multipliziert mit Sinus(Winkelradius)
			float yBewegung = bewegungdistanz*(sin(winkel*Globale::PI / 180));

			x += xBewegung;
			y += yBewegung;

			totaleXBewegeung = xBewegung;
			totaleYBewegung = yBewegung;

			if (!bewegung)
				bewegungsGeschwindigkeit -= bewegungdistanz;
		}
	}

	if (rutschbetrag > 0) {

		float rutschdistanz = rutschbetrag*TimeControl::timeControl.dT*bewegungsLerp;
		if (rutschdistanz > 0) {
			float xBewegung = rutschdistanz*(cos(gleitwinkel*Globale::PI / 180));
			float yBewegung = rutschdistanz*(sin(gleitwinkel*Globale::PI / 180));

			x += xBewegung;
			y += yBewegung;

			totaleXBewegeung += xBewegung;
			totaleYBewegung += yBewegung;
			rutschbetrag -= rutschdistanz;
		}
		else
		{
			rutschbetrag = 0;
		}
	}
	
	//Nach der Bewegung folgt die Bewegung der KolllisionsBox nach
	updateKollisionBox();
	//
	SDL_UnionRect(&KollisionsBOX, &letzteKollisionsBox, &KollisionsBOX);

}

	void bigentity::updateKollisionBox() {
		KollisionsBOX.x = static_cast<int>(x - KollisionsBOX.w / static_cast<float>(2));
		KollisionsBOX.y = static_cast<int>(y + KollisionsBoxYOffset);
		KollisionsBOX.w = KollisionsBoxW;
		KollisionsBOX.h = KollisionsBoxH;
	}
	//Checkt ob wir gegen etwas laufen 
	void bigentity::updateKollision(){
		if (aktiv && KollidierenMitSolids && (bewegungsGeschwindigkeit > 0 || rutschbetrag > 0)) {
			//Liste der Hindernisse
			list<bigentity*> kollisionen;

			for (auto littleEntity = bigentity::bigentities.begin(); littleEntity != bigentity::bigentities.end(); littleEntity++) {
				if ((*littleEntity)->aktiv //Wenn dieses Entity in unserer Liste aktiv ist
					&& (*littleEntity)->type != this->type //und der typ sich von unserem typ unterscheidet
					&& (*littleEntity)->solid // und es solide ist
					&& bigentity::checkKollision(KollisionsBOX, (*littleEntity)->KollisionsBOX)) //und wir damit kollidieren
				{
					kollisionen.push_back(*littleEntity); //füg es in unserer Liste hinzu
				}
			}
			//damit es gecheckt wird
			if (kollisionen.size() > 0) {
				updateKollisionBox();

				//Distannz in 1/6
				float boxDistanz = 0;
				if (KollisionsBOX.w < KollisionsBOX.h)
					boxDistanz = static_cast<float>(KollisionsBOX.w / 4);
				else
					boxDistanz = static_cast<float>(KollisionsBOX.h / 4);

				//RundgangBox wird benutzt um kollisionen vom Startpunkt bis zum Endpunkt zu checken, iterationen = boxDistanz
				SDL_Rect RundgangBox = letzteKollisionsBox;
				float KollisionsBewegungX = RundgangBox.x, KollisionsBewegungY = RundgangBox.y;
				float bewegungsWinkle = bigentity::winkelZwischenZweiRects(letzteKollisionsBox, KollisionsBOX);

				bool gefundeneKollisionen = false;
				while (!gefundeneKollisionen) {
					//checkt RundgangBox für Kollisionen
					SDL_Rect überschneidung;
					for (auto littleEntity = kollisionen.begin(); littleEntity != kollisionen.end(); littleEntity++) {
						if (SDL_IntersectRect(&RundgangBox, &(*littleEntity)->KollisionsBOX, &überschneidung)) //gibt true wieder wenn es Überschneidungen zwischen rundgangBox und kollisionsBox gibt und aktuallisiert diese
						{
							gefundeneKollisionen = true;
							bewegungsGeschwindigkeit = 0;
							bewegung = false;
							gleitwinkel = winkelZwischenZweiEntities((*littleEntity), this);

							//kollision vermeidung

							if (überschneidung.w < überschneidung.h) {

								if (letzteKollisionsBox.x + letzteKollisionsBox.w / 2 < (*littleEntity) ->KollisionsBOX.x + (*littleEntity)->KollisionsBOX.w / 2)
									RundgangBox.x -= überschneidung.w; //von links nach links
								else
									RundgangBox.x += überschneidung.w; //von rechts nach rechts

							}else {
								if (letzteKollisionsBox.y + letzteKollisionsBox.h / 2 < (*littleEntity)->KollisionsBOX.x + (*littleEntity)->KollisionsBOX.h / 2)
									RundgangBox.x -= überschneidung.h; //von oben nach oben
								else
									RundgangBox.x += überschneidung.h; //von unten nach unten

							}
						}
					}
					// exit wenn kollision gefunden wurden ist oder es eine überschneidung von rundangBox und kollisionsBox gibt
					if (gefundeneKollisionen || (RundgangBox.x == KollisionsBOX.x && RundgangBox.y == KollisionsBOX.y))
						break;

					if (distanzZwischenZweiRects(RundgangBox, KollisionsBOX) > boxDistanz) {
						bewegungsWinkle = bigentity::winkelZwischenZweiRects(RundgangBox, KollisionsBOX);
						float xBewegen = boxDistanz * (cos(bewegungsWinkle * Globale::PI / 100)); 
						float yBewegen = boxDistanz * (sin(bewegungsWinkle * Globale::PI / 100));

						
						KollisionsBewegungX += static_cast<int>(xBewegen);
						KollisionsBewegungY += static_cast<int>(yBewegen);
						RundgangBox.x = KollisionsBewegungX;
						RundgangBox.y = KollisionsBewegungY;
						
					}else {
						RundgangBox = KollisionsBOX;

					}

				}
				if (gefundeneKollisionen) {
					//bewege Entity zu unserer RundgangBox
					rutschbetrag = rutschbetrag / 2;
					x = static_cast<float>(RundgangBox.x + RundgangBox.w / 2);
					y = static_cast<float>(RundgangBox.y = static_cast<int>(KollisionsBoxYOffset));
				}

				updateKollisionBox();

			}
		}
	}

//------------------------------------------------------------------------------------

//Unterstützungs Funktionen
	float bigentity::distanzZwischenZweiRects(const SDL_Rect &r1, const SDL_Rect &r2) {
		SDL_Point e1, e2;

		e1.x = r1.x + r1.w / 2;
		e1.y = r1.y + r1.h / 2; //e.y

		e2.x = r2.x + r2.w / 2;
		e2.y = r2.y + r2.h / 2; //e.y

		
		float distanz = static_cast<float>(abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2))));
		return distanz;
	}
	float bigentity::distanzZwischenZweiEntities(const bigentity *e1, const bigentity *e2) {
		float distanz = static_cast<float>(abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2))));
		return distanz;
	}
	float bigentity::distanzZwischenZweiPunkten(float cx1, float cy1, float cx2, float cy2) {
		float distanz = static_cast<float>(abs(sqrt(pow(cx2 - cx1, 2) + pow(cy2 - cy1, 2))));
		return distanz;

	}
	float bigentity::winkelZwischenZweiEntities(const bigentity *e1, const bigentity *e2) {
		float distanzX, distanzY;
		float x1 = e1->x, y1 = e1->y, x2 = e2->x, y2 = e2->y;
		distanzX = x2 - x1;
		distanzY = y2 - y1;

		return atan2(distanzY, distanzY) * 100 / Globale::PI;
	}
	//checks überschneidungen
	bool bigentity::checkKollision(SDL_Rect cbox1, SDL_Rect cbox2) {
		SDL_Rect intersection;
		if (SDL_IntersectRect(&cbox1, &cbox2, &intersection)) {
			return true;
		}
		return false;

	}
	//konvertiert 360° Winkel zu unseren const values
	int bigentity::winkelZuRichtung(float winkel) {
		if ((winkel >= 0 && winkel <= 45) || winkel >= 315 && winkel <= 360)
			return DIR_RECHTS;
		else if (winkel >= 45 && winkel <= 135)
			return DIR_UNTEN;
		else if (winkel >= 135 && winkel <= 225)
			return DIR_LINKS;
		else
			return DIR_OBEN;
		//Gegen den Uhrzeigersinn


	}
	float bigentity::winkelZwischenZweiPunkte(float cx1, float cy1, float cx2, float cy2) {
		float distanzX = cx2 - cx1;
		float distanzY = cy2 - cy1;

		return atan2(distanzY, distanzX) * 180 / Globale::PI;

	}
	float bigentity::winkelZwischenZweiRects(const SDL_Rect &r1, const SDL_Rect &r2) {
		float cx1 = static_cast<float>(r1.x + (r1.w / 2));
		float cy1 = static_cast<float>(r1.y + (r1.h / 2));
		
		float cx2 = static_cast<float>(r2.x + (r2.w / 2));
		float cy2 = static_cast<float>(r2.y + (r2.h / 2));

		return winkelZwischenZweiPunkte(cx1, cy1, cx2, cy2);
	}


//-------------------------------------------------------------------------------------

//globale Entity-Liste
	list <bigentity*> bigentity::bigentities;

	//Vergleicht 2 Entitys (y-Werte) in der Liste
	bool bigentity::entityVergleich(const bigentity* const& a, const bigentity* const& b) {
		if (a != 0 && b != 0) {
			return (a->y < b->y);
		}else {
			return false;
		}

	}

	void bigentity::entferneInaktiveEntitiesVonListe(list<bigentity*> *bigentityListe, bool loeschenEntities) {
		for (auto littleEntity = bigentityListe->begin(); littleEntity != bigentityListe->end();) {
			if (!(*littleEntity)->aktiv) {
				if (loeschenEntities)
					delete (*littleEntity);
				littleEntity = bigentityListe->erase(littleEntity);
			}else {
				littleEntity++;
			}
		}

	}
	void bigentity::entferneAlleVonListe(list<bigentity*>* bigentityListe, bool loeschenEntities) {
		for (auto littleEntity = bigentityListe->begin(); littleEntity != bigentityListe->end();) {
				if (loeschenEntities)
					delete (*littleEntity);
				littleEntity = bigentityListe->erase(littleEntity);
		}

	}