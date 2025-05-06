CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -g 

all:
	$(CC) main.c other.c $(CFLAGS) -o test

.PHONY: clean
clean:
	if [ -f test ]; then rm test; fi
	if [ -f core ]; then rm core; fi
