#ifndef PPU_H
#define PPU_H

#include "nes_types.h"
#include "rom.h"

const unsigned SIZE_X = 256;
const unsigned SIZE_Y = 240;


struct Ppu {

	/* Mapa pamieci w przestrzeni adresowej PPU:
	 * 0x0000 ... 0x0FFF pattern table 1 (CHRROM)
	 * 0x1000 ... 0x1FFF pattern table 2 (CHRROM)
	 * 0x2000 ... 0x2FFF nametables[]
	 * 0x3F00 ... 0x3F0F paleta tla
	 * 0x3F10 ... 0x3F1F paleta sprite'ow
	 */
	ppuctrl_t ppuctrl;
	ppumask_t ppumask;
	ppustatus_t ppustatus;
	regbit_t oam_addr, oam_data, scroll, ppu_addr, ppu_data;
	reg16_t scroll_addr;
	Ppu();

	Rom* rom;

	u8 nt_on_scanline[262];
	u8 bg_buff[256*240];
	u8 sprite_buff[256*240];
	u8 run_scroll_x;
	u8 run_nt;
	u16 scroll_x[262];
	u16 scroll_y;
	bool horiz_copy;


	int scanline;
	int x;


	u8* ppu_mem[0x4000]; // ogolna mapa pamieci PPU

	u8 sprite_mem[256];
	u8 nametables[0x1000];
	u8 bg_pal[0x10], sprite_pal[0x10];


	ppu_addrbus_t io_memaddr;
	bool scroll_latch, addr_latch;


	void tick();
	void reset();
//	void connect_mem(const MemMap&);
	void mem_write(u16 addr, u8 val);
	u8 mem_read(u16 addr);
	void set_nt_mirroring(unsigned);
	void connect_rom(Rom*);
	void draw_scanline();
	void dma_load(u8* sprites);
	void process();
	void update_bg_buff(u8, u8, u8);
	void showbgbuff();
	u8 ppu_read(u16);
	void ppu_write(u16, u8);
	void draw_patterns();
	void draw_frame();
	void draw_sprites();
	void draw_bg();
};



#endif

