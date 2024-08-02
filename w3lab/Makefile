CC = g++
CFLAGS = -W -g
TARGET = main
OBJECTS = main.o array.o rangearray.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.cpp Array.h RangeArray.h
	$(CC) $(CFLAGS) -c main.cpp

array.o: array.cpp Array.h
	$(CC) $(CFLAGS) -c array.cpp

rangearray.o: rangearray.cpp RangeArray.h Array.h
	$(CC) $(CFLAGS) -c rangearray.cpp

.PHONY: clean
clean:
	rm $(TARGET) $(OBJECTS)
