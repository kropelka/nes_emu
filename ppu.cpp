#include "ppu.h"
#include<cstdio>
/*
Ppu::Ppu(MemMap* mem_) {
	mem = mem_;
	ppuctrl = &(mem->ppuctrl);
	ppumask = &(mem->ppumask);
	ppustatus = &(mem->ppustatus);
	oam_addr = &(mem->oam_addr);
	oam_data = &(mem->oam_data);
	scroll = &(mem->scroll);
	ppu_addr = &(mem->ppu_addr);
	ppu_data = &(mem->ppu_data);
};
*/


/*! ustawienie trybu mirroringu tablicy sprite'ow
 * 0 -> mirroring poziomy
 * 1 -> mirroring pionowy
 * 2 -> cztery nametable (dodatkowy VRAM w cartridge'u)
 */
void Ppu::set_nt_mirroring(unsigned mode) {
	unsigned j;
	switch(mode) {
		case 0:  // mirroring poziomy
			for(j=0; j < 0x400; ++j) { 
				ppu_mem[0x2400 + j] = &(nametables[j]);
				ppu_mem[0x2800 + j] = ppu_mem[0x2C00 + j] = &(nametables[0x400 + j]);
			};
			break;

		case 1: // mirroring poziomy
			for(j=0; j < 0x400; ++j) {
				ppu_mem[0x2800 + j] = &(nametables[j]);
				ppu_mem[0x2400 + j] = ppu_mem[0x2C000 + j] = &(nametables[0x400 + j]);
			};
			break;
		case 2 ... 3: // cztery nametable
			fprintf(stderr, "Not yet implemented.");		
			break;
	};
};


Ppu::Ppu() {
	unsigned j;
	for(j=0; j < 0x400; ++j) {
		ppu_mem[0x2000 + j] = &(nametables[j]);
	};

	// paleta
	for(u8 j=0; j < 0x10; ++j) {
		ppu_mem[0x3F00 + j] = &(bg_pal[j]);
		ppu_mem[0x3F10 + j] = &(sprite_pal[j]);
	};
	// osobliwosci PPU...
	ppu_mem[0x3F10] = ppu_mem[0x3F00]; 
	ppu_mem[0x3F14] = ppu_mem[0x3F04];
	ppu_mem[0x3F18] = ppu_mem[0x3F08];
	ppu_mem[0x3F1C] = ppu_mem[0x3F0C];



};


void Ppu::connect_rom(const Rom& rom) {
	u16 j = 0;
	// CHR ROM
	for(j=0; j < 0x2000; ++j) {
		ppu_mem[j] = (u8*) (rom.chr_rom.data()) + j;
	};
	set_nt_mirroring(rom.flags6.b0 + 2*rom.flags6.b1);
};



void Ppu::tick() {

};

u8 Ppu::mem_read(u16 addr) {
	switch(0x2000 + (addr % 8)) {
		case 0x2000:  // ppuctrl
			return ppuctrl.val;
			break;

		case 0x2001: // ppumask
			return ppumask.val;
			break;

		case 0x2002: // ppustatus
			sprite_latch = addr_latch = false;  // odczytanie PPUSTATUS powoduje rozpoczecie wczytywania adresow sprite/vram od nowa
			return ppustatus.val;
			break;

		case 0x2003:
			return oam_addr.val;
			break;

		case 0x2004:  // sprite memory data
			return sprite_mem[(oam_addr.val)++];
			break;

		case 0x2007:
			return *(ppu_mem[io_memaddr.val++]);
			break;
	};
};




void Ppu::mem_write(u16 addr, u8 val) {
	static u16 full_addr;

	switch(0x2000 + (addr % 8)) {
		case 0x2002:
			sprite_latch = addr_latch = false;
			ppustatus.val = val;
			break;

		case 0x2004: // sprite memory data
			sprite_mem[oam_addr.val] = val;
			(oam_addr.val)++;
			break;

		case 0x2006:
			if(!addr_latch) {
				addr_latch = true;
				io_memaddr.lo = val;	
			} else {
				addr_latch = false;
				io_memaddr.hi = val;
			};
	 		break;

		case 0x2007:
			*(ppu_mem[io_memaddr.val]) = val;
			io_memaddr.val += (ppuctrl.vram_inc) ? 32 : 1;
			break;

		default:
			break;
	};

};

void Ppu::draw_scanline() {
};




void Ppu::reset() {
	ppuctrl.val = ppumask.val = ppustatus.val = scroll.val = ppu_addr.val = 0;
};


