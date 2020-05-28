CC = gcc
CFLAGS = -Wall -pthread
DEPS = funcs.h registers.h
OBJ = funcs.o registers.o
TARGETS = U1 Q1

all: U1 Q1

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

U1: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

Q1: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

clean:
	@rm *.o $(TARGETS)