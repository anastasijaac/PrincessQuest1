#ifndef DRAWINGFUNCS_H
#define DRAWINGFUNCS_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"

using namespace std;

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

SDL_Surface* loadSurface(const std::string &file, SDL_Renderer *ren);

void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette);

SDL_Texture *convertSurfaceToTexture(SDL_Surface* surface, SDL_Renderer *ren, bool cleanSurface = false);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip = nullptr);

SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer);

SDL_Texture* renderText(const std::string &message, TTF_Font*font,
	SDL_Color color, SDL_Renderer *renderer);


bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer);

#endif