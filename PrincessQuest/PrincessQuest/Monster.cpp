#include "Monster.h"

const string Monster::MONSTER_ANIM_RIGHT = "WalkingRight";
const string Monster::MONSTER_ANIM_LEFT = "WalkingLeft";
const string Monster::MONSTER_ANIM_ATTACK = "MonsterAttack";
const string Monster::MONSTER_ANIM_RIP_LEFT = "RIPLeft";
const string Monster::MONSTER_ANIM_RIP_RIGHT = "RIPRight";
const string Monster::MONSTER_ANIM_IDLERIGHT = "IdleRight";
const string Monster::MONSTER_ANIM_IDLELEFT = "IdleLeft";
const string Monster::MONSTER_ANIM_IDLEUP = "IdleUp";

const int Monster::MONSTER_STATE_BEWEGUNG = 0;
const int Monster::MONSTER_STATE_ATTACK = 1;
const int Monster::MONSTER_STATE_DEAD = 2;
const int Monster::MONSTER_STATE_IDLE = 3;

const int Monster::MONSTER_AI_NORMAL = 0;
const int Monster::MONSTER_AI_CHASE = 1;

int Monster::MonsterOpfer = 0;


Monster::Monster(AnimationSet* animSet) {
	this->animSet = animSet;

	type = "feind";

	//Defaults
	x = Globale::ScreenWidth / static_cast<float>(2);
	y = Globale::ScreenWidth / static_cast<float>(2);
	bewegungsGeschwindigkeit = 0;
	bewegungsGeschwindigkeitMAx = 20;
	Lebenspunkte = LebenspunkteMax = 20 + (rand() & 30);
	Schaden = 0;
	KollisionsBoxW = 18;
	KollisionsBoxH = 20;
	KollisionsBOX.w = KollisionsBoxW;
	KollisionsBOX.h = KollisionsBoxH;

	KollisionsBoxYOffset = -14;

	richtung = DIR_UNTEN;
	wechselAnimation(MONSTER_STATE_IDLE, true);

	updateKollisionBox();

}
void Monster::update() {

	if (Lebenspunkte < 1 && status != MONSTER_STATE_DEAD) {
		wechselAnimation(MONSTER_STATE_DEAD, true);
		bewegung = false;
		sterben();
	}
	denken();

	updateKollisionBox();
	updateBewegung();
	updateKollision();
	updateHitbox();
	updateSchaeden();

	updateAnimation();
	updateInvincibleTimer();
}
void Monster::denken() {
	if (status == MONSTER_STATE_IDLE || status == MONSTER_STATE_BEWEGUNG) {
		denkTimer -= TimeControl::timeControl.dT;
		if (denkTimer <= 0) {
			// reset timer
			denkTimer = rand() % 5; //0 - 5 sek
			int aktion = rand() % 10; //0 - 9 sek

			if (aktion < 3) {
				bewegung = false;
				AIStatus = MONSTER_AI_NORMAL;
				wechselAnimation(MONSTER_STATE_IDLE, true);
			}
			else {
				sucheZiel();
				// Ziel gefunden 
				if (ziel != nullptr && ziel->Lebenspunkte > 0) {
					float dist = bigentity::distanzZwischenZweiEntities(this, ziel);
					//wenn ziel in der nähe ist dann attacke
					if (dist < 100) {
						attacke();
						AIStatus = MONSTER_AI_NORMAL;
					}
					else {
						AIStatus = MONSTER_AI_CHASE;
						bewegung = true;
						wechselAnimation(MONSTER_STATE_BEWEGUNG, status != MONSTER_STATE_BEWEGUNG);
					}
				}
				else {
					//Keine Ziele
					bewegung = false;
					AIStatus = MONSTER_AI_NORMAL;
					wechselAnimation(MONSTER_STATE_IDLE, true);

				}
			}
		}
	}
	// Wenn Ziel verfolgen dann mach 
	if (AIStatus == MONSTER_AI_CHASE && Lebenspunkte > 0 && aktiv) {
		winkel = bigentity::winkelZwischenZweiEntities(this, ziel);
		bewegen(winkel);
	}
}
void Monster::attacke() {
	bewegung = false;
	frameTimer = 0;
	rutschbetrag = 100;
	gleitwinkel = winkel;
	wechselAnimation(MONSTER_STATE_ATTACK, true);

}
void Monster::sterben() {
	bewegung = false;
	status = MONSTER_STATE_DEAD;
	wechselAnimation(MONSTER_STATE_DEAD, true);
	//TODO SOUND

	//Punkte
	Monster::MonsterOpfer++;

}
void Monster::sucheZiel() {
	ziel = nullptr;
	//findet das nahestes Ziel
	for (auto bigentity = bigentity::bigentities.begin(); bigentity != bigentity::bigentities.end(); bigentity++) {
		if ((*bigentity)->type == "princess" && (*bigentity)->aktiv){
			if (ziel == nullptr) {
				ziel = (LebeEntity*)(*bigentity);
			} else {
				if (bigentity::distanzZwischenZweiEntities(this, (*bigentity)) < bigentity::distanzZwischenZweiEntities(this, ziel)) {
					ziel = (LebeEntity*)(*bigentity);
				}
			}
		}
	}


}
void Monster::wechselAnimation(int neuerStatus, bool neustartFrameZumStart) {
	status = neuerStatus;

	switch (status) {
	case MONSTER_STATE_IDLE:
		switch (richtung) {
		case DIR_RECHTS:
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_IDLERIGHT);
			break;
		case DIR_LINKS:
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_IDLELEFT);
			break;
		case DIR_OBEN:
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_IDLEUP);
			break;
		}
		break;
	case MONSTER_STATE_BEWEGUNG:
		switch (richtung) {
		case DIR_LINKS:
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_LEFT);
			break;
		case DIR_RECHTS:
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_RIGHT);
			break;
		}
		break;
	case MONSTER_STATE_ATTACK:
		if (richtung == DIR_OBEN)
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_ATTACK);
		break;
	case MONSTER_STATE_DEAD:
		switch (richtung) {
		case DIR_LINKS:
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_RIP_LEFT);
			break;
		case DIR_RECHTS:
			aktuellAnim = animSet->getAnimation(MONSTER_ANIM_RIP_RIGHT);
			break;
		}
		break;
	}
	if (neustartFrameZumStart)
		aktuellFrame = aktuellAnim->getFrame(0);
	else
		aktuellFrame = aktuellAnim->getFrame(aktuellFrame->frameNummer);
}
void Monster::updateAnimation() {
	if (aktuellFrame == nullptr || aktuellAnim == nullptr)
		return;

	if (status == MONSTER_STATE_BEWEGUNG && !bewegung) {
		wechselAnimation(MONSTER_STATE_IDLE, true);
	}

	if(status != MONSTER_STATE_BEWEGUNG && bewegung) {
		wechselAnimation(MONSTER_STATE_BEWEGUNG, true);
	}

	frameTimer += TimeControl::timeControl.dT;
	if (frameTimer >= aktuellFrame->duration) {
		if (aktuellFrame->frameNummer == aktuellAnim->getEndFrameNummer()) {
			if (status == MONSTER_STATE_ATTACK) {
				attacke();
			}
			else if (status == MONSTER_STATE_ATTACK) {
				wechselAnimation(MONSTER_STATE_BEWEGUNG, true);
			}
			else if (status == MONSTER_STATE_DEAD) {
				frameTimer = 0;
				if (Lebenspunkte > 0)
					wechselAnimation(MONSTER_STATE_BEWEGUNG, true);
				else aktiv = false;
	
			}
			else {
				aktuellFrame = aktuellAnim->getFrame(0);
			}
		}
		else {
			aktuellFrame = aktuellAnim->getNachsterFrame(aktuellFrame);
		}
		frameTimer = 0;
	}
}
void Monster::updateSchaeden() {
	if (aktiv && Lebenspunkte > 0 && invincibleTimer <= 0) {
		for (auto bigentity = bigentity::bigentities.begin(); bigentity != bigentity::bigentities.end(); bigentity++) {
			if ((*bigentity)->aktiv && (*bigentity)->type == "princess") {
				LebeEntity* feind = (LebeEntity*)(*bigentity);

				if (feind->Schaden > 0 && bigentity::checkKollision(KollisionsBOX, feind->hitBox)) {
					Lebenspunkte -= feind->Schaden;

					if (Lebenspunkte > 0) {
						//TODO SOUND
						invincibleTimer = static_cast <float>(0.1);
					}
					gleitwinkel = bigentity::winkelZwischenZweiEntities((*bigentity), this);
					rutschbetrag = 100;
				}
			}
		}
	}


}
