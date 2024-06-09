CC=gcc
CFLAGS=-Wall

all: myecho test

myecho: myecho.c
	$(CC) $(CFLAGS) -o myecho myecho.c

test: exmJun2013B3.c
	$(CC) $(CFLAGS) -o test exmJun2013B3.c

clean:
	rm -f myecho test
