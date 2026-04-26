CC     := clang
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wconversion

# ------------------------------------------------------------------ #
#  L14 — Sorting in Linear Time                                      #
# ------------------------------------------------------------------ #

L14_DIR  := L14_sorting
L14_SRCS := $(L14_DIR)/sorting.c
L14_TEST := $(L14_DIR)/sorting_test

TESTS := $(L14_TEST)

.PHONY: all test clean

all: test

test: $(TESTS)
	@echo ""
	@for t in $(TESTS); do ./$$t; done

$(L14_TEST): $(L14_DIR)/sorting_test.c $(L14_SRCS)
	$(CC) $(CFLAGS) -I. -o $@ $^

clean:
	rm -f $(TESTS)
