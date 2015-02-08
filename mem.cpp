#include "mem.h"
#include<cstdio>
#include "rom.h"

/*! inicjalizacja pustej mapy pamieci z przydzieleniem obszarow stalych
 */
MemMap::MemMap() {
	reset_intr = nmi_intr = irq_intr = false;


	// RAM ogolnego przeznaczenia
	for(u16 j=0; j < 0x800; ++j) {
		cpu_mem[j] = cpu_mem[0x800 + j] = cpu_mem[0x1000 + j] =
			cpu_mem[0x1800 + j] = &(work_ram[j]);                // mirroring
	};
	
	// porty joysticka
	cpu_mem[0x4016] = &(joy_port1.val);
	cpu_mem[0x4017] = &(joy_port2.val);


	// rejestry PPU
/*	u16 addr = 0x2000;
	while(addr < 0x4000) {
		cpu_mem[addr] = &(ppuctrl.val);
		cpu_mem[addr+1] = &(ppumask.val);
		cpu_mem[addr+2] = &(ppustatus.val);
		cpu_mem[addr+3] = &(oam_addr.val);
		cpu_mem[addr+4] = &(oam_data.val);
		cpu_mem[addr+5] = &(scroll.val);
		cpu_mem[addr+6] = &(ppu_addr.val);
		cpu_mem[addr+7] = &(ppu_data.val);
		addr += 8;
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
*/



	// SRAM
	for(u16 j=0; j < 0x2000; ++j) {
		cpu_mem[0x6000 + j] = &(sram[j]);
	};

	// cartridge
	//
	/*
	switch(mapper) {
		case 0: 
			for(u16 j=0; j < 0x8000; ++j) {
				cpu_mem[0x8000 + j] = rom_bank0[j]);
			};
			break;
	};*/
	

};


void MemMap::connect_rom(const Rom& rom) {
	u16 j=0;
	for(j=0; j < rom.prg_rom.size(); ++j) {
		cpu_mem[0xC000 + j] = cpu_mem[0x8000 + j] = (u8*) (rom.prg_rom.data()) + j;
		
	};
};



void MemMap::cpu_write(u16 addr, u8 val) {
	switch(addr) {

		case 0x2000 ... 0x3FFF:
			ppu->mem_write(addr, val);
			break;

		default:
			*(cpu_mem[addr]) = val;
			break;
	};
};


u8 MemMap::cpu_read(u16 addr) {
	switch(addr) {
		case 0x2000 ... 0x3FFF:
			return ppu->mem_read(addr);
			break;

		default:
			return *(cpu_mem[addr]);
			break;
	};
};


