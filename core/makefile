CXX = g++
CXXFLAGS = -o -g
CFLAGS = -g

Objects=cpu.o memory.o test.o

nes:$(Objects)
	g++ -o nes $(Objects) -g

cpu.o:../global.h memory.h
memory.o:../global.h
test.o:../global.h memory.h cpu.h

.PHONY:test
test:
	./nes

.PHONY:clean
clean:
	rm nes $(Objects)
