#include "PrincessQuestSpiel.h"

PrincessQuestSpiel::PrincessQuestSpiel() {
	string resourcePath = getResourcePath();
	Hintergrund = loadTexture(resourcePath + "map2.png", Globale::renderer);
	StartBild = loadTexture(resourcePath + "", Globale::renderer);
	Overlay = loadTexture(resourcePath + "", Globale::renderer);

	ZeigeStartBild = true;
	OverlayDauer = 2;

	//Beinhaltet eine Liste von gruppen Typen. Diese Liste beschreibt die Typen der Gruppen von den entnommen daten der fdset dateien (Frames)
	list<DataGroupType> dataGroupTypes;

	//Ladet die extra Informationen von fdest Dateien die nicht default sind

	//CollisionBox
	DataGroupType collisionBoxType;
	collisionBoxType.groupName = "collisionBox";
	collisionBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//Hitbox
	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

	//Damage
	DataGroupType damageType;
	damageType.groupName = "damage";
	damageType.dataType = DataGroupType::DATATYPE_NUMBER;

	//Daten Typen in liste einf¸gen
	dataGroupTypes.push_back(collisionBoxType);
	dataGroupTypes.push_back(hitBoxType);
	dataGroupTypes.push_back(damageType);

	PrincessAnimSet = new AnimationSet();
	PrincessAnimSet->ladeAnimationSet("Princess.fdset", dataGroupTypes, false, 0, false);

	MonsterAnimSet = new AnimationSet();
	MonsterAnimSet->ladeAnimationSet("Monster.fdset", dataGroupTypes, false, 0, false);
	
	WandAnimSet = new AnimationSet();
	WandAnimSet->ladeAnimationSet("Wand.fdset", dataGroupTypes);


	//Baut Princess Entity
	princess = new Princess(PrincessAnimSet);
	princess->invincibleTimer = 0;
	princess->x = Globale::ScreenWidth / static_cast<float>(2);
	princess->y = Globale::ScreenHeight / static_cast<float>(2);

	//TastaturEingabe f¸r Princess
	princessEingabe.princess = princess;
	//f¸gt princess in Entity Liste hinzu
	bigentity::bigentities.push_back(princess);

	//baut Wand Entity 
	int Groeﬂe = 32;
	for (int i = 0; i < Globale::ScreenWidth / Groeﬂe; i++) {
		//obere Reihe
		Wand* littlewand = new Wand(WandAnimSet);
		littlewand->x = i * Groeﬂe + Groeﬂe / static_cast<float>(2);
		littlewand->y = Groeﬂe / static_cast<float>(2);

		waende.push_back(littlewand);
		//f¸gt Wand in Entity List hinzu
		bigentity::bigentities.push_back(littlewand);

		//verwendet erneut den Pointer um die untere Reihe von W‰nden zu erstellen
		littlewand = new Wand(WandAnimSet);
		littlewand->x = i * Groeﬂe + Groeﬂe / static_cast<float>(2);
		littlewand->y = Globale::ScreenHeight - Groeﬂe / static_cast<float>(2);

		waende.push_back(littlewand);
		//f¸gt Wand in Entity List hinzu
		bigentity::bigentities.push_back(littlewand);
	}

	//W‰nde Rechts und Links
	for (int i = 1; i < Globale::ScreenHeight / Groeﬂe - 1; i++) {
		//Rechte Reihe
		Wand* littlewand = new Wand(WandAnimSet);
		littlewand->x = Groeﬂe / static_cast<float>(2);
		littlewand->y = i * Groeﬂe + Groeﬂe / static_cast<float>(2);

		waende.push_back(littlewand);
		//f¸gt Wand in Entity List hinzu
		bigentity::bigentities.push_back(littlewand);

		//verwendet erneut den Pointer um die Linke Reihe von W‰nden zu erstellen
		littlewand = new Wand(WandAnimSet);
		littlewand->x = Globale::ScreenWidth - Groeﬂe / static_cast<float>(2);
		littlewand->y = i * Groeﬂe + Groeﬂe / static_cast<float>(2);

		waende.push_back(littlewand);
		//f¸gt Wand in Entity List hinzu
		bigentity::bigentities.push_back(littlewand);
		
	}


}
PrincessQuestSpiel::~PrincessQuestSpiel() {
	cleanup(Hintergrund);
	cleanup(StartBild);
	cleanup(Overlay);

	if (PunktZahlTexture != nullptr)
		cleanup(PunktZahlTexture);

	bigentity::entferneAlleVonListe(&bigentity::bigentities, false); //Entfert entities von der liste, dies werden aber nicht gelˆscht

	delete PrincessAnimSet;
	delete WandAnimSet;
	delete MonsterAnimSet;

	delete princess;

	bigentity::entferneAlleVonListe(&waende, true); //Entfernt alle Wand entities aus der Liste
	bigentity::entferneAlleVonListe(&feinde, true); //Entfernt alle Monster entities aus der Liste
}

void PrincessQuestSpiel::update() {
	//MONSTER
	int monsterBauen = 2;
	int monsterGebaut = 0;
	float monsterBauTimer = 1;

	bool beenden = false;

	SDL_Event event;

	TimeControl::timeControl.neustart(); // Einrichtung meines Timecontrollers vor dem Start des Spiels

	//Spiel Loop
	while (!beenden) {
		TimeControl::timeControl.updateTime();
		bigentity::entferneInaktiveEntitiesVonListe(&bigentity::bigentities, false);
		bigentity::entferneInaktiveEntitiesVonListe(&feinde, true);

		//Checkt wenn Event ausgelˆst wird 
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT)
				beenden = true; //Schlieﬂt das Fenster

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					beenden = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (ZeigeStartBild)
						ZeigeStartBild = false;
					if (OverlayDauer <= 0 && princess->Lebenspunkte) {
						//cleanup 
						monsterGebaut = 0;
						monsterBauen = 2;
						monsterBauTimer = 3;
						OverlayDauer = 2;
					
						Monster::MonsterOpfer = 0;
						if (PunktZahlTexture != nullptr) {

							cleanup(PunktZahlTexture);
							PunktZahlTexture = nullptr;
						}

						//Alle Monster veschwinden 
						for (list<bigentity*>::iterator monster = feinde.begin(); monster != feinde.end(); monster++) {
							(*monster)->aktiv = false;

						}
						
					}
					princess->beleben();
					break;
				}
			}
			princessEingabe.update(&event);
		}

		if (princess->Lebenspunkte < 1 && OverlayDauer > 0) {
			OverlayDauer -= TimeControl::timeControl.dT;
		}


		//akutalisierung aller entities
		for (list<bigentity*>::iterator bigentity = bigentity::bigentities.begin(); bigentity != bigentity::bigentities.end(); bigentity++) {
			(*bigentity)->update();
		}
		
		if (princess->Lebenspunkte > 0 && !ZeigeStartBild) {
			if (monsterBauen == monsterGebaut) {
				monsterBauen = monsterBauen * 2;
				monsterGebaut = 0;
				monsterBauTimer = 4;
			}
			monsterBauTimer -= TimeControl::timeControl.dT;
			if (monsterBauTimer <= 0 && monsterGebaut < monsterBauen && feinde.size() < 10) {
				Monster *feind = new Monster(MonsterAnimSet);
				//Monster position in der Arena 
				feind->x = (float)getRandomNumber(Globale::ScreenWidth - (2 * 32) - 32) + 32 + 16;
				feind->y = (float)getRandomNumber(Globale::ScreenWidth - (2 * 32) - 32) + 32 + 16;
				feind->invincibleTimer = static_cast <float>(0.1);

				feinde.push_back(feind);
				bigentity::bigentities.push_back(feind);

			}
		}
		
		zeichnen();
	}

}
void PrincessQuestSpiel::zeichnen() {
	//s‰ubert Bildschirm
	SDL_SetRenderDrawColor(Globale::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globale::renderer);

	if (ZeigeStartBild) {
		renderTexture(StartBild, Globale::renderer, 0, 0);
	}
	else
	{
		//zeichnet Hintergrund
		renderTexture(Hintergrund, Globale::renderer, 0, 0);

		bigentity::bigentities.sort(bigentity::entityVergleich);

		//zeichnet alle entities
		for (list<bigentity*>::iterator bigentity = bigentity::bigentities.begin(); bigentity != bigentity::bigentities.end(); bigentity++) {
			(*bigentity)->zeichnen();
		}

		if (OverlayDauer <= 0 && princess->Lebenspunkte < 1) {
			renderTexture(StartBild, Globale::renderer, 0, 0);
			if (PunktZahlTexture == nullptr) {
				SDL_Color farbe = { 255, 255, 255, 255 }; //weiﬂe Farbe

				stringstream ss;
				ss << "Monster besiegt: " << Monster::MonsterOpfer;

				string resourcenPath = getResourcePath();
				PunktZahlTexture = renderText(ss.str(), resourcenPath + "", farbe, 30, Globale::renderer);
			}

			renderTexture(PunktZahlTexture, Globale::renderer, 20, 50);
		}



	}
	//Ausgabe
	SDL_RenderPresent(Globale::renderer);

}