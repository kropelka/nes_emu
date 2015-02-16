HEADERS = $(wildcard *.h)
CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -g -Wall 
CCFLAGS = -std=c99  
LIBS = -lncurses `sdl-config --cflags --libs` 


all: nes_emu

nes_emu: main.o cpu.o mem.o rom.o m6502.o ppu.o window.o new_ppu.o debug.o
	$(CXX) $(CXXFLAGS) main.o m6502.o cpu.o mem.o rom.o ppu.o window.o new_ppu.o debug.o -o nes_emu $(LIBS)

mem.o: mem.cpp
	$(CXX) $(CXXFLAGS) -c mem.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

cpu.o: cpu.cpp
	$(CXX) $(CXXFLAGS) -c cpu.cpp

m6502.o: m6502/M6502.c
	$(CC) $(CCFLAGS) -c m6502/M6502.c -o m6502.o
	
rom.o: rom.cpp
	$(CXX) $(CXXFLAGS) -c rom.cpp

new_ppu.o: new_ppu.cpp
	$(CXX) $(CXXFLAGS) -c new_ppu.cpp


ppu.o: ppu.cpp
	$(CXX) $(CXXFLAGS) -c ppu.cpp

window.o: window.cpp
	$(CXX) $(CXXFLAGS) -c window.cpp

debug.o: debug.cpp
	$(CXX) $(CXXFLAGS) -c debug.cpp -o debug.o


clean:
	rm *.o


