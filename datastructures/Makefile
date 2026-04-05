CC       := clang
CFLAGS   := -std=c11 -Wall -Wextra -Wpedantic -Wconversion -fPIC
LDFLAGS  :=

UNAME_S  := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    SHARED_EXT   := dylib
    SHARED_FLAGS := -dynamiclib
    RPATH_FLAG   := -Wl,-rpath,@loader_path
else
    SHARED_EXT   := so
    SHARED_FLAGS := -shared
    RPATH_FLAG   := -Wl,-rpath,'$$ORIGIN'
endif

LIB_NAME := libcxds.$(SHARED_EXT)

LIB_SRCS := stack.c bsTree.c hash_table.c linked_list.c ds_helpers.c
LIB_OBJS := $(LIB_SRCS:.c=.o)

DEMOS    := bsTree_demo hash_table_demo linked_list_demo \
            stack_demo_int stack_demo_node

.PHONY: all demos clean

all: $(LIB_NAME)

$(LIB_NAME): $(LIB_OBJS)
	$(CC) $(SHARED_FLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

demos: $(LIB_NAME) $(DEMOS)

$(DEMOS): %: %.o $(LIB_NAME)
	$(CC) -o $@ $< -L. -lcxds $(RPATH_FLAG) $(LDFLAGS)

clean:
	rm -f *.o $(LIB_NAME) $(DEMOS)
