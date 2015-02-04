#include "mem.h"
#include<cstdio>
#include "rom.h"

/*! inicjalizacja pustej mapy pamieci z przydzieleniem obszarow stalych
 */
MemMap::MemMap() {
	// RAM ogolnego przeznaczenia
	for(u16 j=0; j < 0x800; ++j) {
		cpu_mem[j] = cpu_mem[0x800 + j] = cpu_mem[0x1000 + j] =
			cpu_mem[0x1800 + j] = &(work_ram[j]);                // mirroring
	};
	
	// porty joysticka
	cpu_mem[0x4016] = &(joy_port1.val);
	cpu_mem[0x4017] = &(joy_port2.val);

	// rejestry PPU
	u16 addr = 0x2000;
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


	for(j=0; j < 0x2000; ++j) {
		ppu_mem[j] = (u8*) (rom.chr_rom.data()) + j;
	};

	fprintf(stderr, "Connected ROM, address space ends at %x", j);
};
