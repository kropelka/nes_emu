#include "nes_types.h"
#include<iostream>
#include "rom.h"
#include "mem.h"
#include "cpu.h"
#include "ppu.h"
#include "window.h"
#include <SDL/SDL.h>
#define DEBUG_CONSOLE

#ifdef DEBUG_CONSOLE
#include "debug.h"
#endif



u8 nes_pal[65][3] = {
 { 0x7c, 0x82, 0x7b},
 { 0x28, 0xf7, 0x6c},
 { 0x23, 0xdd, 0x70},
 { 0x47, 0xc5, 0x7e},
 { 0x43, 0xaa, 0xbb},
 { 0x40, 0x77, 0xce},
 { 0x44, 0x5e, 0xca},
 { 0x40, 0x61, 0xb8},
 { 0x3c, 0x63, 0x92},
 { 0x4c, 0x5a, 0x4f},
 { 0x44, 0x5e, 0x55},
 { 0x3c, 0x63, 0x5c},
 { 0x38, 0x95, 0x5f},
 { 0x10, 0x80, 0x80},
 { 0x10, 0x80, 0x80},
 { 0x10, 0x80, 0x80},
 { 0xb4, 0x82, 0x7c},
 { 0x65, 0xd1, 0x3b},
 { 0x6d, 0xcd, 0x36},
 { 0x67, 0xd0, 0x81},
 { 0x5d, 0xc0, 0xd8},
 { 0x54, 0x88, 0xe5},
 { 0x6c, 0x45, 0xe1},
 { 0x7d, 0x47, 0xc4},
 { 0x7c, 0x3b, 0x9e},
 { 0x6c, 0x45, 0x36},
 { 0x64, 0x4a, 0x3c},
 { 0x6b, 0x6c, 0x37},
 { 0x63, 0x9b, 0x3d},
 { 0x2a, 0x80, 0x80},
 { 0x17, 0x80, 0x80},
 { 0x17, 0x80, 0x80},
 { 0xe6, 0x81, 0x81},
 { 0x97, 0xb1, 0x3a},
 { 0x87, 0xbb, 0x67},
 { 0x8d, 0xb8, 0x86},
 { 0xa5, 0xa9, 0xb3},
 { 0x8c, 0x85, 0xc8},
 { 0x95, 0x5b, 0xc0},
 { 0xa8, 0x46, 0xb1},
 { 0xad, 0x1d, 0xad},
 { 0xbf, 0x1e, 0x73},
 { 0x9c, 0x56, 0x4c},
 { 0xb2, 0x6d, 0x3a},
 { 0x9b, 0xa2, 0x11},
 { 0x58, 0x80, 0x80},
 { 0x1b, 0x80, 0x80},
 { 0x1b, 0x80, 0x80},
 { 0xeb, 0x80, 0x80},
 { 0xc7, 0x93, 0x5d},
 { 0xb5, 0x9f, 0x7b},
 { 0xbd, 0x9a, 0x8a},
 { 0xc6, 0x94, 0x9b},
 { 0xc5, 0x86, 0x95},
 { 0xc9, 0x6c, 0x92},
 { 0xd3, 0x65, 0x90},
 { 0xca, 0x51, 0x96},
 { 0xd1, 0x46, 0x7b},
 { 0xc8, 0x4c, 0x6a},
 { 0xcb, 0x7d, 0x62},
 { 0xa5, 0xa9, 0x08},
 { 0xba, 0x7d, 0x83},
 { 0x1e, 0x80, 0x80},
 { 0x1e, 0x80, 0x80},
 { 0x00, 0x00, 0x00}, // NES BG PLACE

};

bool quit = false;


struct Keys {
	int exit;
	int debug_refresh;
	int toggle_stepping;
	Keys() {
		exit = SDLK_ESCAPE;
		debug_refresh = SDLK_PAGEDOWN;
		toggle_stepping = SDLK_F10;
	};
} keybindings;



/*
void process_event(SDL_Event* ev, Window* w) {
	static int key;
	switch(ev->type) {
		case SDL_VIDEORESIZE:
			w->resize(ev);
			break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			key = ev->key.keysym.sym;
			if(key==keybindings.exit) {
				quit = true;
			} else if(key==keybindings.debug_refresh) {
			};
			switch(ev->key.keysym.sym) {
				case keybindings.exit:
					quit = true;
					break;

				default:
					break;
			};
			break;
	};
};
*/




int main(int argc, char* argv[]) {
	Rom rom;
	MemMap mem;
	Ppu ppu;


	bool step=false;
	std::cout << "NES_EMU version 0.01" << std::endl;
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " romfile " << std::endl;
		return 1;
	};

	std::cout << "Loading " << argv[1] << std::endl;
	rom.from_file(argv[1]);

	//inicjalizacja i polaczenie szyn adresowych CPU, PPU i cartridge'a
	ppu.connect_rom(&rom);
	mem.connect_rom(&rom);
	mem.connect_ppu(&ppu);

	Cpu cpu(&mem);
	cpu.reset();
	
	
	Window window;
	SDL_Event ev;
	ppu.set_nt_mirroring(0);

	window.refresh();

	u8 color[3];
	u8 tmp1, tmp2, tmp3;

#ifdef DEBUG_CONSOLE
	DebugWindow debug(&cpu, &mem, &ppu);
#endif

	// Główna pętla programu
	while(!quit) {

		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_VIDEORESIZE:
					window.resize(&ev);
					break;
		
				case SDL_QUIT:
					quit = true;
					break;
					
				case SDL_KEYUP:
					if(ev.key.keysym.sym == SDLK_ESCAPE) {
						quit = true;
					};
#ifdef DEBUG_CONSOLE
					if(ev.key.keysym.sym == SDLK_PAGEDOWN) {
						debug.update();
					};
#endif
					break;

				default:
					break;
				};
		};

		ppu.ppustatus.vblank = 0;
		ppu.run_nt = 0;
		for(ppu.scanline = 0; ppu.scanline < 262; ++ppu.scanline) {
			cpu.do_ticks(144);
			ppu.nt_sel[ppu.scanline] = ppu.run_nt;
			ppu.scroll_x[ppu.scanline] = ppu.run_scroll_x;

			// wchodzimy w vblank...
			if(ppu.scanline == 239) {
				ppu.ppustatus.vblank = 1;
				if(ppu.ppuctrl.nmi_on_vblank==1) {
					cpu.do_nmi();
					fprintf(stderr, "nmi na vblank on!\n");
				};
			};
		};
		ppu.draw_frame();
	}; 
};
