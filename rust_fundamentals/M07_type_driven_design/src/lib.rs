//! # Module 7 — Type-Driven Design
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! This module translates RUSTPATTERNS.md §3 into exercises:
//!
//! | Pattern | Types | RUSTPATTERNS.md reference |
//! |---------|-------|--------------------------|
//! | Newtype | `Metres`, `Seconds` | "Convert at the boundary, use integers in the core" |
//! | Data-carrying enum + Copy | `Arrival` | "`Node { idx, arrival }` is the DP key" |
//! | Generic scope wrapper | `ScopeWrapper<T>` | "`ScopeObjective<T>` reuse this wrapper" |
//! | `#[must_use]` | `compute_fare` | "prevents callers from silently discarding" |
//! | Private fields, public API | `BoundedValue` | "All solver-core struct fields are private" |

// ------------------------------------------------------------------ //
//  1. Newtypes — "Convert at the boundary"                           //
// ------------------------------------------------------------------ //

/// Metres — a newtype over `f64` to prevent mixing up units at compile time.
///
/// Implement:
/// - `Metres::new(val: f64) -> Self`
/// - `Metres::value(&self) -> f64`
/// - `impl std::ops::Add for Metres` — adding two `Metres` gives `Metres`
/// - `impl std::fmt::Display for Metres` — format as `"3.50 m"`
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Metres(f64);

impl Metres {
    /// TODO: wrap `val` in the newtype.
    pub fn new(val: f64) -> Self {
        todo!()
    }

    /// TODO: return the inner value.
    pub fn value(self) -> f64 {
        todo!()
    }
}

impl std::ops::Add for Metres {
    type Output = Metres;
    /// TODO: return `Metres(self.0 + other.0)`.
    fn add(self, other: Metres) -> Metres {
        todo!()
    }
}

impl std::fmt::Display for Metres {
    /// TODO: write `"{:.2} m"` to `f`.
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

/// Seconds — integer newtype for time deltas (mirrors `i32` timestamps in RUSTPATTERNS.md).
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct Seconds(pub i32);

impl Seconds {
    pub fn new(val: i32) -> Self { Seconds(val) }
    pub fn value(self) -> i32 { self.0 }
}

// ------------------------------------------------------------------ //
//  2. Data-carrying enum + Copy                                       //
// ------------------------------------------------------------------ //

/// Arrival variant for a port call — directly from RUSTPATTERNS.md §3.
///
/// - `Scheduled`    — the vessel arrives exactly as scheduled.
/// - `Proforma`     — arrival is estimated from the schedule.
/// - `TimeUnit(i32)` — arrival is offset by `i32` seconds from the baseline.
///
/// Derives `Copy` so it can be used as a `HashMap` key without cloning.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Arrival {
    Scheduled,
    Proforma,
    TimeUnit(i32),
}

impl Arrival {
    /// Return `true` if this is a `TimeUnit` variant.
    ///
    /// TODO: match on self.
    pub fn is_time_unit(&self) -> bool {
        todo!()
    }

