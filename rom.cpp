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
