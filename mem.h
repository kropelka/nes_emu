#ifndef MEM_H
#define MEM_H
#include "nes_types.h"
#include "rom.h"
#include "ppu.h"


struct MemMap {
	u8* cpu_mem[0x10000];
	u8 work_ram[0x800];
	u8 rom_bank0[0x8000];
	u8 sram[0x2000];
	Ppu* ppu;
	Rom* rom;

	// rejestry PPU
	ppuctrl_t ppuctrl;
	ppumask_t ppumask;
	ppustatus_t ppustatus;
	regbit_t oam_addr, oam_data, scroll, ppu_addr, ppu_data;
	// RAM PPU
	u8 nametables[0x1000];
	// rejestry palet
	u8 bg_pal[0x10], sprite_pal[0x10];


	bool reset_intr, nmi_intr, irq_intr;

	void cpu_write(u16 addr, u8 val);
	u8 cpu_read(u16 addr);

	regbit_t joy_port1, joy_port2;
	u8 mapper;
	void connect_rom(Rom*);
	void connect_ppu(Ppu*);
	MemMap();

};

#endif
