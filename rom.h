#ifndef ROM_H
#define ROM_H
#include<vector>
#include<string>
#include "nes_types.h"

struct Rom {
	std::vector<u8> prg_rom, chr_rom;
	regbit_t flags6;
	int from_file(const std::string& file_name);
};

#endif
