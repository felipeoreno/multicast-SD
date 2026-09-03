CXX=g++
CXXFLAGS=-I. -Wall -Wall -O0 -ggdb3
DEPS=client.h

all: main client

client: client.o
	$(CXX) -o $@ $^

main: main.o
	$(CXX) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -f *.o client main