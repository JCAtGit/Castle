# Use strict compiler flags:
Catching dangerous compiling code.

## Catch most issues
gcc -Wall -Wextra -pedantic -std=c99 program.c

## Even stricter
gcc -Wall -Wextra -pedantic -std=c99 -Werror -Wshadow -Wconversion program.c

## Runtime checks
gcc -fsanitize=address -fsanitize=undefined program.c


# User static analysis

clang --analyze program.c
cppcheck --enable=all program.c