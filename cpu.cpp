#include "nes_types.h"
#include "cpu.h"
#include<iostream>
#include<cstdio>

static MemMap* mem;
char* opcode_table[0x100] = {
	#include "names.txt"
};


extern "C" {
	void Wr6502(u16 addr, u8 val) { 
//		fprintf(stderr, "Writing from memory at address %4x value %2x", addr, val);
		mem->cpu_write(addr, val);
	};
	
	u8 Rd6502(u16 addr) {
//		fprintf(stderr, "Reading from memory at address %4x\n", addr);
		return mem->cpu_read(addr);
	};

	u8 Op6502(u16 addr) {
		return Rd6502(addr);
	};

	u8 Loop6502(M6502 *r) {
		return INT_NONE;
	};

	u8 Patch6502(byte op, M6502 *r) {
//		fprintf(stderr, "Inv opcode %2x on PC=%4x\n", op, r->PC.W);
//		exit(1);
		return 0;
	};
	u8 Debug6502(M6502 *R) {
		fprintf(stderr, "%s ", opcode_table[mem->cpu_read(R->PC.W)]);
		fprintf(stderr, "PC=%4x opcode = %2x A=%2x P=%2x X=%2x Y=%2x S=%2x\n", R->PC.W, mem->cpu_read(R->PC.W), R->A, R->P, R->X, R->Y, R->S);
		return 0;
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
	fprintf(stderr, "CPU init.\n");
};

void Cpu::reset() {
	Reset6502(&R);
//	R.PC.W = 0xC000;
};

void Cpu::do_nmi() {
	Int6502(&R, INT_NMI);
};
	

void Cpu::debug_msg() {
	Debug6502(&R);
};


int Cpu::tick() {
	return Exec6502(&R, 1);
};

void Cpu::do_ticks(unsigned no) {
	Exec6502(&R, no);
};
