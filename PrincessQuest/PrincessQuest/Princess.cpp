#include "princess.h"
#include "AnimationSet.h"

//IDLE
const string Princess::PRINCESS_ANIM_IDLE_UNTEN = "IdleDown";
const string Princess::PRINCESS_ANIM_IDLE_OBEN = "IdleUp";
const string Princess::PRINCESS_ANIM_IDLE_LINKS = "IdleLeft";
const string Princess::PRINCESS_ANIM_IDLE_RECHTS = "IdleRight";

//Bewegung
const string Princess::PRINCESS_ANIM_LINKS = "WalkingLeft";
const string Princess::PRINCESS_ANIM_RECHTS = "WalkingRight";
const string Princess::PRINCESS_ANIM_OBEN = "WalkingUp";
const string Princess::PRINCESS_ANIM_UNTEN = "WalkingDown";

////Sterben
const string Princess::PRINCESS_ANIM_STERBEN = "RIP";

//SLASH-Angreifen
const string Princess::PRINCESS_SLASH_ANIM_RECHTS = "SlashRight";
const string Princess::PRINCESS_SLASH_ANIM_LINKS = "SlashLeft";


const int Princess::PRINCESS_STATE_IDLE = 0;
const int Princess::PRINCESS_STATE_BEWEGUNG = 1;
const int Princess::PRINCESS_STATE_STERBEN = 2;
const int Princess::PRINCESS_STATE_SLASH = 3;


//Konstruktor
Princess::Princess(AnimationSet* animSet) {
	this->animSet = animSet;
	type = "princess";

	x = static_cast<float>(Globale::ScreenWidth / 2);
	y = Globale::ScreenHeight / static_cast<float>(2);
	bewegungsGeschwindigkeit = 0;
	bewegungsGeschwindigkeitMAx = 20;
	Lebenspunkte = LebenspunkteMax = 50;
	Schaden = 0;
	KollisionsBoxW = 20;
	KollisionsBoxH = 24;
	KollisionsBoxYOffset = -20;
	

	richtung = DIR_UNTEN;

	wechselAnimation(PRINCESS_STATE_IDLE, true);

	updateKollisionBox();


}



void Princess::update() {
	//Check ob Tod ist
	if (Lebenspunkte < 1 && status != PRINCESS_STATE_STERBEN) {
		wechselAnimation(PRINCESS_STATE_STERBEN, true);
		bewegung = false;
		sterben();
	}

	updateKollisionBox();
	updateBewegung();
	updateKollision();
	updateHitbox();
	updateSchaeden();

	updateAnimation();
	updateInvincibleTimer();


}
void Princess::slash(bool check) {
	if (Lebenspunkte > 0 && (status == PRINCESS_STATE_BEWEGUNG || status == PRINCESS_STATE_IDLE)) {


		bewegung = check;
		wechselAnimation(PRINCESS_STATE_SLASH, true);
		//TO DO vielleicht fügen wir sounds ein -> Angriff-Sound
	}

}
void Princess::obenBewegung(bool check) {
	bewegung = check;
	aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_OBEN);
}
void Princess::untenBewegung(bool check) {
	bewegung = check;
	wechselAnimation(PRINCESS_STATE_BEWEGUNG, !check);
}
void Princess::linksBewegung(bool check) {
	bewegung = check;
	aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_LINKS);
	
	
}
void Princess::rechtsBewegung(bool check) {
	bewegung = check;
	aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_RECHTS);
}
void Princess::sterben() {
	bewegung = false;
	aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_IDLE_UNTEN);
	//wechselAnimation(PRINCESS_STATE_STERBEN, true);

}
void Princess::beleben() {
	Lebenspunkte = LebenspunkteMax;
	wechselAnimation(PRINCESS_STATE_IDLE, true);
	bewegung = false;
	x = Globale::ScreenWidth / static_cast<float>(2);
	y = Globale::ScreenHeight / static_cast<float>(2);
	rutschbetrag = 0;
}

