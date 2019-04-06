all: exo1.exe exo3.exe exo5.exe

%.exe: %.cpp structs.cpp structs.h heap.h heap.cpp
	g++ -O5 -o $@ $^

clean:
	rm -f *.exe

.PHONY: all clean
