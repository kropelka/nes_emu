#ifndef MEM_H
#define MEM_H
#include "nes_types.h"
#include "rom.h"

struct MemMap {
	u8* cpu_mem[0x10000];
	u8* ppu_mem[0x4000];
	u8 work_ram[0x800];
	u8 rom_bank0[0x8000];
	u8 sram[0x2000];
	// rejestry PPU
	ppuctrl_t ppuctrl;
	ppumask_t ppumask;
	ppustatus_t ppustatus;
	regbit_t oam_addr, oam_data, scroll, ppu_addr, ppu_data;



	regbit_t joy_port1, joy_port2;
	u8 mapper;
	void connect_rom(const Rom& rom);
	MemMap();

};

#endif
