#include "ppu.h"
#include<cstdio>
/*
Ppu::Ppu(MemMap* mem_) {
	mem = mem_;
	ppuctrl = &(mem->ppuctrl);
	ppumask = &(mem->ppumask);
	ppustatus = &(mem->ppustatus);
	oam_addr = &(mem->oam_addr);
	oam_data = &(mem->oam_data);
	scroll = &(mem->scroll);
	ppu_addr = &(mem->ppu_addr);
	ppu_data = &(mem->ppu_data);
};
*/


/*! ustawienie trybu mirroringu tablicy sprite'ow
 * 0 -> mirroring poziomy
 * 1 -> mirroring pionowy
 * 2 -> cztery nametable (dodatkowy VRAM w cartridge'u)
 */
void Ppu::set_nt_mirroring(unsigned mode) {
	unsigned j;
	switch(mode) {
		case 0:  // mirroring poziomy
			for(j=0; j < 0x400; ++j) { 
				ppu_mem[0x2400 + j] = &(nametables[j]);
				ppu_mem[0x2800 + j] = ppu_mem[0x2C00 + j] = &(nametables[0x400 + j]);
			};
			horiz_copy = false;
			break;

		case 1: // mirroring poziomy
			for(j=0; j < 0x400; ++j) {
				ppu_mem[0x2800 + j] = &(nametables[j]);
				ppu_mem[0x2400 + j] = ppu_mem[0x2C000 + j] = &(nametables[0x400 + j]);
			};
			horiz_copy = true;
			break;
		case 2 ... 3: // cztery nametable
			fprintf(stderr, "Not yet implemented.");		
			break;
	};
};


