CC       := clang
CFLAGS   := -std=c11 -Wall -Wextra -Wpedantic -Wconversion -fPIC -MMD -MP
LDFLAGS  :=

UNAME_S  := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    SHARED_EXT   := dylib
    SHARED_FLAGS := -dynamiclib -install_name @rpath/libcxds.$(SHARED_EXT)
    RPATH_FLAG   := -Wl,-rpath,@loader_path
else
    SHARED_EXT   := so
    SHARED_FLAGS := -shared -Wl,-soname,libcxds.$(SHARED_EXT).1
    RPATH_FLAG   := -Wl,-rpath,'$$ORIGIN'
endif

LIB_NAME := libcxds.$(SHARED_EXT)

LIB_SRCS := stack.c bsTree.c hash_table.c linked_list.c fib_heap.c ds_helpers.c
LIB_OBJS := $(LIB_SRCS:.c=.o)

DEMOS    := bsTree_demo hash_table_demo linked_list_demo \
            stack_demo_int stack_demo_node fib_heap_demo fib_heap_bench
DEMO_OBJS := $(addsuffix .o,$(DEMOS))

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
	rm -f *.o *.d $(LIB_NAME) $(DEMOS)

-include $(LIB_OBJS:.o=.d) $(DEMO_OBJS:.o=.d)
