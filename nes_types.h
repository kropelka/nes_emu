/*
 * podstawowe typy danych uzywane w emulatorze NESa
 */
#ifndef NES_TYPES_H
#define NES_TYPES_H

#include<cstdint>

typedef uint8_t u8;
typedef uint16_t u16;

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


/*! rejestry PPU
 */
union ppuctrl_t {
	u8 val;
	struct {
		u8 nt_addr:2;  // adres nametable, 0 -> 0x2000, 1 -> 0x2400, 2 -> 0x2800, 3 -> 0x2c00)
		u8 vram_inc:1; /* zmiana adresu VRAM przy odczycie/zapisie 
					 * 0: dodaj jeden, czyli w prawo  1: dodaj 32, czyli w dol
					 */
		u8 sprite_pattern_table:1; // adres tablicy sprite'ow 8x8  0: 0x0000  1: 0x1000
		u8 bg_pattern_table:1;	// tablica sprite'ow dla tla  0: 0x0000 1: 0x1000
		u8 sprite_size:1;   // 0: 8x8  1: 8x16
		u8 ppu_master_slave:1;
		u8 nmi_on_vblank:1;
	};
};

union ppumask_t {
	u8 val;
	struct {
		u8 grayscale:1;
		u8 bg_on_left:1;
		u8 sprites_on_left:1;
		u8 show_bg:1;
		u8 show_sprites:1;
		u8 emph_red:1;
		u8 emph_green:1;
		u8 emph_blue:1;
	};
};

union ppustatus_t {
	u8 val;
	struct {
		u8 lsb_last:5;  // 5 mniejszych bitow ostatnio zapisanych do PPU
		u8 sprite_overflow:1;
		u8 hit:1;
		u8 vblank_started:1;
	};
};

union ppu_addrbus_t {
	u16 val;
	struct {
		u8 lo:8;
		u8 hi:8;
	};
};


#endif
