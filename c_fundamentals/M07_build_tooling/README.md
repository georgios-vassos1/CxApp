# Module 7 — Build & Tooling

This module is different from the others: there is no `.h`/`.c` to implement.
Instead, there are four hands-on exercises that build your understanding of the
C build pipeline and the sanitizer tools that catch bugs you cannot see.

---

## Exercise 1 — Read the Makefile

Open `c_fundamentals/Makefile` and answer:

1. What does the `asan` target do differently from `test`?
2. Why does `asan` depend on `clean` before rebuilding?
3. What flag controls dependency tracking between modules?
4. How would you add Module 8 to the Makefile?

---

## Exercise 2 — Header guards

Every `.h` file in this repo has a header guard:

```c
#ifndef C_FUND_BITS_H
#define C_FUND_BITS_H
/* ... */
#endif
```

**Task:** Create a file `double_include_test.c` that includes `bits.h` twice:

```c
#include "bits.h"
#include "bits.h"   /* second include — must not cause compile errors */

int main(void) { return 0; }
```

Compile it manually:

```bash
clang -std=c11 -I../M05_bit_manipulation double_include_test.c -o double_include_test
```

Now remove the header guard from a copy of `bits.h` and observe what happens.

---

## Exercise 3 — AddressSanitizer (ASan)

Run all tests under ASan:

```bash
make asan   # from c_fundamentals/
```

ASan detects:
- Heap buffer overflows
- Stack buffer overflows
- Use-after-free
- Memory leaks (with `ASAN_OPTIONS=detect_leaks=1`)

**Task:** Introduce a deliberate bug into one of your implementations
(e.g., write one element past the end of a malloc'd buffer in `int_arr_dup`),
run `make asan`, read the error report, then fix the bug.

The ASan output tells you:
- The type of error (`heap-buffer-overflow`)
- The exact line where the write happened
- The allocation site
- A stack trace

---

## Exercise 4 — UndefinedBehaviorSanitizer (UBSan)

Run all tests under UBSan:

```bash
make ubsan   # from c_fundamentals/
```

UBSan detects:
- Signed integer overflow
- Null pointer dereference
- Shift past bit width
- Array index out of bounds (when detectable statically)

**Task:** In `safe_add`, temporarily replace the overflow check with a plain
`a + b` (letting it overflow), then run `make ubsan` and observe the report.

---

## Exercise 5 — Separate compilation

The Makefile currently compiles each test as a single command:

```makefile
$(CC) $(CFLAGS) -I. -o $@ test.c impl.c
```

**Task:** Rewrite one module's build rule to use separate compilation:

```makefile
impl.o: impl.c impl.h
    $(CC) $(CFLAGS) -c -o $@ $<

test.o: test.c impl.h
    $(CC) $(CFLAGS) -I. -c -o $@ $<

test_binary: test.o impl.o
    $(CC) -o $@ $^
```

Observe that changing only `impl.c` now only recompiles `impl.o`, not
`test.o`. This is the basis of incremental builds in large projects.
