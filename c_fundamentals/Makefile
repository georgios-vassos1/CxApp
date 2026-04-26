CC     := clang
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wconversion

# ------------------------------------------------------------------ #
#  Module directories                                                 #
# ------------------------------------------------------------------ #

M01_DIR := M01_memory_pointers
M02_DIR := M02_strings_arrays
M03_DIR := M03_structs_typedef
M04_DIR := M04_function_pointers
M05_DIR := M05_bit_manipulation
M06_DIR := M06_c_idioms

# ------------------------------------------------------------------ #
#  Test binaries                                                      #
# ------------------------------------------------------------------ #

M01_TEST := $(M01_DIR)/memory_test
M02_TEST := $(M02_DIR)/strings_arrays_test
M03_TEST := $(M03_DIR)/structs_test
M04_TEST := $(M04_DIR)/funcptrs_test
M05_TEST := $(M05_DIR)/bits_test
M06_TEST := $(M06_DIR)/idioms_test

TESTS := $(M01_TEST) $(M02_TEST) $(M03_TEST) $(M04_TEST) $(M05_TEST) $(M06_TEST)

# ------------------------------------------------------------------ #
#  Targets                                                            #
# ------------------------------------------------------------------ #

.PHONY: all test clean asan ubsan

all: test

test: $(TESTS)
	@echo ""
	@for t in $(TESTS); do ./$$t; echo ""; done

# AddressSanitizer — detects heap/stack overflows, use-after-free, leaks.
asan: CFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
asan: clean $(TESTS)
	@echo ""
	@for t in $(TESTS); do ./$$t; echo ""; done

# UndefinedBehaviorSanitizer — detects signed overflow, null deref, bad shifts.
ubsan: CFLAGS += -fsanitize=undefined -g
ubsan: clean $(TESTS)
	@echo ""
	@for t in $(TESTS); do ./$$t; echo ""; done

# ------------------------------------------------------------------ #
#  Build rules per module                                             #
# ------------------------------------------------------------------ #

$(M01_TEST): $(M01_DIR)/memory_test.c $(M01_DIR)/memory.c
	$(CC) $(CFLAGS) -I. -o $@ $^

$(M02_TEST): $(M02_DIR)/strings_arrays_test.c $(M02_DIR)/strings_arrays.c
	$(CC) $(CFLAGS) -I. -o $@ $^

$(M03_TEST): $(M03_DIR)/structs_test.c $(M03_DIR)/structs.c
	$(CC) $(CFLAGS) -I. -o $@ $^ -lm

$(M04_TEST): $(M04_DIR)/funcptrs_test.c $(M04_DIR)/funcptrs.c
	$(CC) $(CFLAGS) -I. -o $@ $^ -lm

$(M05_TEST): $(M05_DIR)/bits_test.c $(M05_DIR)/bits.c
	$(CC) $(CFLAGS) -I. -o $@ $^

$(M06_TEST): $(M06_DIR)/idioms_test.c $(M06_DIR)/idioms.c
	$(CC) $(CFLAGS) -I. -o $@ $^

# ------------------------------------------------------------------ #
#  Clean                                                              #
# ------------------------------------------------------------------ #

clean:
	rm -f $(TESTS)
