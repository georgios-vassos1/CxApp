CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -g
LDLIBS  = -lm

# Hash-table tunables — override on the command line if needed:
#   make INIT_HSIZE=4 HIGH_LOAD=2.0
INIT_HSIZE ?= 1
HIGH_LOAD  ?= 4.0
LOW_LOAD   ?= 1.0

CPPFLAGS += -DINIT_HSIZE=$(INIT_HSIZE) \
            -DHIGH_LOAD=$(HIGH_LOAD)   \
            -DLOW_LOAD=$(LOW_LOAD)

SRCS = dlist.c htable.c slist.c utils.c hw2.c
OBJS = $(SRCS:.c=.o)
BIN  = hw2

# Shared library (for Python ctypes / R .Call wrapping)
LIB_SRCS = dlist.c htable.c slist.c utils.c
LIB_OBJS = $(LIB_SRCS:.c=.pic.o)
SHLIB    = libhw2ds.so

.PHONY: all lib clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

lib: $(SHLIB)

$(SHLIB): $(LIB_OBJS)
	$(CC) -shared -o $@ $^ $(LDLIBS)

%.pic.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(LIB_OBJS) $(BIN) $(SHLIB)

# Header dependencies (conservative)
dlist.o  dlist.pic.o:  dlist.c  dlist.h
htable.o htable.pic.o: htable.c htable.h dlist.h
slist.o  slist.pic.o:  slist.c  slist.h  utils.h
utils.o  utils.pic.o:  utils.c  utils.h
hw2.o:                 hw2.c    hw2.h    htable.h dlist.h slist.h utils.h
