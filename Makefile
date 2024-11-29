CC=gcc
CFLAGS=-Llib -Iinclude -lraylib -lopengl32 -lgdi32 -lwinmm

all: main.c convert.c gui.c
	$(CC) main.c convert.c gui.c -o main $(CFLAGS)

run: main.c convert.c gui.c
	$(CC) main.c convert.c gui.c -o main $(CFLAGS)
	.\main.exe

clean: 
	rm main