OFILES  = main.o body.o gifenc.o nbody.o quad.o bhtree.o gui.o
TARGET  = ../nbodysimul
CC 	    = gcc
CFLAGS  = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99
LDFLAGS = -lm

.PHONY: all clean run archive


all: $(TARGET)
clean:
	rm -f $(OFILES) $(TARGET)

$(TARGET): $(OFILES)
	$(CC) $(OFILES) -o $(TARGET) $(LDFLAGS)

bhtree.o: bhtree.c bhtree.h quad.h body.h gifenc.h
body.o: body.c body.h
gifenc.o: gifenc.c gifenc.h
nbody.o: nbody.c body.h nbody.h gifenc.h bhtree.h quad.h
quad.o: quad.c gifenc.h quad.h
gui.o: gui.c gui.h
main.o: main.c nbody.h gifenc.h
