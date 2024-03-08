INCLUDE = -I.

CFLAGS = -O3 -Wall -Wextra -Werror
TARGETS = $(wildcard *.c)
EXE = main.exe

all:
	gcc $(CFLAGS) -o $(EXE) $(TARGETS) -lm

test:
	./$(EXE)

clean:
	rm -f main.exe output.raw

.PHONY: all test clean
