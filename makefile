CC=gcc
CFLAGS=-Wall -march=native -mtune=native
all:
	$(CC) $(CFLAGS) -o fuzzy_run run.c rule_base.c fuzzifier.c inf_engine.c defuzzifier.c