void Princess::wechselAnimation(int neuerStatus, bool neustartFrame) {
	status = neuerStatus;

	switch (status) {
	case PRINCESS_STATE_IDLE:
		switch (richtung) {
		case DIR_UNTEN:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_IDLE_UNTEN);
			break;
		case DIR_OBEN:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_IDLE_OBEN);
			break;
		case DIR_LINKS:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_IDLE_LINKS);
			break;
		case DIR_RECHTS:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_IDLE_RECHTS);
			break;
		}
		break;
	case PRINCESS_STATE_BEWEGUNG:
		switch (richtung) {
		case DIR_UNTEN:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_UNTEN);
			break;
		case DIR_OBEN:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_OBEN);
			break;
		case DIR_LINKS:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_LINKS);
			break;
		case DIR_RECHTS:
			aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_RECHTS);
			break;
		}
		break;
	case PRINCESS_STATE_SLASH: {
		switch (richtung) {
		case DIR_LINKS:
			aktuellAnim = animSet->getAnimation(PRINCESS_SLASH_ANIM_LINKS);
			break;
		case DIR_RECHTS:
			aktuellAnim = animSet->getAnimation(PRINCESS_SLASH_ANIM_RECHTS);
			break;
		}
		break;
	}
	case PRINCESS_STATE_STERBEN:
		aktuellAnim = animSet->getAnimation(PRINCESS_ANIM_STERBEN);
		break;
	}
	aktuellFrame = neustartFrame ? aktuellAnim->getFrame(0)
		: aktuellAnim->getFrame(aktuellFrame->frameNummer);

}
	

void Princess::updateAnimation() {
	if (aktuellFrame == nullptr || aktuellAnim == nullptr)
		return;

	if (status == PRINCESS_STATE_BEWEGUNG && !bewegung) {
		wechselAnimation(PRINCESS_STATE_IDLE, true);
	}
	if (status != PRINCESS_STATE_BEWEGUNG && bewegung) {
		wechselAnimation(PRINCESS_STATE_BEWEGUNG, true);
	}


	frameTimer += TimeControl::timeControl.dT;

	//Änderung der Frames
	if (frameTimer >= aktuellFrame->duration) {

		//Ende eines Frames
		if (aktuellFrame->frameNummer == aktuellAnim->getEndFrameNummer()) {
			if (status == PRINCESS_STATE_SLASH) {
				//zur�ck zur bewegung  status/animation
				wechselAnimation(PRINCESS_STATE_BEWEGUNG, true);
			}
			else if (status == PRINCESS_STATE_STERBEN && Lebenspunkte > 0) {
				//Wiederbelebung durch Lebenspunkte -> Bewegungsstatus

			}
			else if (status == PRINCESS_STATE_STERBEN && Lebenspunkte > 0) {
				//Wiederbelebung durch Lebenspunkte -> Bewegungsstatus
				wechselAnimation(PRINCESS_STATE_BEWEGUNG, true);
			}
			else
			{
				//neustart der Animation 
				aktuellFrame = aktuellAnim->getFrame(0);
			}
		}

	}
		else
		{
			//Übergang zum nächsten Frame in der Animation
			aktuellFrame = aktuellAnim->getNachsterFrame(aktuellFrame);
		}
		frameTimer = 0;
}

void Princess::updateSchaeden() {
	if (aktiv && Lebenspunkte > 0 && invincibleTimer <= 0) {
		for (auto bigentity = bigentity::bigentities.begin(); bigentity != bigentity::bigentities.end(); bigentity++ ) //Iterator 
		{
			if ((*bigentity)->aktiv && (*bigentity)->type == "feind") {
				//Monster können Monster wiederbeleben
				LebeEntity* monster = (LebeEntity*)(*bigentity);

				if (monster->Schaden > 0 && bigentity::checkKollision(KollisionsBOX, monster->hitBox)) {

					//noch am Leben 
					if (Lebenspunkte > 0) {
						invincibleTimer = static_cast<float>(0.3);

						//TO DO: Hit Sound
					}

					gleitwinkel = bigentity::winkelZwischenZweiEntities((*bigentity), this);
					rutschbetrag = 200;
				}
			}
		}
	}

}
