CFLAGS=-Wall -Werror

.PHONY: clean run-bitpacking run-planner

run-bitpacking: bitpacking
	./bitpacking

run-planner: planner
	./planner

run-main: main
	./main

bitpacking: bitpacking.c timelib.c
	gcc bitpacking.c timelib.c $(CFLAGS) -o bitpacking

planner: planner.c timelib.c
	gcc planner.c timelib.c $(CFLAGS) -o planner

main: main.c timelib.c
	gcc main.c timelib.c $(CFLAGS) -o main

clean:
	rm -f bitpacking planner
