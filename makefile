CFLAGS= -Wall -Wextra -std=c11 -pedantic -ggdb `pkg-config --cflags libpng`
LIBS= `pkg-config --libs libpng`
CC= gcc

fontgen: main.c
	$(CC) $(CFLAGS) main.c -ofontgen.exe $(LIBS)
