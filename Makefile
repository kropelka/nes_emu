HEADERS = $(wildcard *.h)
CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -g 
CCFLAGS = -std=c99  
LIBS = `sdl-config --cflags --libs` 


all: nes_emu

nes_emu: main.o cpu.o mem.o rom.o m6502.o ppu.o window.o
	$(CXX) $(CXXFLAGS) main.o m6502.o cpu.o mem.o rom.o ppu.o window.o -o nes_emu $(LIBS)

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

ppu.o: ppu.cpp
	$(CXX) $(CXXFLAGS) -c ppu.cpp

window.o: window.cpp
	$(CXX) $(CXXFLAGS) -c window.cpp

debug.o: m6502/Debug.c
	$(CC) $(CCFLAGS) -c m6502/Debug.c -o debug.o


clean:
	rm *.o


