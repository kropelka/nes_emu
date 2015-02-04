#ifndef PPU_H
#define PPU_H

#include "nes_types.h"
#include "mem.h"

struct Ppu {
	ppuctrl_t* ppuctrl;
	ppumask_t* ppumask;
	ppustatus_t* ppustatus;
	regbit_t* oam_addr, *oam_data, *scroll, *ppu_addr, *ppu_data;
	MemMap* mem;
	Ppu(MemMap*);
//	void connect_mem(const MemMap&);

};

#endif

