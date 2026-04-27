//! # Module 11 — Trait Polymorphism & Collection Patterns
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! This module exercises RUSTPATTERNS.md §3 (Bunker trait), §6 (DP patterns):
//!
//! | Pattern | Types / Functions | RUSTPATTERNS.md reference |
//! |---------|-------------------|--------------------------|
//! | Static dispatch via `impl Trait` | `total_cost` | "`accept &impl Bunker` — static dispatch, no boxing" |
//! | Linear interpolation via `partition_point` | `LinearInterp` | "`partition_point` for binary search on sorted slices" |
//! | Test double (`ZeroCost`) | `ZeroCost` | "`ZeroBunker` prevents bunker consumption from confounding tests" |
//! | `entry().and_modify().or_insert_with()` | `upsert_min` | "The closure is called at most twice" |
//! | zip + skip(1) | `route_costs` | "`zip` + `skip(1)` for adjacent-pair iteration" |

use std::collections::HashMap;

// ------------------------------------------------------------------ //
//  1. The CostFn trait — static dispatch, no boxing                  //
// ------------------------------------------------------------------ //

/// A function that computes a cost for a given speed.
/// All functions that need a cost curve accept `&impl CostFn` (static dispatch).
///
/// RUSTPATTERNS.md §3:
/// > "All solver functions accept `&impl Bunker` — static dispatch, no boxing."
pub trait CostFn {
    fn cost(&self, speed: f64) -> f64;
}

// ------------------------------------------------------------------ //
//  2. Production implementation — linear interpolation               //
// ------------------------------------------------------------------ //

/// Linear interpolator built from a table of `(speed, cost)` pairs.
///
/// Invariants:
/// - `xs` and `ys` have the same length (≥ 2).
/// - `xs` is strictly sorted ascending.
///
/// `cost(speed)`:
/// - Clamp `speed` to `[xs[0], xs[last]]`.
/// - Find the interval `[xs[i-1], xs[i]]` containing `speed` using `partition_point`.
/// - Return the linearly interpolated `y` value in that interval.
///
/// RUSTPATTERNS.md §6:
/// > "`partition_point` for binary search on sorted slices"
/// > "`let idx = windows.partition_point(|w| w.end_diff_sec <= eta_diff_sec);`"
pub struct LinearInterp {
    pub xs: Vec<f64>,
    pub ys: Vec<f64>,
}

impl LinearInterp {
    /// Construct a new interpolator.
    /// Panics if `xs.len() != ys.len()` or `xs.len() < 2`.
    pub fn new(xs: Vec<f64>, ys: Vec<f64>) -> Self {
        assert_eq!(xs.len(), ys.len(), "xs and ys must have equal length");
        assert!(xs.len() >= 2, "need at least 2 points");
        LinearInterp { xs, ys }
    }
}

