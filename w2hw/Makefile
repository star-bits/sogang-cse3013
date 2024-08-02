CC = gcc
CFLAGS = -W -g
TARGET = main
OBJECTS = main.o count.o print.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): header.h

.PHONY: clean
clean:
	rm $(TARGET) $(OBJECTS)
