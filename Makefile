CC := gcc
CFLAGS := -Wall -Wextra

all:
	$(CC) main.c $(CFLAGS) -o test

.PHONY: clean
clean:
	rm test
