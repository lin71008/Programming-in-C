CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Werror -Wextra -Wno-unused
LDFLAGS = -lm

OBJECTS = $(patsubst %c, %o, $(wildcard *.c))
TARGETS = hw0501 hw0502 hw0503

hw0501: hw0501.o
hw0502: hw0502.o
hw0503: hw0503.o

################################################################################

.DEFAULT_GOAL := all
.PHONY: install clean
FORCE:

usage:
	@echo "Usage: make [Options]"
	@echo "Options:"
	@echo "  usage    Display this information."
	@echo "  install  Install dependency package."
	@echo "  all      Build all targets."
	@echo "  [target] Build specified target that listing below."
	@echo "  	          target: $(TARGETS)"
	@echo "  clean    remove all build result."
	@echo ""
	@echo "DEFAULT OPTIONS : all"

install:
	@echo "================================"
	@echo "Please install libcurl and add it to the compiler search path by yourself."

$(OBJECTS): %o:%c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: $(%)
	$(CC) -o $@ $^ $(LDFLAGS)

all: $(TARGETS)
	@echo "================================"
	@echo "All program have been compiled."

clean:
	rm -f $(OBJECTS) $(TARGETS) $(TEMPFILE)

# ignore below

pre-release-exe: CFLAGS = -std=gnu11 -O2 -Wall -Werror -Wextra
pre-release-exe: all clean
	@

pre-release-doc: DOCUMENT = $(filter-out $(EXCLUDE),$(wildcard *.md))
pre-release-doc: EXCLUDE = README% Note%

pre-release-doc:
	mkdir -p pre-release
	$(foreach doc,$(DOCUMENT),cp $(doc) ./pre-release/$(doc);)
	$(foreach doc,$(DOCUMENT:%=./pre-release/%),pandoc --pdf-engine=xelatex --template owo.tex $(doc) -o $(doc:%md=%pdf);)
	$(foreach doc,$(DOCUMENT:%=./pre-release/%),rm $(doc);)
	sed '/ignore below/Q' ./Makefile > ./pre-release/Makefile
	test -e ./LICENSE && cp ./LICENSE ./pre-release/LICENSE
	test -e ./Note.md && cp ./Note.md ./pre-release/Note.md

release: VERSION := $(shell date --utc +"%m-%d-%Y-%H-%M-%S")
release: SOURCE := $(wildcard *.[^(exe|out)])

release: pre-release-exe pre-release-doc clean
	7z a ./release/$(VERSION).zip ./pre-release/*
	7z u ./release/$(VERSION).zip $(SOURCE)
	rm -r -f ./pre-release/
	@echo "================================"
	@echo "The release was success."
	@echo "Available in ./release/$(VERSION).zip"
