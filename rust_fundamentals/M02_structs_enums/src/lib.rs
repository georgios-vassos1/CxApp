//! # Module 2 — Structs, Methods & Enums
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! | Group | Types / Functions | Concept |
//! |-------|-------------------|---------|
//! | Structs & methods | `Point` | `impl` blocks, associated functions, methods |
//! | Enums & match     | `Direction` | exhaustive match, data-carrying variants |
//! | Option<T>         | `first_even`, `safe_divide` | `map`, `and_then`, `unwrap_or` |

// ------------------------------------------------------------------ //
//  1. Structs & impl blocks                                           //
// ------------------------------------------------------------------ //

/// A point in 2-D space.
#[derive(Debug, Clone)]
pub struct Point {
    pub x: f64,
    pub y: f64,
}

impl Point {
    /// Construct a new `Point` at `(x, y)`.
    ///
    /// TODO: store x and y in the struct.
    pub fn new(x: f64, y: f64) -> Self {
        todo!()
    }

    /// Construct the origin `(0.0, 0.0)`.
    ///
    /// TODO: delegate to `Point::new`.
    pub fn origin() -> Self {
        todo!()
    }

    /// Return the Euclidean distance between `self` and `other`.
    ///
    /// Formula: sqrt((x2-x1)^2 + (y2-y1)^2).
    /// Hint: `f64::sqrt`, or `(dx*dx + dy*dy).sqrt()`.
    pub fn distance_to(&self, other: &Point) -> f64 {
        todo!()
    }

    /// Return a new `Point` shifted by `(dx, dy)`.
    /// Does NOT mutate `self`.
    ///
    /// TODO: construct Point::new(self.x + dx, self.y + dy).
    pub fn translate(&self, dx: f64, dy: f64) -> Point {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  2. Enums & exhaustive match                                        //
// ------------------------------------------------------------------ //

/// A compass direction.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Direction {
    North,
    South,
    East,
    West,
}

impl Direction {
    /// Return the opposite direction.
    ///
    /// TODO: match on self and return the 180° flip.
    pub fn opposite(&self) -> Direction {
        todo!()
    }

    /// Return `true` if this direction is North or South.
    ///
    /// TODO: match or use a comparison.
    pub fn is_vertical(&self) -> bool {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  3. Option<T> — presence/absence of a value                        //
// ------------------------------------------------------------------ //

/// Return the first even number in `nums`, or `None` if there is none.
///
/// Hint: `Iterator::find`, or a for-loop with an early return.
pub fn first_even(nums: &[i32]) -> Option<i32> {
    todo!()
}

/// Divide `a` by `b`, returning `None` if `b` is zero.
///
/// TODO: if b == 0.0 return None, else return Some(a / b).
pub fn safe_divide(a: f64, b: f64) -> Option<f64> {
    todo!()
}

/// Square `opt` if it is `Some`, otherwise return `None`.
///
/// Practises `Option::map`.
/// TODO: use `opt.map(|v| v * v)`.
pub fn square_opt(opt: Option<f64>) -> Option<f64> {
    todo!()
}

/// Return the value inside `opt`, or `default` if it is `None`.
///
/// Practises `Option::unwrap_or`.
/// TODO: use `opt.unwrap_or(default)`.
pub fn unwrap_or_default(opt: Option<i32>, default: i32) -> i32 {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests — TDD red phase: all non-trivial tests must fail initially  //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- Point::new / origin ---

    #[test]
    fn point_new_stores_coordinates() {
        let p = Point::new(3.0, 4.0);
        assert_eq!(p.x, 3.0);
        assert_eq!(p.y, 4.0);
    }

    #[test]
    fn point_origin_is_zero_zero() {
        let o = Point::origin();
        assert_eq!(o.x, 0.0);
        assert_eq!(o.y, 0.0);
    }

    // --- Point::distance_to ---

    #[test]
    fn point_distance_to_self_is_zero() {
        let p = Point::new(1.0, 1.0);
        assert!((p.distance_to(&p) - 0.0).abs() < 1e-9);
    }

    #[test]
    fn point_distance_3_4_gives_5() {
        let a = Point::origin();
        let b = Point::new(3.0, 4.0);
        assert!((a.distance_to(&b) - 5.0).abs() < 1e-9);
    }

    #[test]
    fn point_distance_is_symmetric() {
        let a = Point::new(1.0, 2.0);
        let b = Point::new(4.0, 6.0);
        assert!((a.distance_to(&b) - b.distance_to(&a)).abs() < 1e-9);
    }

    // --- Point::translate ---

    #[test]
    fn point_translate_returns_new_point() {
        let p = Point::new(1.0, 2.0);
        let q = p.translate(3.0, -1.0);
        assert!((q.x - 4.0).abs() < 1e-9);
        assert!((q.y - 1.0).abs() < 1e-9);
        // original unchanged
        assert!((p.x - 1.0).abs() < 1e-9);
    }

    #[test]
    fn point_translate_zero_delta_is_identity() {
        let p = Point::new(5.0, 7.0);
        let q = p.translate(0.0, 0.0);
        assert!((q.x - p.x).abs() < 1e-9);
        assert!((q.y - p.y).abs() < 1e-9);
    }

    // --- Direction::opposite ---

    #[test]
    fn direction_opposite_north_is_south() {
        assert_eq!(Direction::North.opposite(), Direction::South);
    }

    #[test]
    fn direction_opposite_east_is_west() {
        assert_eq!(Direction::East.opposite(), Direction::West);
    }

    #[test]
    fn direction_opposite_twice_is_identity() {
        for d in [Direction::North, Direction::South, Direction::East, Direction::West] {
            assert_eq!(d.opposite().opposite(), d);
        }
    }

    // --- Direction::is_vertical ---

    #[test]
    fn direction_north_south_are_vertical() {
        assert!(Direction::North.is_vertical());
        assert!(Direction::South.is_vertical());
    }

    #[test]
    fn direction_east_west_are_not_vertical() {
        assert!(!Direction::East.is_vertical());
        assert!(!Direction::West.is_vertical());
    }

    // --- first_even ---

    #[test]
    fn first_even_found() {
        assert_eq!(first_even(&[1, 3, 4, 6]), Some(4));
    }

    #[test]
    fn first_even_returns_first_not_any() {
        assert_eq!(first_even(&[1, 2, 4]), Some(2));
    }

    #[test]
    fn first_even_none_when_all_odd() {
        assert_eq!(first_even(&[1, 3, 5]), None);
    }

    #[test]
    fn first_even_empty_slice() {
        assert_eq!(first_even(&[]), None);
    }

    // --- safe_divide ---

    #[test]
    fn safe_divide_normal() {
        let r = safe_divide(10.0, 4.0).unwrap();
        assert!((r - 2.5).abs() < 1e-9);
    }

    #[test]
    fn safe_divide_by_zero_is_none() {
        assert_eq!(safe_divide(5.0, 0.0), None);
    }

    // --- square_opt ---

    #[test]
    fn square_opt_some() {
        assert_eq!(square_opt(Some(3.0)), Some(9.0));
    }

    #[test]
    fn square_opt_none() {
        assert_eq!(square_opt(None), None);
    }

    // --- unwrap_or_default ---

    #[test]
    fn unwrap_or_default_some() {
        assert_eq!(unwrap_or_default(Some(42), 0), 42);
    }

    #[test]
    fn unwrap_or_default_none() {
        assert_eq!(unwrap_or_default(None, 99), 99);
    }
}
