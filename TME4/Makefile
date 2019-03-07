all: exo1.exe exo2.exe exo4.exe

exo1.exe: exo1.cpp structs.h structs.cpp
	g++ -o $@ structs.cpp $<

exo2.exe: exo2.cpp structs.h structs.cpp
	g++ -O5 -o $@ structs.cpp $<

exo4.exe: exo4.cpp structs.h structs.cpp
	g++ -O5 -o $@ structs.cpp $<

clean:
	rm -rf *.o *.exe
