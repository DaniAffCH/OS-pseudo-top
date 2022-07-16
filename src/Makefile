CC=gcc
CCOPTS=--std=gnu99 -Wall -g -D DEBUG
LINKED=-lncurses 

OBJS= ncurses_handler.o update.o linked_list.o

HEADERS=types.h ncurses_handler.h update.h linked_list.h

BINDIR = ../bin

ENTRY=pseudo_top.c
BINS=pseudo_top

.phony: clean all


all:	$(BINS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

$(BINS): $(OBJS) $(ENTRY)
	$(CC) $(CCOPTS) -o $(BINDIR)/$@ $^ $(LINKED) 

clean:
	rm -rf *.o *~ $(BINS)

