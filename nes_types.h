/*
 * podstawowe typy danych uzywane w emulatorze NESa
 */
#ifndef NES_TYPES_H
#define NES_TYPES_H

#include<cstdint>

typedef unsigned char u8;
typedef unsigned short u16;

/*! rejestr z mozliwoscia dostepu do poszczegolnych bitow
 * oraz czesci 3-5 bitow (uzywanych przez PPU)
 */
union regbit_t {
	u8 val;
	struct {
		u8 b0:1;
		u8 b1:1;
		u8 b2:1;
		u8 b3:1;
		u8 b4:1;
		u8 b5:1;
		u8 b6:1;
		u8 b7:1;
	};
	struct {
		u8 lower:3;
		u8 higher:5;
	};
};

struct reg16_t {
	u8 lo, hi;
	reg16_t() : lo(0), hi(0) {};
	reg16_t(u16 val) : lo(val & 0xFF), hi((val >> 8) & 0xFF) {};
	u16 get() {return 256*hi + lo;};
	void set_lo(u8 val) { lo = val; };
	void set_hi(u8 val) { hi = val; };
};


/*! rejestry PPU
 */

struct ppuctrl_t {
	ppuctrl_t() { set(0); };
	u8 get() {
		return nt_addr | (vram_inc<<2) | (sprite_pattern_table<<3) |
			(bg_pattern_table<<4) | (sprite_size << 5) | (ppu_master_slave << 6) |
			(nmi_on_vblank << 7);
	};
	void set(u8 val) {
		nt_addr = val & 3;
		vram_inc = val & 4;
		sprite_pattern_table = val & 8;
		bg_pattern_table = val & 16;
		sprite_size = val & 32;
		ppu_master_slave = val & 64;
		nmi_on_vblank = val & 128;
	};
	u8 nt_addr;
	bool vram_inc;
	bool sprite_pattern_table;
	bool bg_pattern_table;
	bool sprite_size;
	bool ppu_master_slave;
	bool nmi_on_vblank;
};



struct ppumask_t {
	bool grayscale;
	bool bg_on_left;
	bool sprites_on_left;
	bool show_bg;
	bool show_sprites;
	bool emph_red;
	bool emph_green;
	bool emph_blue;
	void set(u8 val) {
		grayscale = val & 1;
		bg_on_left = val & 2;
		sprites_on_left = val & 4;
		show_bg = val & 8;
		show_sprites = val & 16;
		emph_red = val & 32;
		emph_green = val & 64;
		emph_blue = val & 128;
	};
	u8 get() {
		return grayscale | (bg_on_left<<1) | (sprites_on_left<<2) | (show_bg<<3) |
			(show_sprites<<4) |(emph_red<<5) | (emph_green<<6) | (emph_blue<<7);
	};
};

union ppustatus_t {
	u8 lsb_last;
	bool sprite_overflow;
	bool hit;
	bool vblank;
	void set(u8 val) {
		lsb_last = 0b00011111 & val;
		sprite_overflow = val & 32;
		hit = val & 64;
		vblank = val & 128;
	};
	u8 get() {
		return lsb_last | (sprite_overflow << 5) | (hit<<6) | (vblank<<7);
	};
};

struct ppu_addrbus_t {
	u8 lo, hi;
	u16 tmp;
	ppu_addrbus_t() { lo = hi = 0;};
	u16 get() {	return 256*hi + lo;	};
	void set_lo(u8 val) { lo = val; };
	void set_hi(u8 val) { hi = val; };
	void add(u8 val) {
		tmp = get();
		tmp += val;
		lo = val & 0xFF;
		hi = (val >> 8) & 0xFF;
	};
};


#endif
