CC := clang++
CFLAGS := -Wall -Wextra -std=c++20 -g 

all:
	$(CC) main.cpp $(CFLAGS) -o test

.PHONY: clean
clean:
	if [ -f test ]; then rm test; fi
	if [ -f core ]; then rm core; fi
