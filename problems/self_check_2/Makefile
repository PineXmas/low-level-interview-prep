CC = gcc
CFLAGS = -Wall -Wextra -I../../shared

all: test.out

test.out: test_runner.c
	$(CC) $(CFLAGS) test_runner.c -o test.out

clean:
	rm -f test.out