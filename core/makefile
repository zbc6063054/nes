CXX = g++
CXXFLAGS = -o -g

Objects=cpu.o memory.o test.o

nes:$(Objects)
	g++ -o nes  -g $(Objects)

cpu.o:../global.h memory.h
memory.o:../global.h
test.o:../global.h memory.h cpu.h

.PHONY:test
test:
	./nes


.PHONY:clean
clean:
	rm nes $(Objects)
