#include "window.h"
#include "nes_types.h"

Window::Window() {
	SDL_Init(SDL_INIT_EVERYTHING);

	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_RESIZABLE);
	overlay = SDL_CreateYUVOverlay(256, 240, SDL_IYUV_OVERLAY, screen);
	videoRect.x = 0;
	videoRect.y = 0;
	videoRect.w = 640;
	videoRect.h = 480;
	SDL_UnlockYUVOverlay(overlay);
};


void Window::refresh() {
//	SDL_YUVOverlay(overlay);
	SDL_DisplayYUVOverlay(overlay, &videoRect);
//	SDL_YUVOverlay(overlay);
};

void Window::put_pixel(u8 x, u8 y, u8 col_y, u8 col_u, u8 col_v) {
	fprintf(stderr, "%u %u\n", x, y);
	SDL_LockYUVOverlay(overlay);
	overlay->pixels[0][256*y + x] = col_y;
	overlay->pixels[1][256*y + x] = col_u;
	overlay->pixels[2][256*y + x] = col_v;
	SDL_UnlockYUVOverlay(overlay);
};

void Window::resize(SDL_Event* ev) {
	SDL_SetVideoMode(ev->resize.w, ev->resize.h, 0, SDL_RESIZABLE);
	videoRect.w = ev->resize.w;
	videoRect.h = ev->resize.h;
};
/*
void Window::draw_sprite(Rom* rom, unsigned nr, u8 x, u8 y) {
	for(auto i=0; i < 8; ++i) {
		for(auto j=0; j<8; ++j) {

};
*/
void Window::random() {
	for(int i=0; i < 100; ++i) {
	u8 x, y, yy, u, v;
	x = rand() % 255;
	y = rand() % 240;
	yy = rand() % 0xFF;
	u = rand() % 0xFF;
	v = rand() % 0xFF;
	put_pixel(x, y, yy, u, v);
	};
};


Window::~Window() {
	SDL_FreeYUVOverlay(overlay);
};

