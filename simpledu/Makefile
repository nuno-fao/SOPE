CC = gcc
CFLAGS = -Wall
OBJS = simpledu.o funcs.o registers.o
XHDRS = simpledu.h funcs.h registers.h
EXEC = simpledu

$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	@$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	@-rm $(OBJS) $(EXEC)