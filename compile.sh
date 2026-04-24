#!/usr/bin/env bash

set -e

echo "Compiling Zulu..."
gcc -Wall -Wextra -pedantic -Werror -O3 -flto src/zulu.c src/ui.c src/core.c -o zulu
strip zulu

echo "Success"
