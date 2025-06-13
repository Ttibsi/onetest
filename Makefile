CFLAGS := -Wall -Wextra -g 

all:
	g++ main.cpp $(CFLAGS) -std=c++20 -o test
	gcc main.c $(CFLAGS) -std=c23 -o test2

.PHONY: clean
clean:
	if [ -f test ]; then rm test; fi
	if [ -f test2 ]; then rm test; fi
	if [ -f core ]; then rm core; fi
