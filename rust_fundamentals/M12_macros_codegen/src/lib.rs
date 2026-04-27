//! # Module 12 — Macros & Code Generation
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! This module exercises RUSTPATTERNS.md §7 — the `make_objective_values!` macro pattern.
//!
//! You will implement two `macro_rules!` macros:
//!
//! | Macro | Generates | Pattern |
//! |-------|-----------|---------|
//! | `make_unit!` | A newtype struct + `Display` + `Add` + `From<inner>` | Eliminate repetition for unit types |
//! | `make_stats!` | A stats accumulator struct with named `f64` fields + `accumulate` method | RUSTPATTERNS.md §7 — one entry per objective |
//!
//! The key insight from RUSTPATTERNS.md §7:
//! > "Adding a new objective requires only a new entry here — no struct field,
//! > no `with_next_leg` logic, no `Default` impl needed elsewhere."

// ------------------------------------------------------------------ //
//  1. make_unit! — generate unit-type newtypes                       //
// ------------------------------------------------------------------ //

/// Generate a newtype struct wrapping an `f64` (or any numeric type).
///
/// Usage:
/// ```ignore
/// make_unit!(Metres, f64);
/// make_unit!(Kilograms, f64);
/// ```
///
/// The macro must generate:
/// - `pub struct <Name>(pub <Inner>);`
/// - `impl <Name> { pub fn new(val: <Inner>) -> Self { <Name>(val) } }`
/// - `impl <Name> { pub fn value(self) -> <Inner> { self.0 } }`
/// - `impl std::fmt::Display for <Name> { ... }` — format as `"{value} <name_lowercase>"`
///   e.g. `Metres(3.5)` → `"3.50 metres"`
/// - `impl std::ops::Add for <Name>` — returns `<Name>(self.0 + other.0)`
/// - `impl From<<Inner>> for <Name>` — wraps the value
///
/// TODO: implement the macro body below.
#[macro_export]
macro_rules! make_unit {
    ($name:ident, $inner:ty) => {
        todo!() // Replace with the generated code described above
    };
}

// ------------------------------------------------------------------ //
//  2. make_stats! — generate a stats accumulator struct              //
// ------------------------------------------------------------------ //

/// Generate a stats accumulator struct with named `f64` fields.
///
/// Usage:
/// ```ignore
/// make_stats! {
///     total_cost: f64,
///     total_distance: f64,
///     num_delays: f64,
/// }
/// ```
///
/// The macro must generate:
/// - `#[derive(Debug, Default, Clone, PartialEq)] pub struct Stats { pub total_cost: f64, ... }`
/// - `impl Stats { pub fn add_step(&mut self, total_cost: f64, ...) { self.total_cost += total_cost; ... } }`
/// - `impl Stats { pub fn combine(&self, other: &Self) -> Self { Stats { total_cost: self.total_cost + other.total_cost, ... } } }`
///
/// The key property: **adding a new field requires only one line** in the macro invocation.
///
/// TODO: implement the macro body below.
#[macro_export]
macro_rules! make_stats {
    ($($field:ident : $ty:ty),* $(,)?) => {
        todo!() // Replace with the generated struct and impl blocks
    };
}

// ------------------------------------------------------------------ //
//  Instantiate the macros — declare the types used by the tests      //
// ------------------------------------------------------------------ //

// Declare a Metres newtype using make_unit!
// TODO: uncomment after implementing make_unit!
// make_unit!(Metres, f64);
// make_unit!(Kilograms, f64);

// Declare a Stats struct with three fields using make_stats!
// TODO: uncomment after implementing make_stats!
// make_stats! {
//     total_cost: f64,
//     total_distance: f64,
//     num_steps: f64,
// }

// ------------------------------------------------------------------ //
//  Placeholder types — used by the tests until the macros work       //
//                                                                     //
//  Once you implement the macros, delete these placeholders and       //
//  uncomment the make_unit! / make_stats! calls above.               //
// ------------------------------------------------------------------ //

