CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Werror -Wextra -Wno-unused
LDFLAGS = -lm

OBJECTS = $(patsubst %c, %o, $(wildcard *.c))
TARGETS = hw0301 hw0302 hw0303 hw0304 hw0305 hw0306

hw0301: hw0301.o share.o
hw0302: hw0302.o share.o
hw0303: hw0303.o share.o bmp.o
hw0304: hw0304.o share.o bmp.o
hw0305: hw0305.o share.o
hw0306: hw0306.o

TPATH = ./test
TIMELIMIT = 10.00s
TIGNORE = hw0301

TINPUT = $(patsubst %,$(TPATH)/%.input,$(EXEC))
TOUTPUT = $(patsubst %,$(TPATH)/%.output,$(EXEC))

TEMPFILE = hw0303_out.bmp hw0304_out.bmp

test-hw0301: TIMELIMIT = 10.0m
test-hw0303: TIMELIMIT = 1.0m
test-hw0304: TIMELIMIT = 1.0m

################################################################################

.DEFAULT_GOAL := all
.PHONY: install test clean
FORCE:

usage:
	@echo "Usage: make [Options]"
	@echo "Options:"
	@echo "  usage    Display this information."
	@echo "  install  Install dependency package."
	@echo "  all      Build all targets."
	@echo "  [target] Build specified target that listing below."
	@echo "  	          target: $(TARGETS)"
	@echo "  test     Testing all executable with example I/O in test folder."
	@echo "  clean    remove all build result."
	@echo ""
	@echo "DEFAULT OPTIONS : all"

install:
	@echo "================================"
	@echo "Nothing needs to be installed."

$(OBJECTS): %o:%c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: $(%)
	$(CC) -o $@ $^ $(LDFLAGS)

all: $(TARGETS)
	@echo "================================"
	@echo "All program have been compiled."

test-%: EXEC = $(patsubst test-%,%,$@)
test-%: TINPUTEXIST = $(shell test -e $(TINPUT) && echo "1")
test-%: TOUTPUTEXIST = $(shell test -e $(TOUTPUT) && echo "1")

test-%:
	@$(if $(shell test -e $(EXEC) && echo "1"),\
		echo "=== Start test program: $(EXEC)";\
		$(if $(TINPUTEXIST),\
			echo "==== Test with example input";\
			head -n 10 $(TINPUT);\
			$(if $(TOUTPUTEXIST),\
				timeout -k 9 $(TIMELIMIT) ./$(EXEC) <$(TINPUT)|diff - $(TOUTPUT) --color=always -B -w -s,\
				echo "==== No example output";\
				timeout -k 9 $(TIMELIMIT) ./$(EXEC) <$(TINPUT) && echo ""\
			),\
			echo "==== No example input";\
			$(if $(TOUTPUTEXIST),\
				timeout -k 9 $(TIMELIMIT) ./$(EXEC)|diff - $(TOUTPUT) --color=always -B -w -s,\
				echo "==== No example output";\
				timeout -k 9 $(TIMELIMIT) ./$(EXEC) && echo ""\
			)\
		),\
		echo "=== Test target not exist."\
	)
	@echo "================================"

test: $(patsubst %,test-%,$(filter-out $(TIGNORE),$(TARGETS)))
	@echo "================================"
	@echo "All program have been tested."

clean:
	rm -f $(OBJECTS) $(TARGETS) $(TEMPFILE)

# ignore below

pre-release-exe: CFLAGS = -std=gnu11 -O2 -Wall -Werror -Wextra
pre-release-exe: all test clean
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
release: SOURCE = $(wildcard *.[^(exe|out)])

release: pre-release-exe pre-release-doc clean
	7z a ./release/$(VERSION).zip ./pre-release/*
	7z u ./release/$(VERSION).zip $(SOURCE) $(TPATH)
	rm -r -f ./pre-release/
	@echo "================================"
	@echo "The release was success."
	@echo "Available in ./release/$(VERSION).zip"
