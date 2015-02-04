#include "nes_types.h"
#include<iostream>
#include "rom.h"
#include "mem.h"
#include "cpu.h"
#include "ppu.h"
#include "window.h"
#include <SDL/SDL.h>
int main(int argc, char* argv[]) {
	Rom rom;
	MemMap mem;
	std::cout << "NES_EMU version 0.01" << std::endl;
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " romfile " << std::endl;
		return 1;
	} else {
		std::cout << "Loading " << argv[1] << std::endl;
	rom.from_file(argv[1]);
	mem.connect_rom(rom);
	Cpu cpu(&mem);
	cpu.reset();
	Ppu ppu(&mem);
	bool quit = false;
	Window window;
	SDL_Event ev;
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
					window.random();
					window.refresh();
					break;
			};
		};
	};
	};
/*
	while(true) {
		cpu.tick();
		std::cin.get();
	};
*/
};
