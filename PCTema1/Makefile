all: send recv

link_emulator/lib.o:
	$(MAKE) -C link_emulator

utils.o: utils.c
	gcc -std=c99 -c utils.c -o utils.o

recv.o: recv.cpp
	g++  -Wall -g -c recv.cpp -o recv.o

send.o: send.c
	gcc -Wall -g -c send.c -o send.o

send: send.o link_emulator/lib.o utils.o
	gcc -std=c99 -g utils.o send.o link_emulator/lib.o -o send

recv: recv.o link_emulator/lib.o utils.o
	g++ -g utils.o recv.o link_emulator/lib.o -o recv




clean:
	$(MAKE) -C link_emulator clean
	-rm -f *.o send recv
