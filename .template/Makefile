CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Werror -Wextra
LDFLAGS = -lm

OBJECTS = $(patsubst %.c, %.o, $(wildcard hw*.c))
TARGETS = hw0*01 hw0*02 hw0*03

hw0*01: hw0*01.o
hw0*02: hw0*02.o share.o
hw0*03: hw0*03.o share.o extra.o

$(OBJECTS): %o: %c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: $(%)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean
.DEFAULT_GOAL := all

all: $(TARGETS)
	@echo All program has been compiled.

install:
	@echo Nothing needs to be installed.

test:
	@echo Avaliable program: $(TARGETS)

clean:
	-rm $(OBJECTS)
