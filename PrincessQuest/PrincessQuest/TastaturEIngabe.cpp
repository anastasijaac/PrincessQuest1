#include "TastaturEingabe.h"
#include "princess.h"
#include "AnimationSet.h"
#include "bigentity.h"

TastaturEingabe::TastaturEingabe() {
	//Tasten für Akitionen Hardcode
	WalkingUp = SDL_SCANCODE_UP;
	WalkingDown = SDL_SCANCODE_DOWN;
	WalkingLeft = SDL_SCANCODE_LEFT;
	WalkingRight = SDL_SCANCODE_RIGHT;
	Slash = SDL_SCANCODE_S;


}


void TastaturEingabe::update(SDL_Event* event) {
	kollision();
	//Taste gedrückt
	if (event->type == SDL_KEYDOWN) {
		if (event->key.keysym.scancode == WalkingUp) {
			princess->y-=5;
			princess->obenBewegung(true);
		}
		if (event->key.keysym.scancode == WalkingDown) {
			princess->y += 5;
			princess->untenBewegung(true);
		}
		if (event->key.keysym.scancode == WalkingLeft) {
			princess->x -= 5;
			princess->linksBewegung(true);
		}
		if (event->key.keysym.scancode == WalkingRight) {
			princess->x += 5;
			princess->rechtsBewegung(true);
		}
		if (event->key.keysym.scancode == Slash) {
			princess->slash(true);
		}
	}
	//Taste losgelassen
	else if (event->type == SDL_KEYUP)
	{
		if (event->key.keysym.scancode == WalkingUp) {
			princess->y -= 5;
			princess->obenBewegung(false);
		}
		if (event->key.keysym.scancode == WalkingDown) {
			princess->y += 5;
			princess->untenBewegung(false);
		}
		if (event->key.keysym.scancode == WalkingLeft) {
			princess->x -= 5;
			princess->linksBewegung(false);
		}
		if (event->key.keysym.scancode == WalkingRight) {
			princess->x+=5;
			princess->rechtsBewegung(false);
		}
		if (event->key.keysym.scancode == Slash) {
			princess->slash(false);
		}
	}
}

void TastaturEingabe::kollision()
{
	if (princess->x <= 40)

	{
		princess->x = 40;
		std::cout << princess->x << std::endl;
	}
	if (princess->x >= 595)
	{
		princess->x = 595;
		std::cout << princess->x << std::endl;
	}
	if (princess->y <=51)

	{
		princess->y = 51;
		
	}
	if (princess->y >= 316)
	{
		princess->y = 316;
		std::cout << princess->y << std::endl;
	}
}
