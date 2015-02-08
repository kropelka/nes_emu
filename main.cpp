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
	bool step=false;
	std::cout << "NES_EMU version 0.01" << std::endl;
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " romfile " << std::endl;
		return 1;
	} else {
		std::cout << "Loading " << argv[1] << std::endl;
		rom.from_file(argv[1]);
		if(argc > 2) {
			if( *argv[2] == 's') {
				step = true;
			} else
				return 0;
		};

		Ppu ppu;
		ppu.connect_rom(&rom);
		mem.connect_rom(&rom);
		mem.connect_ppu(&ppu);
		Cpu cpu(&mem);
		cpu.reset();
		bool quit = false;
		Window window;
		SDL_Event ev;
		ppu.set_nt_mirroring(0);
		while(!quit) {

			ppu.ppustatus.vblank = 0;
			while(SDL_PollEvent(&ev)) {
				switch(ev.type) {
					case SDL_VIDEORESIZE:
						window.resize(&ev);
						break;
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_KEYUP:
						if(!step) {
//						window.random();
						for(unsigned i = 0; i < 30; i++) {
							for(unsigned j = 0; j < 32; j++) {
								ppu.update_bg_buff(i, j, 0);
							};
						};
						ppu.showbgbuff();
						for(unsigned i=0; i < 240; ++i) {
							for(unsigned j=0; j < 256; ++j) {
								window.put_pixel(i, j, ppu.bg_buff[256*i + j], ppu.bg_buff[256*i+j] * 2, ppu.bg_buff[256*i+j] * 4);
							};
						};
						window.refresh();
						} else
							fprintf(stderr, "PC = %x\n", cpu.R.PC.W);
							cpu.do_ticks(1);
						break;
				};
			};
			if(!step) {
				for(ppu.scanline = 0; ppu.scanline < 262; ++ppu.scanline) {
					//fprintf(stderr, "CPU PC = %x\n", cpu.R.PC.W); 
					cpu.do_ticks(144);
					ppu.process();
					if(ppu.scanline == 239) {
						ppu.ppustatus.vblank = 1;
//						cpu.do_nmi();
						if(ppu.ppuctrl.nmi_on_vblank==1) {
							cpu.do_nmi();
							fprintf(stderr, "nmi na vblank on!\n");
						};
					};
				};
			}; //else {
				//cpu.do_ticks(1);
			//};
		};
	};
/*
	while(true) {
		cpu.tick();
		std::cin.get();
	};
*/
};
