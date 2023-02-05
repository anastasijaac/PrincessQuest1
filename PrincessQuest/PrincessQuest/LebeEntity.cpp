#include "LebeEntity.h"


void LebeEntity::updateHitbox() {


	Schaden = 0;

	GroupBox* hitBoxes = (GroupBox*)GroupBuilder::findGroupByName("hitBox", aktuellFrame->frameData);
	if (hitBoxes != nullptr && hitBoxes->numberOfDataInGroup() > 0) {

		//aktualisiere hitbox
		SDL_Rect hb = hitBoxes->data.front();
		hitBox.x = static_cast<int>(x - aktuellFrame->offSet.x + hb.x);
		hitBox.y = static_cast<int>(y - aktuellFrame->offSet.y + hb.y);
		hitBox.w = hb.w;
		hitBox.h = hb.h;


		//aktualisiere damage 
		GroupNumber* Schaeden = (GroupNumber*)GroupBuilder::findGroupByName("damage", aktuellFrame->frameData);
		if (Schaeden != nullptr && Schaeden->numberOfDataInGroup() > 0) {
			Schaden = static_cast<int>(Schaeden->data.front());

		}



	}



}

void LebeEntity::updateInvincibleTimer() {
	if (invincibleTimer > 0) {
		invincibleTimer -= TimeControl::timeControl.dT;
	}

}


void LebeEntity::zeichnen() {

	//zeichnet aktuellen Frame
	if (aktuellFrame != nullptr && aktiv) {
		if (invincibleTimer > 0 && animSet->weißeSkizze != nullptr)
			aktuellFrame->Draw(animSet->weißeSkizze, x, y);
		else
			aktuellFrame->Draw(animSet->Skizze, x, y);


	}

	//zeichent KollisionsBox
	if (solid && Globale::debugging) {

		//setzt die aktuelle Farbe 
		SDL_SetRenderDrawColor(Globale::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globale::renderer, &letzteKollisionsBox);

		SDL_SetRenderDrawColor(Globale::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globale::renderer, &KollisionsBOX);

		SDL_SetRenderDrawColor(Globale::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globale::renderer, &hitBox);
	}

}