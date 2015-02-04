#include "ppu.h"

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
