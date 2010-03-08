# Makefile
CC=gcc
CFLAGS=-Wall -O0
VERSION=1.02

PREFIX=/usr/local/
MANPREFIX=/usr/man/man1/
MANPAGE=skybadge.1
PROGS=skybadge

MAKEFLAGS += --no-print-directory

all: skybadge manuel

skybadge: skybadge.o badge.o serial.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $+

manuel: man/$(MANPAGE)
	@if [ -f man/$(MANPAGE) ]; then \
	  gzip $+; \
	fi

man/$(MANPAGE):
	@if [ -f man/$(MANPAGE).gz ]; then \
	  gunzip man/$(MANPAGE).gz; \
	fi


# Dés/Installation des binaires et page de manuel

install: 
	cp $(PROGS) $(PREFIX)bin/
	cp man/$(MANPAGE).gz $(MANPREFIX)

uninstall:
	rm -rf $(PREFIX)bin/skybadge
	rm -rf $(MANPREFIX)$(MANPAGE).gz

# Divers
dist: clean
	cd .. &&\
	tar -czvf Skybadge-$(VERSION).tgz Skybadge-$(VERSION)/

clean:
	rm -f *.o core *~ *% *#	
	rm -f *.swp
	rm -f $(PROGS)
	@if [ -f man/$(MANPAGE).gz ]; then \
		gunzip man/$(MANPAGE).gz; \
	fi
	
new: clean all

# end of Makefile
