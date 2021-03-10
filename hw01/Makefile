CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Werror -Wextra
CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-unused-but-set-variable
LDFLAGS = -lm

OBJECTS = $(patsubst %c,%o,$(wildcard *.c))
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

.DEFAULT_GOAL := all
.PHONY: test clean release
FORCE:

install:
	@echo "Nothing needs to be installed."

all: $(TARGETS)
	@echo "All program have been compiled."

TPATH = ./test
TFI = $(patsubst %exe,$(TPATH)/%input,$@)
TFO = $(patsubst %exe,$(TPATH)/%output,$@)

EXEC = $(filter $(patsubst %,%.exe, $(TARGETS)),$(wildcard *.exe))

$(EXEC): FORCE
	@echo "=== Start test program $@"
	@echo "=== Test with example input"
	@cat $(TFI)
	@echo "=== Result"
	@./$@ <$(TFI)|diff - $(TFO) --color=always -s
	@echo "================================"

test: $(EXEC)
	@echo "All program have been tested."

clean:
	rm -f $(OBJECTS) $(EXEC)

# ignore below

pre-release: DOCUMENT = $(filter-out $(EXCLUDE),$(wildcard *.md))
pre-release: EXCLUDE = README% Note%

release: VERSION = $(shell date --utc +"%m-%d-%Y-%H-%M-%S")
release: SOURCE = $(filter-out $(EXCLUDE) $(OBJECTS) $(EXEC),$(wildcard *))
release: EXCLUDE = Makefile %md pre-release release temp
release: INCLUDE = README.md Note.md

pre-release: CFLAGS = -std=gnu11 -O2 -Wall -Werror -Wextra

pre-release: all test
	mkdir -p pre-release
	$(foreach doc,$(DOCUMENT),cp $(doc) ./pre-release/$(doc);)
	$(foreach doc,$(DOCUMENT:%=./pre-release/%),pandoc --pdf-engine=xelatex --template owo.tex $(doc) -o $(doc:%md=%pdf);)
	$(foreach doc,$(DOCUMENT:%=./pre-release/%),rm $(doc);)
	sed '/ignore below/Q' Makefile > ./pre-release/Makefile

release: pre-release clean
	7z u ./release/$(VERSION).zip $(SOURCE) $(INCLUDE)
	7z u ./release/$(VERSION).zip ./pre-release/*
	@echo "================================"
	@echo "The release was success."
	@echo "Available in ./release/$(VERSION).zip"
