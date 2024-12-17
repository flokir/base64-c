.PHONY: all
CC=gcc
FLAGS=-std=c99 -Wall -pedantic -D_POSIX_C_SOURCE=200809L

build/cli_base64.o: cli/cli_base64.c cli/cli_base64.h include/base64_lookup.h include/base64.h
	$(CC) $(FLAGS) -Iinclude -c -o $@ $<
	
build/%.o: src/%.c include/base64_lookup.h include/base64.h
	$(CC) $(FLAGS) -Iinclude -c -o $@ $<

build/cli_base64: build/cli_base64.o build/base64_lookup.o build/base64.o
	$(CC) $(FLAGS) -o $@ $^

build/test.o: tests/test.c include/base64_lookup.h include/base64.h
	$(CC) $(FLAGS) -Iinclude -c -o $@ $<

build/test: build/test.o build/base64_lookup.o build/base64.o
	$(CC) $(FLAGS) -o $@ $^

all: build/cli_base64 build/test

clean:
	rm -rf build/*