    /// Return the offset in seconds, or `None` for non-`TimeUnit` variants.
    ///
    /// TODO: match on `TimeUnit(n)` and return `Some(n)`, else `None`.
    pub fn offset_seconds(&self) -> Option<i32> {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  3. Generic scope wrapper                                           //
// ------------------------------------------------------------------ //

/// A wrapper that holds two values of the same type — one for in-scope,
/// one for out-of-scope port calls.  Mirrors `ScopeObjective<T>` in RUSTPATTERNS.md §3.
///
/// Adding a new scoped objective requires only a field in a declarative macro invocation —
/// no struct field, no `Default` impl, nothing else.
#[derive(Debug, Clone, PartialEq)]
pub struct ScopeWrapper<T> {
    pub in_scope: T,
    pub out_of_scope: T,
}

impl<T: std::ops::Add<Output = T> + Copy> ScopeWrapper<T> {
    pub fn new(in_scope: T, out_of_scope: T) -> Self {
        ScopeWrapper { in_scope, out_of_scope }
    }

    /// Return a new `ScopeWrapper` where both fields are the sum of the
    /// corresponding fields in `self` and `other`.
    ///
    /// TODO: `ScopeWrapper { in_scope: self.in_scope + other.in_scope, ... }`.
    pub fn combine(&self, other: &Self) -> Self {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  4. #[must_use]                                                     //
// ------------------------------------------------------------------ //

/// Compute the fare for a journey of `distance` metres at `rate` per metre.
///
/// `#[must_use]` means the compiler will warn if the caller drops the return value.
/// This mirrors `#[must_use] fn solve(...)` in RUSTPATTERNS.md §3.
///
/// TODO: return `distance.value() * rate`.
#[must_use]
pub fn compute_fare(distance: Metres, rate: f64) -> f64 {
    todo!()
}

// ------------------------------------------------------------------ //
//  5. Private fields, public constructor                             //
// ------------------------------------------------------------------ //

/// A value guaranteed to be within `[min, max]` at construction time.
/// Fields are private — the only way to create a `BoundedValue` is through `new`.
/// This mirrors "All solver-core struct fields are private" in RUSTPATTERNS.md §3.
#[derive(Debug, Clone, Copy)]
pub struct BoundedValue {
    value: i32,
    min: i32,
    max: i32,
}

impl BoundedValue {
    /// Return `Some(BoundedValue)` if `min <= value <= max`, else `None`.
    ///
    /// TODO: validate the invariant and construct the struct.
    pub fn new(value: i32, min: i32, max: i32) -> Option<Self> {
        todo!()
    }

    /// Return the stored value.
    pub fn get(&self) -> i32 {
        self.value
    }

    /// Return a new `BoundedValue` with the value incremented by `delta`,
    /// clamped to `[self.min, self.max]`.
    ///
    /// TODO: clamp `self.value + delta` then call `BoundedValue::new`.
    pub fn saturating_add(&self, delta: i32) -> Self {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- Metres ---

    #[test]
    fn metres_new_and_value() {
        let m = Metres::new(3.5);
        assert!((m.value() - 3.5).abs() < 1e-9);
    }

    #[test]
    fn metres_addition() {
        let a = Metres::new(1.5);
        let b = Metres::new(2.5);
        assert!((( a + b).value() - 4.0).abs() < 1e-9);
    }

    #[test]
    fn metres_display() {
        let m = Metres::new(3.5);
        assert_eq!(format!("{m}"), "3.50 m");
    }

    // --- Arrival ---

    #[test]
    fn arrival_is_time_unit_true() {
        assert!(Arrival::TimeUnit(300).is_time_unit());
    }

    #[test]
    fn arrival_is_time_unit_false_for_others() {
        assert!(!Arrival::Scheduled.is_time_unit());
        assert!(!Arrival::Proforma.is_time_unit());
    }

    #[test]
    fn arrival_offset_seconds_time_unit() {
        assert_eq!(Arrival::TimeUnit(-120).offset_seconds(), Some(-120));
    }

    #[test]
    fn arrival_offset_seconds_none_for_others() {
        assert_eq!(Arrival::Scheduled.offset_seconds(), None);
        assert_eq!(Arrival::Proforma.offset_seconds(), None);
    }

    #[test]
    fn arrival_copy_semantics() {
        let a = Arrival::TimeUnit(60);
        let b = a; // copy, not move
        assert_eq!(a, b);
    }

    // --- ScopeWrapper ---

    #[test]
    fn scope_wrapper_combine_integers() {
        let a: ScopeWrapper<i32> = ScopeWrapper::new(3, 7);
        let b: ScopeWrapper<i32> = ScopeWrapper::new(1, 2);
        let c = a.combine(&b);
        assert_eq!(c.in_scope, 4);
        assert_eq!(c.out_of_scope, 9);
    }

    // --- compute_fare ---

    #[test]
    fn compute_fare_basic() {
        let dist = Metres::new(10.0);
        let fare = compute_fare(dist, 2.5);
        assert!((fare - 25.0).abs() < 1e-9);
    }

    // --- BoundedValue ---

    #[test]
    fn bounded_value_valid_construction() {
        let bv = BoundedValue::new(5, 0, 10).unwrap();
        assert_eq!(bv.get(), 5);
    }

    #[test]
    fn bounded_value_at_boundaries() {
        assert!(BoundedValue::new(0, 0, 10).is_some());
        assert!(BoundedValue::new(10, 0, 10).is_some());
    }

    #[test]
    fn bounded_value_out_of_range_is_none() {
        assert!(BoundedValue::new(-1, 0, 10).is_none());
        assert!(BoundedValue::new(11, 0, 10).is_none());
    }

    #[test]
    fn bounded_value_saturating_add_clamps() {
        let bv = BoundedValue::new(8, 0, 10).unwrap();
        let bv2 = bv.saturating_add(5); // 8+5=13 → clamped to 10
        assert_eq!(bv2.get(), 10);
    }

    #[test]
    fn bounded_value_saturating_add_no_clamp() {
        let bv = BoundedValue::new(3, 0, 10).unwrap();
        let bv2 = bv.saturating_add(4); // 3+4=7 → within range
        assert_eq!(bv2.get(), 7);
    }
}
