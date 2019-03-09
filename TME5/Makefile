all: pagerank.exe exo2.exe pageranknonmem.exe perspagerank.exe

%.exe: %.cpp structs.cpp structs.h
	g++ -O5 -o $@ $^

clean:
	rm -f *.exe

.PHONY: all clean
