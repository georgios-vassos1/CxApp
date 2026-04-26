# MIT 6.851 Advanced Data Structures — Hands-On Course Plan

Source: https://www.youtube.com/playlist?list=PLUl4u3cNGP61hsJNdULdudlRL493b-XZf
Instructor: Erik Demaine, MIT, Spring 2012
Language: C11 (clang)
Methodology: TDD red → green → refactor, then demo, then benchmark

## Learning Flow (per lecture)

1. Theory explanation (key ideas, invariants, complexity)
2. Implement core structure (.h API + .c)
3. Demo — makes semantics visible
4. Benchmark — confirms theoretical complexity experimentally
5. Comparison — against naive baseline

## Reordered Curriculum (dependency-respecting, DS/CS-friendly)

### Phase 1 — Familiar ground, immediate payoff

| # | Original | Directory | Status |
|---|---|---|---|
| 1 | L14 | `L14_sorting/` | RED (counting_sort partial, radix_sort stub) |
| 2 | L10 | `L10_dictionaries/` | not started |
| 3 | L15 | `L15_static_trees/` | not started |

### Phase 2 — Memory & cache (most impactful for data science)

| # | Original | Directory | Status |
|---|---|---|---|
| 4 | L07 | `L07_memory_hierarchy/` | not started |
| 5 | L08 | `L08_cache_oblivious_1/` | not started |
| 6 | L09 | `L09_cache_oblivious_2/` | not started |

### Phase 3 — Geometric structures

| # | Original | Directory | Status |
|---|---|---|---|
| 7 | L03 | `L03_geometric_1/` | not started |
| 8 | L04 | `L04_geometric_2/` | not started |

### Phase 4 — Integer structures

| # | Original | Directory | Status |
|---|---|---|---|
| 9  | L11 | `L11_veb_tree/` | not started |
| 10 | L12 | `L12_fusion_tree/` | not started |
| 11 | L13 | `L13_integer_lower_bounds/` | not started |

### Phase 5 — Dynamic BST optimality

| # | Original | Directory | Status |
|---|---|---|---|
| 12 | L05 | `L05_splay_tree/` | not started |
| 13 | L06 | `L06_tango_tree/` | not started |

### Phase 6 — Temporal structures (most exotic)

| # | Original | Directory | Status |
|---|---|---|---|
| 14 | L01 | `L01_persistent/` | not started |
| 15 | L02 | `L02_retroactive/` | not started |

## L14 — Sorting in Linear Time (current)

### Files
- `L14_sorting/sorting.h` — API (done)
- `L14_sorting/sorting.c` — counting_sort (partial GREEN), radix_sort (stub)
- `L14_sorting/sorting_test.c` — 22 tests, RED phase confirmed
- `L14_sorting/sorting_demo.c` — TODO (step 3)
- `L14_sorting/sorting_bench.c` — TODO (step 4)

### Remaining work
- [ ] Implement `radix_sort` (GREEN phase)
- [ ] All 22 tests pass
- [ ] Write `sorting_demo.c` — show step-by-step counting sort on small input, then radix
- [ ] Write `sorting_bench.c` — measure vs qsort at N = 10k, 100k, 1M
- [ ] Add `sorting_demo` and `sorting_bench` targets to Makefile

## Shared infrastructure

- `test_framework.h` — minimal C11 test harness (done, shared across lectures)
- `Makefile` — extend as each lecture is added
