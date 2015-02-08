#ifndef CPU_H
#define CPU_H

#define DEBUG
#define EXEC6502
#include "m6502/M6502.h"
#include "mem.h"

struct Cpu {
	M6502 R;
	void reset();
	int tick();
	void do_ticks(unsigned);
	void do_nmi();
	Cpu(MemMap*);
};

#endif
