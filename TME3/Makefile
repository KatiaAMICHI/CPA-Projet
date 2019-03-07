all: inner-sort.exe exo1.exe exo2.exe exo3.exe exo4.exe

inner-sort.exe: inner-sort.c
	g++ -o $@ $<

exo1.exe: exo1.cpp
	g++ -o $@ $<

exo2.exe: exo2.cpp structs.h structs.cpp
	g++ -o $@ structs.cpp $<

exo3.exe: exo3.cpp structs.h structs.cpp algs.h algs.cpp
	g++ -o $@ structs.cpp algs.cpp $<

exo4.exe: exo4.cpp structs.h structs.cpp algs.h algs.cpp
	g++ -o $@ structs.cpp algs.cpp $<

clean:
	rm -rf *.o *.exe

tar:
	tar -cvf tme3Pesin.tar *.c *.cpp *.h Makefile plot.py rapport/rapport.pdf
