CC=gcc
CFLAGS=-Wall

all: add sum redirect piper

add: add.c
	$(CC) $(CFLAGS) -o add add.c

sum: sum.c
	$(CC) $(CFLAGS) -o sum sum.c

redirect: fork_redir_exec.c
	$(CC) $(CFLAGS) -o redirect fork_redir_exec.c

piper: piper.c
	$(CC) $(CFLAGS) -o piper piper.c

clean:
	rm -f add sum redirect piper
