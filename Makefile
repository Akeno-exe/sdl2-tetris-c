CC = clang
CFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs)

tetris: tetris.c
	$(CC) $(CFLAGS) tetris.c -o tetris $(LIBS)

run: tetris
	./tetris