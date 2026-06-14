SHELL = /usr/bin/env bash

.PHONY: clean strip all

CC = gcc
CCFLAGS = -Wall -Wextra -pedantic -Werror -O2 -flto -MMD -MP
OUT = ~/.local/bin/zulu

SRC = src/zulu.c src/core.c src/ui.c
OBJ = $(SRC:src/%.c=build/%.o)
DEPS = $(OBJ:.o=.d)

all: $(OBJ)
	$(CC) $(CCFLAGS) -o $(OUT) $(OBJ)

build/%.o: src/%.c | build
	$(CC) $(CCFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -f build/*.o build/*.d

-include $(DEPS)
	
strip:
	strip $(OUT)
