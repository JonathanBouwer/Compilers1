all:
	g++ -o bin/main src/parser.cpp --std=c++11

clean:
	rm -f bin/main
