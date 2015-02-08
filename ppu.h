#ifndef PPU_H
#define PPU_H

#include "nes_types.h"
#include "rom.h"

struct Ppu {
	ppuctrl_t ppuctrl;
	ppumask_t ppumask;
	ppustatus_t ppustatus;
	regbit_t oam_addr, oam_data, scroll, ppu_addr, ppu_data;
	Ppu();


	int scanline;

	u8* ppu_mem[0x4000]; // ogolna mapa pamieci PPU

	u8 sprite_mem[256];
	u8 nametables[0x1000];
	u8 bg_pal[0x10], sprite_pal[0x10];


	ppu_addrbus_t io_memaddr;
	bool sprite_latch, addr_latch;


	void tick();
	void reset();
//	void connect_mem(const MemMap&);
	void mem_write(u16 addr, u8 val);
	u8 mem_read(u16 addr);
	void set_nt_mirroring(unsigned);
	void connect_rom(const Rom&);
	void draw_scanline();
};



#endif

