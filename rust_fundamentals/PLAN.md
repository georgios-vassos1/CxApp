# Rust Fundamentals — Course Plan

A TDD-driven Rust curriculum that builds from core language mechanics
to the production patterns documented in RUSTPATTERNS.md.

Each module is a standalone Cargo crate.  Run a single module with:

    cargo test -p m01_ownership_borrowing

Run the entire course:

    cargo test --workspace

---

## Prerequisites — Core Language

| Module | Topic | Key Concepts |
|--------|-------|--------------|
| M01 | Ownership & Borrowing | move, borrow, &T, &mut T, slices, lifetimes |
| M02 | Structs, Methods & Enums | impl blocks, match, Option<T>, method chaining |
| M03 | Collections & Iterators | Vec, HashMap, entry API, map/filter/fold, zip |
| M04 | Error Handling Basics | Result<T,E>, custom errors, ? operator |
| M05 | Traits & Generics | trait definitions, impl Trait, generic bounds, derive |
| M06 | Closures & Functional Patterns | Fn/FnMut/FnOnce, combinators, partition_point |

---

## Advanced Patterns — from RUSTPATTERNS.md

| Module | Topic | RUSTPATTERNS.md sections |
|--------|-------|--------------------------|
| M07 | Type-Driven Design | §3 — Newtypes, data-carrying enums, Copy, #[must_use], generic wrappers |
| M08 | Module Organisation & Visibility | §1 — Composition root, re-export gateways, pub(crate), feature flags |
| M09 | Advanced Error-Handling Patterns | §2 — if-let-err, let-else, guard-then-unwrap, Option as three-state |
| M10 | Serde & DTO Patterns | §4 — Serialize/Deserialize, alias, default, strict request/response split |
| M11 | Trait Polymorphism & Collection Patterns | §3/§6 — static dispatch, entry API, zip+skip(1), partition_point |
| M12 | Macros & Code Generation | §7 — macro_rules!, generating structs and method impls |
| M13 | Test Patterns & Safety Philosophy | §8/§10/§11/§12 — factories, rstest, epsilon PartialEq, debug_assert!, unreachable! |

---

## TDD Workflow

1. `cargo test -p <module>` — all non-trivial tests fail (red).
2. Implement each stub function in `src/lib.rs`.
3. Re-run until all tests pass (green).
4. Move to the next module.

M01 is the **reference implementation** — all tests already pass.
M02–M13 are in the **red phase** — stubs return `todo!()`.
