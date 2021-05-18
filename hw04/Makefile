CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Werror -Wextra -Wno-unused
LDFLAGS = -lm

OBJECTS = $(patsubst %c, %o, $(wildcard *.c))
TARGETS = hw0401 hw0402 hw0403 hw0404

hw0401: hw0401.o share.o
hw0402: hw0402.o share.o
hw0403: hw0403.o myprintf.o
hw0404: hw0404.o share.o bst.o

main: main.o
test_out: test_out.o

TPATH = ./test
TIMELIMIT = 1.00s
TIGNORE = hw0404

TINPUT = $(patsubst %,$(TPATH)/%.input,$(EXEC))
TOUTPUT = $(patsubst %,$(TPATH)/%.output,$(EXEC))
TEMPFILE = test_out.* main.c.* main

test-hw0401:
	@$(if $(shell test -e $(EXEC) && echo "1"),\
		echo "=== Start test program: $(EXEC)";\
		echo "./$(EXEC) --help";\
		echo "================================";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) --help && \
		echo "================================";\
		echo "./$(EXEC) -s main.c --size 500";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) -s main.c --size 500 && \
		echo "================================";\
		ls -l | grep -- main && \
		echo "================================";\
		echo "./$(EXEC) -r test_out.c main.c.1 main.c.3 main.c.2";\
		echo "================================";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) -r test_out.c main.c.1 main.c.3 main.c.2 && \
		diff main.c test_out.c --color=always -B -w -s;\
		echo "================================",\
		echo "=== Test target not exist."\
	)

test-hw0402:
	@$(if $(shell test -e $(EXEC) && echo "1"),\
		echo "=== Start test program: $(EXEC)";\
		echo "./$(EXEC) --help";\
		echo "================================";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) --help && \
		echo "================================";\
		make main test_out ;\
		timeout -k 9 $(TIMELIMIT) ./main > ./test_out.txt; \
		echo "================================";\
		echo "./$(EXEC) -i main.c -o test_out.c -l 1";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) -i main.c -o test_out.c -l 1 && \
		timeout -k 9 $(TIMELIMIT) ./test_out|diff - ./test_out.txt --color=always -B -w -s; \
		echo "================================";\
		echo "./$(EXEC) -i main.c -o test_out.c -l 2";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) -i main.c -o test_out.c -l 1 && \
		timeout -k 9 $(TIMELIMIT) ./test_out|diff - ./test_out.txt --color=always -B -w -s; \
		echo "================================";\
		echo "./$(EXEC) -i main.c -o test_out.c -l 3";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) -i main.c -o test_out.c -l 1 && \
		timeout -k 9 $(TIMELIMIT) ./test_out|diff - ./test_out.txt --color=always -B -w -s; \
		echo "================================",\
		echo "./$(EXEC) -i main.c -o test_out.c -l 4";\
		timeout -k 9 $(TIMELIMIT) ./$(EXEC) -i main.c -o test_out.c -l 1 && \
		timeout -k 9 $(TIMELIMIT) ./test_out|diff - ./test_out.txt --color=always -B -w -s; \
		echo "================================",\
		echo "=== Test target not exist."\
	)

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
	@$(if $(filter $(TARGETS),$(EXEC)),\
		$(if $(shell test -e $(EXEC) && echo "1"),\
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
			);\
			echo "================================",\
			echo "=== Test target not exist."\
		),\
		echo "=== Unknown target."\
	)

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
release: SOURCE := $(wildcard *.[^(exe|out)])

release: pre-release-exe pre-release-doc clean
	7z a ./release/$(VERSION).zip ./pre-release/*
	7z u ./release/$(VERSION).zip $(SOURCE)
	rm -r -f ./pre-release/
	@echo "================================"
	@echo "The release was success."
	@echo "Available in ./release/$(VERSION).zip"
