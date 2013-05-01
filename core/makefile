CXX = g++
CXXFLAGS = -o -g
CFLAGS = -g

Objects=cpu.o memory.o file.o ppu.o test.o

nes:$(Objects)
	g++ -o nes $(Objects) -g

cpu.o:../global.h memory.h
memory.o:../global.h
file.o:../global.h
ppu.o:../global.h
test.o:../global.h memory.h cpu.h

.PHONY:test
test:
	./nes

.PHONY:clean
clean:
	rm nes $(Objects)

.PHONY:debug
debug:
	g++ -o nes cpu.cpp file.cpp memory.cpp ppu.cpp test.cpp -g
