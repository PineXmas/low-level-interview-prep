include ../../shared/common.mk

all: test.out

test.out: test_runner.c swap_2_nums.c
	$(CC) $(CFLAGS) swap_2_nums.c test_runner.c -o test.out

clean:
	rm -f test.out