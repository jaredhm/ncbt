OFILES = main.o lexer.o driver.o parser.o ast.o codegen.o
LEX = flex
BISON = bison
CPATH = /usr/bin/
CC = $(CPATH)clang++
LD = $(CPATH)clang++
CXXFLAGS= -std=c++11 -Wno-deprecated-register

all: parse scan ncbt

scan: lexer.c

parse: parser.c parser.h

parser.c parser.h: parser.y
	$(BISON) --no-line --debug -d $< -o parser.c

lexer.c: lexer.lex parser.h
	$(LEX) --bison-bridge -o $@ $<

ncbt: $(OFILES)
	$(LD) -g $(OFILES) -o $@

%.o %.d : %.cpp
	$(CC) -g -c -x c++ $(CXXFLAGS) $< -MD

%.o %.d : %.c
	$(CC) -g -c -x c++ $(CXXFLAGS) $< -MD

clean:
	rm -rf *.o *~ parser.c parser.h lexer.c *.d ncbt

include $(addsuffix .d ,$(basename $(OFILES)))
