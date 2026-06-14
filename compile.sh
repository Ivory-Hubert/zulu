#!/usr/bin/env bash

set -e

echo "Compiling Zulu..."
gcc -Wall -Wextra -pedantic -Werror -O2 -flto src/zulu.c src/ui.c src/core.c -o ~/.local/bin/zulu
strip ~/.local/bin/zulu

echo "Success"
