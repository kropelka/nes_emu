#ifndef WINDOW_H
#define WINDOW_H
#include "SDL/SDL.h"


struct Window {
	SDL_Surface* screen;
	SDL_Overlay* overlay;
	SDL_Rect videoRect;

};



#endif
