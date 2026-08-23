CC = gcc

CFLAGS = -Wall -Wextra -std=c11

TARGET = allocator

SRC = src/allocator.c src/main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

CFLAGS = -Wall -Wextra -Werror -Iinclude