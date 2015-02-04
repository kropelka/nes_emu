#ifndef WINDOW_H
#define WINDOW_H
#include "SDL/SDL.h"
#include "nes_types.h"

struct Window {
	SDL_Surface* screen;
	SDL_Overlay* overlay;
	SDL_Rect videoRect;
	void refresh();
	Window();
	void put_pixel(u8, u8, u8, u8, u8);
	void random();
	void resize(SDL_Event*);
	~Window();

};



#endif
