CC=gcc
CCOPTS=--std=gnu99 -Wall -g -D NODEBUG -D TH -ggdb3
LINKED=-lncurses 

OBJS= ncurses_handler.o update.o linked_list.o action_handler.o input_listener.o exit_routine.o

HEADERS=types.h ncurses_handler.h update.h linked_list.h action_handler.h input_listener.h exit_routine.h

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

