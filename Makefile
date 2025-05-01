
CC = gcc
CFLAGS = -std=c89 -Wall -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

btcinc: $(SRC)
	$(CC) $(CFLAGS) -o btcinc $(SRC)

clean:
	rm -f btcinc
