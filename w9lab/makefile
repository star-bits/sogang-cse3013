all: tetris

tetris: tetris.o
	gcc tetris.o -lncurses

tetris.o: tetris.c tetris.h
	gcc -c tetris.c

clean:
	rm a.out *.o