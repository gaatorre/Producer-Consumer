CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g -pthread
OBJECTS = PC.o

.PHONY: all
all: prog

prog:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o PC.exe

PC.o: PC.c
	 $(CC) $(CFLAGS) PC.c -c

.Phony: clean
clean:
	rm -f $(OBJECTS) PC.exe
