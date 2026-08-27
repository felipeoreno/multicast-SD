CXX=g++
CXXFLAGS=-I.
DEPS=client.h

client: client.o
	$(CXX) -o $@ $^

main: main.o
	$(CXX) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)