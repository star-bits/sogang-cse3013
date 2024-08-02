CC = g++
CFLAGS = -g
TARGET = main
OBJECTS = main.o string.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): string.h

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS)