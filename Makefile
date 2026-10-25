CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -D_DEFAULT_SOURCE -Iinclude
src := $(wildcard src/*.c)
OUT := build/main

.PHONY : corganizer run

corganizer:
	mkdir -p build
	$(CC) $(CFLAGS) $(src) -o $(OUT)
run:
	./$(OUT) $(filter-out $@,$(MAKECMDGOALS))

%:
	@:

clean:
	rm -rf $(OUT)
