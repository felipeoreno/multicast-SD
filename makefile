
CC=g++
CFLAGS=-I.
DEPS = client.h
OBJ = client.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

main : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)