CC=gcc
CFLAGS=-Wall -g

all: out

out: main.o arvorern.o
	$(CC) $(CFLAGS) main.o arvorern.o -o out

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm a.out *o out