/// Placeholder until make_unit!(Metres, f64) is implemented.
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Metres(pub f64);
impl Metres {
    pub fn new(val: f64) -> Self { todo!() }
    pub fn value(self) -> f64 { todo!() }
}
impl std::fmt::Display for Metres {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result { todo!() }
}
impl std::ops::Add for Metres {
    type Output = Metres;
    fn add(self, other: Metres) -> Metres { todo!() }
}
impl From<f64> for Metres {
    fn from(v: f64) -> Self { todo!() }
}

/// Placeholder until make_unit!(Kilograms, f64) is implemented.
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Kilograms(pub f64);
impl Kilograms {
    pub fn new(val: f64) -> Self { todo!() }
    pub fn value(self) -> f64 { todo!() }
}
impl std::fmt::Display for Kilograms {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result { todo!() }
}
impl std::ops::Add for Kilograms {
    type Output = Kilograms;
    fn add(self, other: Kilograms) -> Kilograms { todo!() }
}
impl From<f64> for Kilograms {
    fn from(v: f64) -> Self { todo!() }
}

/// Placeholder until make_stats! is implemented.
#[derive(Debug, Default, Clone, PartialEq)]
pub struct Stats {
    pub total_cost: f64,
    pub total_distance: f64,
    pub num_steps: f64,
}
impl Stats {
    pub fn add_step(&mut self, total_cost: f64, total_distance: f64, num_steps: f64) {
        todo!()
    }
    pub fn combine(&self, other: &Stats) -> Stats {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- Metres (generated by make_unit!) ---

    #[test]
    fn metres_new_and_value() {
        let m = Metres::new(2.5);
        assert!((m.value() - 2.5).abs() < 1e-9);
    }

    #[test]
    fn metres_add() {
        let a = Metres::new(1.0);
        let b = Metres::new(3.0);
        assert!((( a + b).value() - 4.0).abs() < 1e-9);
    }

    #[test]
    fn metres_display() {
        let m = Metres::new(3.5);
        assert_eq!(format!("{m}"), "3.50 metres");
    }

    #[test]
    fn metres_from_f64() {
        let m: Metres = Metres::from(7.0);
        assert!((m.value() - 7.0).abs() < 1e-9);
    }

    // --- Kilograms (generated by the same make_unit! macro) ---

    #[test]
    fn kilograms_new_and_value() {
        let k = Kilograms::new(50.0);
        assert!((k.value() - 50.0).abs() < 1e-9);
    }

    #[test]
    fn kilograms_display() {
        let k = Kilograms::new(1.5);
        assert_eq!(format!("{k}"), "1.50 kilograms");
    }

    // --- Stats (generated by make_stats!) ---

    #[test]
    fn stats_default_is_zero() {
        let s = Stats::default();
        assert_eq!(s.total_cost, 0.0);
        assert_eq!(s.total_distance, 0.0);
        assert_eq!(s.num_steps, 0.0);
    }

    #[test]
    fn stats_add_step_accumulates() {
        let mut s = Stats::default();
        s.add_step(10.0, 5.0, 1.0);
        s.add_step(20.0, 8.0, 1.0);
        assert!((s.total_cost - 30.0).abs() < 1e-9);
        assert!((s.total_distance - 13.0).abs() < 1e-9);
        assert!((s.num_steps - 2.0).abs() < 1e-9);
    }

    #[test]
    fn stats_combine() {
        let a = Stats { total_cost: 10.0, total_distance: 3.0, num_steps: 2.0 };
        let b = Stats { total_cost: 5.0, total_distance: 7.0, num_steps: 1.0 };
        let c = a.combine(&b);
        assert!((c.total_cost - 15.0).abs() < 1e-9);
        assert!((c.total_distance - 10.0).abs() < 1e-9);
        assert!((c.num_steps - 3.0).abs() < 1e-9);
    }

    #[test]
    fn adding_a_new_field_requires_only_one_line() {
        // This test is intentionally structural — it documents the goal.
        // Once make_stats! works, adding `penalty: f64` to the invocation
        // should automatically extend add_step, combine, and Default.
        // No other code changes needed.
        let s = Stats::default();
        let _ = s.total_cost; // field exists
    }
}
