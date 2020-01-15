CC = gcc
CFLAGS = -Wall -ansi -g -I$(IDIR)

IDIR = ./includes/
SRCDIR = ./src/
BINDIR = ./bin/

all: $(BINDIR)contactList

$(BINDIR)contactList: $(BINDIR)main.o $(BINDIR)contactList.o
	$(CC) $(BINDIR)main.o $(BINDIR)contactList.o -o $@

$(BINDIR)main.o: $(SRCDIR)main.c
	$(CC) $(CFLAGS) -c $(SRCDIR)main.c -o $@

$(BINDIR)contactList.o: $(SRCDIR)contactList.c
	$(CC) $(CFLAGS) -c $(SRCDIR)contactList.c -o $@

memtest: all
	valgrind ./bin/contactList

clean:
	rm bin/*.o ./bin/contactList ./bin/myContactList.db
