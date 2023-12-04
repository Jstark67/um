#
# Makefile for the UM
# 
CC = gcc

IFLAGS  = -I/comp/40/build/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64
LDLIBS  = -lcii40-O2 -l40locality -lm -lbitpack

EXECS   = um

all: $(EXECS)

um: um.o  
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)


# To get *any* .o file, compile its .c file with the following rule.
uexecute.o: uexecute.c umemory.o
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o