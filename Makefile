CC = gcc
CFLAGS ?= -std=c89 -Wall -Iinclude
SRC = $(wildcard src/*.c)
TARGET = btcinc

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
