CC=gcc
CFLAGS= -I -g -lstdc++ -std=c++11

compfs: main.cpp scanner.cpp scanner.hpp character.hpp token.hpp parser.cpp parser.hpp node.hpp tree.hpp tree.cpp semantic.hpp semantic.cpp codegenerator.hpp codegenerator.cpp
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o compfs
	rm -f *.gch
	rm file.txt
