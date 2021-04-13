CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Werror -Wextra -Wno-unused
LDFLAGS = -lm

OBJECTS = $(patsubst %c,%o,$(wildcard *.c))
TARGETS = mid01 mid02 mid03

mid01: mid01.o
mid02: mid02.o
mid03: mid03.o

$(OBJECTS): %o:%c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: $(%)
	$(CC) $(LDFLAGS) -o $@ $^

.DEFAULT_GOAL := all
.PHONY: install test release
FORCE:

usage:
	@echo "Usage: make [Options]"
	@echo "Options:"
	@echo "  all (default): Build all targets that listing below."
	@echo "  	targets: $(TARGETS)"
	@echo "  test: Testing all executable with example I/O in test folder."
	@echo "  clean: remove all build result."

install:
	@echo "Nothing needs to be installed."

all: $(TARGETS)
	@echo "All program have been compiled."

TPATH = ./test
TINPUT = $(patsubst %exe,$(TPATH)/%input,$@)
TOUTPUT = $(patsubst %exe,$(TPATH)/%output,$@)

EXEC = $(filter $(patsubst %,%.exe,$(TARGETS)),$(wildcard *.exe))
TIMELIMIT = 10.00s

$(EXEC): TINPUTEXIST = $(shell test -a $(TINPUT) && echo "1")
$(EXEC): TOUTPUTEXIST = $(shell test -a $(TOUTPUT) && echo "1")

$(EXEC): FORCE
	@echo "=== Start test program $@"
	$(if $(TINPUTEXIST),\
		@echo "=== Test with example input" &&  head -c 300 $(TINPUT),\
		@echo "=== No example input"\
	)
	@echo "=== Result"
	$(if $(TINPUTEXIST),\
		$(if $(TOUTPUTEXIST),\
			@timeout -k 9 $(TIMELIMIT) ./$@ <$(TINPUT)|diff - $(TOUTPUT) --color=always -B -w -s,\
			$(if $(shell timeout -k 9 $(TIMELIMIT) ./$@ <$(TINPUT)),\
				@timeout -k 9 $(TIMELIMIT) ./$@ <$(TINPUT),\
				@echo "(no output)"\
			)\
		),\
		$(if $(TOUTPUTEXIST),\
			@timeout -k 9 $(TIMELIMIT) ./$@|diff - $(TOUTPUT) --color=always -B -w -s,\
			$(if $(shell timeout -k 9 $(TIMELIMIT) ./$@),\
				@timeout -k 9 $(TIMELIMIT) ./$@,\
				@echo "(no output)"\
			)\
		)\
	)
	@echo ""
	@echo "================================"

test: $(EXEC)
	@echo "$(EXEC)"
	@echo "================================"
	@echo "All program have been tested."

clean:
	rm -f $(OBJECTS) $(EXEC)

# ignore below

pre-release: DOCUMENT = $(filter-out $(EXCLUDE),$(wildcard *.md))
pre-release: EXCLUDE = README% Note%

release: VERSION := $(shell date --utc +"%m-%d-%Y-%H-%M-%S")
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
	7z a ./release/$(VERSION).zip $(SOURCE) $(INCLUDE)
	7z u ./release/$(VERSION).zip ./pre-release/*
	@echo "================================"
	@echo "The release was success."
	@echo "Available in ./release/$(VERSION).zip"
