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


void MemMap::connect_rom(Rom* r) {
	rom = r;
};

void MemMap::connect_ppu(Ppu* p) {
	ppu = p;
};


void MemMap::cpu_write(u16 addr, u8 val) {
//	fprintf(stderr, "mem[%x] <- %x\n", addr, val);
	switch(addr) {
		case 0x0000 ... 0x1FFF:
			work_ram[addr % 0x800] = val;
			break;

		case 0x2000 ... 0x3FFF:
			ppu->mem_write(addr, val);
			break;

		case 0x4000 ... 0x4013:
			break;

		case 0x4014:   // sprite DMA
			fprintf(stderr, "dma!");
			ppu->dma_load(&work_ram[val*0x100]);
			break;

		default:
			break;
	};
};


u8 MemMap::cpu_read(u16 addr) {
//	fprintf(stderr, "rd mem[%x]\n", addr);
	switch(addr) {
		case 0x0000 ... 0x1FFF:
			return work_ram[addr % 0x800];
			break;

		case 0x2000 ... 0x3FFF:
			return ppu->mem_read(addr);
			break;

		case 0x8000 ... 0xFFFF:
		//	fprintf(stderr, "rom read z %x\n", addr);
			return rom->read(addr);
			break;
	
		default:
			return 0;
			break;
	};
};


