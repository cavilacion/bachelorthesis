CC=gcc
CFLAGS=-Wall
CLIBS=-lm

fsa-example: src/main.c src/fsa.c src/functions-example.c src/stutter.c src/export-promela-example.c
	$(CC) $(CFLAGS) -o $(@) src/main.c src/fsa.c src/functions-example.c src/stutter.c src/export-promela-example.c $(CLIBS)

fsa-insect: src/main.c src/fsa.c src/functions-insect.c src/stutter.c src/export-promela-insect.c
	$(CC) $(CFLAGS) -o $(@) src/main.c src/fsa.c src/functions-insect.c src/stutter.c src/export-promela-insect.c $(CLIBS)
