#include "rom.h"
#include<fstream>
#include<iostream>
#include<cstdio>
int Rom::from_file(const std::string& file_name) {
	regbit_t flags;
	std::vector<int8_t> buff;
	std::ifstream file(file_name, std::ios_base::in | std::ios_base::binary);

	// odczytaj rozmiar pliku
	file.seekg(0, file.end);
	int len = file.tellg();
	buff.resize(len);
	file.seekg(0, file.beg);

	// wczytaj ROM
	for(int j=0; j < len; ++j) {
		file >> buff[j];
	};
	prg_rom.resize(16384 * buff[4]);
	chr_rom.resize(8192 * buff[5]);
	if(buff[4] < 2) {
		short_rom = true;
	} else
		short_rom = false;
	fprintf(stderr, "PRGROM %d kB, CHRROM %d kB\n", 16*buff[4], 8*buff[5]);
	fprintf(stderr, "%c%c%c", buff[0], buff[1], buff[2]);
	flags6.val = buff[6];

	for(int j=0; j < prg_rom.size(); ++j) {
		prg_rom[j] = buff[16+j];
	};

	for(int j=0; j < chr_rom.size(); ++j) {
		chr_rom[j] = buff[16 + prg_rom.size() + j];
	};		
	std::cout << "Rom loaded." << std::endl;
	
	return 0;
};

u8 Rom::read_pat0(u16 addr) {
	return chr_rom[addr];
};

u8 Rom::read_pat1(u16 addr) {
	return chr_rom[addr];
};


u8 Rom::read(u16 addr) {
//	fprintf(stderr, "czytam z romu, addr %x .\n", addr);
	switch(addr) {
		case 0x8000 ... 0xBFFF:
			return prg_rom[addr - 0x8000];
			break;

		case 0xC000 ... 0xFFFF:
			return short_rom ? prg_rom[addr - 0xC000] : prg_rom[addr-0x8000];
			break;
		
		default:
			return 0;
			break;
	};
};

		
