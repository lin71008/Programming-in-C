CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Werror -Wextra
CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
LDFLAGS = -lm

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))
TARGETS = hw0101 hw0102 hw0103 hw0105

hw0101: hw0101.o
hw0102: hw0102.o
hw0103: hw0103.o
hw0104: hw0104.o
hw0105: hw0105.o

$(OBJECTS): %o: %c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: $(%)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean
.DEFAULT_GOAL := all
FORCE:

all: $(TARGETS)
	@echo "All program have been compiled."

install:
	@echo "Nothing needs to be installed."

EXEC = $(filter $(patsubst %, %.exe, $(TARGETS)),$(wildcard *.exe))

$(EXEC): FORCE
	@echo "=== Start test program $@ ==="
	@echo "=== Test with example input ==="
	@cat $(patsubst %.exe, ./test/%.input, $@)
	@echo "=== Result ==="
	./$@ <$(patsubst %.exe, ./test/%.input, $@) | diff - $(patsubst %.exe, ./test/%.output, $@) --color=always -s

test: $(EXEC)
	@echo "================================"
	@echo "All program have been tested."

clean:
	rm -f $(OBJECTS)

release: FORCE
	7z a ./release/$(shell date --utc +"%m-%d-%Y-%H-%M-%S").zip $(filter-out %zip $(OBJECTS) $(EXEC),$(wildcard *))
