CC = gcc
CFLAGS = -Wall
FLAGS = -lallegro_font -lallegro_color -lallegro_ttf -lallegro_primitives -lallegro

ifeq ($(OS), Windows_NT)
    TARGET_EXT = .exe
    RM = del /Q
else
    TARGET_EXT =
    RM = rm -f
endif

all: klondike$(TARGET_EXT)

klondike$(TARGET_EXT): klondike.o telag.o
	$(CC) $(CFLAGS) klondike.o telag.o $(FLAGS) -o klondike$(TARGET_EXT)

klondike.o: klondike.c funcoes.h
	$(CC) $(CFLAGS) -c klondike.c 

telag.o: telag.c telag.h
	$(CC) $(CFLAGS) -c telag.c

run: klondike$(TARGET_EXT)
	./klondike$(TARGET_EXT)

clean:
	$(RM) klondike.o telag.o klondike$(TARGET_EXT)