impl CostFn for LinearInterp {
    /// TODO:
    /// 1. Clamp speed: `let s = speed.clamp(self.xs[0], *self.xs.last().unwrap());`
    /// 2. Find the interval index: `let i = self.xs.partition_point(|&x| x <= s);`
    ///    then adjust: `let i = i.min(self.xs.len() - 1).max(1);`
    /// 3. Interpolate between `(xs[i-1], ys[i-1])` and `(xs[i], ys[i])`:
    ///    `let t = (s - self.xs[i-1]) / (self.xs[i] - self.xs[i-1]);`
    ///    `return self.ys[i-1] + t * (self.ys[i] - self.ys[i-1]);`
    fn cost(&self, speed: f64) -> f64 {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  3. Test double — ZeroCost                                          //
// ------------------------------------------------------------------ //

/// A `CostFn` that always returns 0.0 — the Rust equivalent of a test double.
///
/// Prevents cost from confounding assertions in tests that care about structure,
/// not about the specific cost values.
///
/// RUSTPATTERNS.md §10:
/// > "`ZeroBunker` prevents bunker consumption from confounding objective-value assertions"
pub struct ZeroCost;

impl CostFn for ZeroCost {
    fn cost(&self, _speed: f64) -> f64 { 0.0 }
}

/// A `CostFn` with constant cost equal to `value` — for predictable test assertions.
pub struct ConstantCost {
    pub value: f64,
}

impl CostFn for ConstantCost {
    fn cost(&self, _speed: f64) -> f64 { self.value }
}

// ------------------------------------------------------------------ //
//  4. Static dispatch functions                                       //
// ------------------------------------------------------------------ //

/// Compute the total cost for a list of (speed, hours) legs.
/// `cost_per_hour = cost_fn.cost(speed)` and `total += cost_per_hour * hours`.
///
/// Accepts `&impl CostFn` — works with any type that implements the trait,
/// selected at compile time (static dispatch, no `dyn`).
///
/// TODO: iterate over legs, accumulate `cost_fn.cost(speed) * hours`.
pub fn total_cost(legs: &[(f64, f64)], cost_fn: &impl CostFn) -> f64 {
    todo!()
}

/// Compute the cost for each adjacent pair of waypoints along a route.
///
/// Given a list of speeds `[s0, s1, s2, ...]` and travel times `[t0, t1, t2, ...]`,
/// pair them up: `[(s0,t0), (s1,t1), ...]` then return `cost(s_i) * t_i` for each.
///
/// This uses the **zip + skip(1)** idiom — here we zip two parallel slices:
/// ```ignore
/// speeds.iter().zip(times.iter()).map(|(&s, &t)| cost_fn.cost(s) * t).collect()
/// ```
///
/// TODO: implement the zip iteration.
pub fn route_costs(speeds: &[f64], times: &[f64], cost_fn: &impl CostFn) -> Vec<f64> {
    todo!()
}

// ------------------------------------------------------------------ //
//  5. entry().and_modify().or_insert_with() pattern                  //
// ------------------------------------------------------------------ //

/// Insert `candidate` into `map` under `key` only if it is smaller than
/// the current value, or if `key` does not exist yet.
///
/// This is the **`entry().and_modify().or_insert_with()`** pattern from RUSTPATTERNS.md §6:
/// ```ignore
/// map.entry(key)
///     .and_modify(|e| { if candidate < *e { *e = candidate; } })
///     .or_insert(candidate);
/// ```
///
/// TODO: implement the entry update.
pub fn upsert_min(map: &mut HashMap<String, f64>, key: String, candidate: f64) {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    fn make_interp() -> LinearInterp {
        // Speed:  [0.0,  5.0,  10.0]
        // Cost:   [0.0, 10.0,  40.0]
        LinearInterp::new(vec![0.0, 5.0, 10.0], vec![0.0, 10.0, 40.0])
    }

    // --- LinearInterp ---

    #[test]
    fn linear_interp_at_knot_points() {
        let f = make_interp();
        assert!((f.cost(0.0) - 0.0).abs() < 1e-9);
        assert!((f.cost(5.0) - 10.0).abs() < 1e-9);
        assert!((f.cost(10.0) - 40.0).abs() < 1e-9);
    }

    #[test]
    fn linear_interp_midpoint_first_interval() {
        let f = make_interp();
        // speed=2.5 is halfway between 0 and 5 → cost = halfway between 0 and 10 = 5
        assert!((f.cost(2.5) - 5.0).abs() < 1e-9);
    }

    #[test]
    fn linear_interp_midpoint_second_interval() {
        let f = make_interp();
        // speed=7.5 is halfway between 5 and 10 → cost = halfway between 10 and 40 = 25
        assert!((f.cost(7.5) - 25.0).abs() < 1e-9);
    }

    #[test]
    fn linear_interp_clamped_below() {
        let f = make_interp();
        // speed < xs[0] → clamp to 0.0 → cost = 0.0
        assert!((f.cost(-5.0) - 0.0).abs() < 1e-9);
    }

    #[test]
    fn linear_interp_clamped_above() {
        let f = make_interp();
        // speed > xs[last] → clamp to 10.0 → cost = 40.0
        assert!((f.cost(20.0) - 40.0).abs() < 1e-9);
    }

    // --- ZeroCost test double ---

    #[test]
    fn zero_cost_always_zero() {
        let zc = ZeroCost;
        assert_eq!(zc.cost(0.0), 0.0);
        assert_eq!(zc.cost(999.0), 0.0);
    }

    #[test]
    fn total_cost_with_zero_cost_is_zero() {
        let legs = vec![(10.0_f64, 2.0_f64), (15.0, 3.0)];
        assert_eq!(total_cost(&legs, &ZeroCost), 0.0);
    }

    // --- total_cost ---

    #[test]
    fn total_cost_constant_cost() {
        // cost(speed) = 2.0 for any speed; legs: 3 hours, 5 hours → total = 16.0
        let legs = vec![(1.0_f64, 3.0_f64), (2.0, 5.0)];
        let result = total_cost(&legs, &ConstantCost { value: 2.0 });
        assert!((result - 16.0).abs() < 1e-9);
    }

    #[test]
    fn total_cost_empty_legs() {
        assert_eq!(total_cost(&[], &ZeroCost), 0.0);
    }

    // --- route_costs ---

    #[test]
    fn route_costs_constant_cost() {
        let speeds = vec![1.0, 2.0, 3.0];
        let times  = vec![2.0, 3.0, 4.0];
        let costs = route_costs(&speeds, &times, &ConstantCost { value: 5.0 });
        assert_eq!(costs, vec![10.0, 15.0, 20.0]);
    }

    #[test]
    fn route_costs_different_lengths_uses_shorter() {
        let speeds = vec![1.0, 2.0, 3.0];
        let times  = vec![2.0, 3.0];
        let costs = route_costs(&speeds, &times, &ConstantCost { value: 1.0 });
        assert_eq!(costs.len(), 2);
    }

    // --- upsert_min ---

    #[test]
    fn upsert_min_inserts_new_key() {
        let mut map = HashMap::new();
        upsert_min(&mut map, "k".to_string(), 5.0);
        assert!((map["k"] - 5.0).abs() < 1e-9);
    }

    #[test]
    fn upsert_min_updates_when_smaller() {
        let mut map = HashMap::new();
        upsert_min(&mut map, "k".to_string(), 10.0);
        upsert_min(&mut map, "k".to_string(), 3.0);
        assert!((map["k"] - 3.0).abs() < 1e-9);
    }

    #[test]
    fn upsert_min_keeps_existing_when_larger_candidate() {
        let mut map = HashMap::new();
        upsert_min(&mut map, "k".to_string(), 3.0);
        upsert_min(&mut map, "k".to_string(), 10.0);
        assert!((map["k"] - 3.0).abs() < 1e-9);
    }
}
