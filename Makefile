CC=gcc
CCOPTS=--std=gnu99 -Wall 
LINKED=-lncurses 

OBJS= ncurses_handler.o

HEADERS=types.h ncurses_handler.h


ENTRY=pseudo_top.c
BINS=pseudo_top

.phony: clean all


all:	$(BINS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

$(BINS): $(OBJS) $(ENTRY)
	$(CC) $(CCOPTS) -o $@ $^ $(LINKED) 

clean:
	rm -rf *.o *~ $(BINS)