Ppu::Ppu() {
	scroll_latch = false;
	addr_latch = false;
	ppustatus.val = 0;
	ppuctrl.val = 0;
	ppumask.val = 0;
	horiz_copy = true;
	io_memaddr.val = 0;
	unsigned j;
	for(j=0; j < 0x400; ++j) {
		ppu_mem[0x2000 + j] = &(nametables[j]);
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



};

u8 Ppu::ppu_read(u16 addr) {
	switch(addr) {
		case 0x0000 ... 0x0FFF:
			return rom->read_pat0(addr);
			break;
		
		case 0x1000 ... 0x1FFF:
			return rom->read_pat1(addr);
			break;

		case 0x2000 ... 0x23FF:
			return nametables[addr - 0x2000];
			break;

		case 0x2400 ... 0x27FF:
			return horiz_copy ? nametables[addr - 0x2400] : nametables[addr - 0x2200];
			break;

		case 0x2800 ... 0x2BFF:
			return horiz_copy ? nametables[addr - 0x2400] : nametables[addr - 0x2800];
			break;

		case 0x2C00 ... 0x2FFF:
			return nametables[addr - 0x2800];
			break;

		case 0x3000 ... 0x3EFF:
			return ppu_read(addr - 0x1000);
			break;

		case 0x3F00 ... 0x3F1F:
			return (addr & 0x0010) ? sprite_pal[addr % 0x3F00] : bg_pal[addr % 0x3F10];
			break;

		default:
			return 0;
			break;

	};
};

void Ppu::ppu_write(u16 addr, u8 val) {
	switch(addr) {
		case 0x2000 ... 0x23FF:
			nametables[addr - 0x2000] = val;
			break;

		case 0x2400 ... 0x27FF:
			horiz_copy? nametables[addr - 0x2400] = val : nametables[addr - 0x2200] = val;
			break;

		case 0x2800 ... 0x2BFF:
			horiz_copy? nametables[addr - 0x2400] = val : nametables[addr-0x2800] = val;
			break;

		case 0x2C00 ... 0x2FFF:
			nametables[addr - 0x2800] = val;
			break;

		case 0x3000 ... 0x3EFF:
			ppu_write(addr - 0x1000, val);
			break;

		case 0x3F00 ... 0x3F1F:
			addr & 0x0010 ? sprite_pal[addr % 0x3F00] = val : bg_pal[addr % 0x3F10] = val;
			break;
	};
};



		






/*
void Ppu::connect_rom(const Rom& rom) {
	u16 j = 0;
	// CHR ROM
	for(j=0; j < 0x2000; ++j) {
		ppu_mem[j] = (u8*) (rom.chr_rom.data()) + j;
	};
	set_nt_mirroring(rom.flags6.b0 + 2*rom.flags6.b1);
};
*/

void Ppu::connect_rom(Rom* r) {
	rom = r;
};

/*
int next_scanline(int scanline) {
	switch(scanline) {
		case 
};
*/
/*
void Ppu::tick() {
	switch(scanline) {
		case 0 ... 19:   // nic sie tutaj nie dzieje
			if(x++==341) {
				x = 0;
				++scanline;
			};
			break;
		case 20:
		case 21 ... 260:
			

};
*/


u8 Ppu::mem_read(u16 addr) {
	u8 tmp;
	switch(0x2000 + (addr % 8)) {
		case 0x2000:  // ppuctrl
			return ppuctrl.val;
			break;

		case 0x2001: // ppumask
			return ppumask.val;
			break;

		case 0x2002: // ppustatus
			scroll_latch = addr_latch = false;  // odczytanie PPUSTATUS powoduje rozpoczecie wczytywania adresow sprite/vram od nowa
			tmp = ppustatus.val;
			ppustatus.vblank = 0;
			return tmp;
			break;

		case 0x2003:
			return oam_addr.val;
			break;

		case 0x2004:  // sprite memory data
			return sprite_mem[(oam_addr.val)++];
			break;

		case 0x2007:
			tmp = ppu_read(io_memaddr.val);
			io_memaddr.val += ppuctrl.vram_inc ? 32 : 1;
			break;

		default:
			return 0;
			break;
	};
};




void Ppu::mem_write(u16 addr, u8 val) {
	static u16 full_addr;

	switch(0x2000 + (addr % 8)) {
		case 0x2002:
			scroll_latch = addr_latch = false;
			ppustatus.val = val;
			break;

		case 0x2004: // sprite memory data
			sprite_mem[oam_addr.val] = val;
			(oam_addr.val)++;
			break;

		case 0x2005: // scroll
			if(!scroll_latch) {
				run_scroll_x = val;
			} else  {
				scroll_y = val;
			};
			break;

		case 0x2006:
			fprintf(stderr, "sel vram\n");
			if(!addr_latch) {
				addr_latch = true;
				io_memaddr.hi = val;	
			} else {
				addr_latch = false;
				io_memaddr.lo = val;
			};
	 		break;

		case 0x2007:
			//fprintf(stderr, "Zapis do ppu vram %x", io_memaddr.val);
			ppu_write(io_memaddr.val, val);
			io_memaddr.val += (ppuctrl.vram_inc) ? 32 : 1;
			break;

		default:
			break;
	};

};

/*void Ppu::draw_frame() {
};*/




void Ppu::update_bg_buff(u8 tx, u8 ty, u8 nt_nr) {
	//adres wpisu w nametable'u
	u16 nt_addr = 0x2000 + nt_nr*0x400 + 32*ty  + tx;
	u16 attr_addr = 0x23C0 + nt_nr * 0x400 + tx/4 + ty/4;
	u8 raw_sprite[16], raw_attr;
	u8 color;
	for(unsigned i=0; i < 16; ++i) {
//		raw_sprite[i] = nametables[nt_addr + i];
//		color = nametables[attr_addr];
		raw_sprite[i] = ppu_read(ppu_read(nt_addr) + i);
		color = ppu_read(attr_addr);
		if((tx % 4) < 2) {   // lewa polowka
			if((ty % 4) < 2) {   // lewa gorna polowka
				raw_attr = (color << 6) >> 6;
			} else {    // lewa dolna polowka
				raw_attr = (color << 2) >> 6;
			};
		} else {   // prawa polowka
			if((ty % 4) < 2) {  // prawa gorna polowka
				raw_attr = (color << 4) >> 6;
			} else {  // prawa dolna
				raw_attr = color >> 6;
			};
		};
	};

	for(unsigned i=0; i < 8; ++i) {
		for(unsigned j=0; j < 8 ; ++j) {
			bg_buff[SIZE_X*(8*ty + i) + 8*tx + j]  = ((raw_sprite[i] << (7-j)) >> 7) + 2 * 
				((raw_sprite[8+i] << (7-j)) >> 7 )+
				4 * raw_attr;
		};
	};
};

void Ppu::showbgbuff() {
	for(unsigned x = 0; x < 240; x++) {
		fprintf(stderr, "\n");
		for(unsigned y = 0; y < 256; y++) {
			if(bg_buff[256*y + x] !=0) {
				fprintf(stderr, "*");
			} else
				fprintf(stderr, " ");
		};
	};
	fprintf(stderr,"\n");
}

	


/*
	u16 nt_addr_base = 0x2000;
	u8 nt_entry_x, nt_entry_y;
	u8 curr_nt;
	u8 c;
	unsigned x, y, scrolled_x, scrolled_y;
	unsigned nt_entry_x, nt_entry_y; 
	for(y=0; y < SIZE_Y; ++y) {
		curr_nt = nt_on_scanline[y];
		scrolled_y = scroll_y + y;
		nt_entry_y = scrolled_y % SIZE_Y;
		scrolled_x = scroll_x[y];

		for(x=0; x < SIZE_X/4; ++x) {
			c = *ppu_mem[nt_entry_x + (256 * nt_entry_x) + 


};

void Ppu::process() {
	if((sprite_mem[0] + 7) == scanline) {   // hit detection
		ppustatus.hit = 1;
	};
	if(ppumask.show_bg) {
		draw_bg();
	};
	if(ppumask.show_sprites) {
		// draw sprites
	};
};

*/


void Ppu::process() {
};

	


void Ppu::reset() {
	ppuctrl.val = ppumask.val = ppustatus.val = scroll.val = ppu_addr.val = 0;
};


void Ppu::dma_load(u8* sprites) {
	fprintf(stderr, "DMA!\n");
	for(int j=0; j < 0x100; ++j) {
		sprite_mem[j] = sprites[j];
	};
};

