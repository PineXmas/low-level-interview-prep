CC = gcc
CFLAGS = -Wall -Wextra -I../../shared

all: test.out

test.out: test_runner.c bit_manip.c
	$(CC) $(CFLAGS) bit_manip.c test_runner.c -o test.out

clean:
	rm -f test.out