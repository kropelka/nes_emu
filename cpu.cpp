#include "nes_types.h"
#include "cpu.h"
#include<iostream>
#include<cstdio>

static MemMap* mem;

extern "C" {
	void Wr6502(u16 addr, u8 val) { 
		mem->cpu_write(addr, val);
	};
	
	u8 Rd6502(u16 addr) {
		fprintf(stderr, "Reading from memory at address %4x\n", addr);
		return mem->cpu_read(addr);
	};

	u8 Loop6502(M6502 *r) {
		if(mem->nmi_intr) {
			return INT_NMI;
		} else if(mem->irq_intr) {
			return INT_IRQ;
		} else
			return INT_NONE;
	};

	u8 Patch6502(byte op, M6502 *r) {
		fprintf(stderr, "Inv opcode %2x on PC=%4x\n", op, r->PC.W);
		return 1;
	};
	u8 Debug6502(M6502 *R) {
		fprintf(stderr, "PC=%4x A=%2x P=%2x X=%2x Y=%2x S=%2x\n", R->PC.W, R->A, R->P, R->X, R->Y, R->S);
		return 1;
	};
		
};

/*
struct Cpu {
	M6502* R;
	void reset();
	int tick() { return Exec6502(R, 1); };
	Cpu(MemMap*);
};
*/


Cpu::Cpu(MemMap* mem_) {
	mem = mem_;
};

void Cpu::reset() {
	Reset6502(&R);
	R.PC.W = 0xC000;
};

int Cpu::tick() {
	return Exec6502(&R, 1);
};
	
