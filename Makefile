CFLAGS := -Wall -Wextra -g 

all:
	gcc main.c $(CFLAGS) -std=c23 -o test

.PHONY: clean
clean:
	if [ -f test ]; then rm test; fi
	if [ -f core ]; then rm core; fi
