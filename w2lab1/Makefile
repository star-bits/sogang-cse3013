CC = gcc
target = main
objects = main.o dog.o blackcow.o turtle.o

$(target): $(objects)
	$(CC) -o $(target) $(objects)

$(objects): header.h

.PHONY: clean
clean:
	rm $(target) $(objects)
