all: Megamaniac

LIBS=-lzdk04 -lncurses -lm -lrt
DIRS=-I../ZDK04 -L../ZDK04
FLAGS=-std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror

clean:
	rm *.exe

rebuild: clean all

SRC=submission.c game.c player.c alien.c
HDR=game.h player.h alien.h

Megamaniac: $(SRC) $(HDR) ../ZDK04/libzdk04.a
	gcc $(FLAGS) $(SRC) -o $@ $(DIRS) $(LIBS)
