SHELL = /usr/bin/env bash

.PHONY: clean compile strip
CC = gcc
CCFLAGS = -Wall -Wextra -pedantic -Werror -O3 -flto -MMD -MP

SRC = src/zulu.c src/ui.c src/core.c
OBJ = $(SRC:src/%.c=build/%.o)
DEPS = $(OBJ:.o=.d)

zulu: $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ)

build/%.o: src/%.c | build
	$(CC) $(CCFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -f zulu build/*.o build/*.d

-include $(DEPS)

compile:
	./compile.sh
	
strip:
	strip zulu
