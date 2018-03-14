all:
	g++ -o bin/main src/main.cpp --std=c++11

clean:
	rm -f bin/main
