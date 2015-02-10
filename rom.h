#ifndef ROM_H
#define ROM_H
#include<vector>
#include<string>
#include "nes_types.h"

struct Rom {
//	std::vector<u8> prg_rom, chr_rom;
	u8 prg_rom[0xFFFF];
	u8 chr_rom[0xFFFF];
	regbit_t flags6;
	bool short_rom;
	int from_file(const std::string& file_name);
	u8 read(u16 addr);
	u8 read_pat0(u16 addr);
	u8 read_pat1(u16 addr);
};

#endif
