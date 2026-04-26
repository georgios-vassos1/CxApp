CC     := clang
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wconversion

# ------------------------------------------------------------------ #
#  M01 — Memory & Pointers                                           #
# ------------------------------------------------------------------ #

M01_DIR  := M01_memory_pointers
M01_SRCS := $(M01_DIR)/memory.c
M01_TEST := $(M01_DIR)/memory_test

TESTS := $(M01_TEST)

.PHONY: all test clean

all: test

test: $(TESTS)
	@echo ""
	@for t in $(TESTS); do ./$$t; done

$(M01_TEST): $(M01_DIR)/memory_test.c $(M01_SRCS)
	$(CC) $(CFLAGS) -I. -o $@ $^

clean:
	rm -f $(TESTS)
