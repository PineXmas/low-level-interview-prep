include ../../shared/common.mk

all: test.out

test.out: test_runner.c check_power_of_2.c
	$(CC) $(CFLAGS) check_power_of_2.c test_runner.c -o test.out

clean:
	rm -f test.out