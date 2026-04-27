//! # Module 6 — Closures & Functional Patterns
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! | Group | Functions | Concept |
//! |-------|-----------|---------|
//! | Higher-order functions | `apply_n_times`, `compose` | `Fn` bounds, returning closures |
//! | Predicate functions | `keep_while`, `partition_by` | `FnMut` predicates |
//! | Combinators | `zip_with`, `adjacent_map` | zip + skip(1) pattern |
//! | Binary search | `partition_threshold` | `partition_point` for sorted slices |

// ------------------------------------------------------------------ //
//  1. Higher-order functions                                          //
// ------------------------------------------------------------------ //

/// Apply `f` to `x` exactly `n` times in sequence.
/// `apply_n_times(|x| x * 2, 1, 3)` → 1 * 2 * 2 * 2 = 8.
///
/// TODO: use a loop (or fold over a range) to apply `f` repeatedly.
pub fn apply_n_times<F>(f: F, mut x: i32, n: u32) -> i32
where
    F: Fn(i32) -> i32,
{
    todo!()
}

/// Return a new closure that first applies `g`, then `f`.
/// `compose(f, g)(x)` == `f(g(x))`.
///
/// TODO: move `f` and `g` into a closure with the `move` keyword and return it.
pub fn compose<F, G>(f: F, g: G) -> impl Fn(i32) -> i32
where
    F: Fn(i32) -> i32,
    G: Fn(i32) -> i32,
{
    // TODO: replace with `move |x| f(g(x))`
    let _ = (f, g);
    move |_x| todo!()
}

// ------------------------------------------------------------------ //
//  2. Predicate functions (FnMut)                                     //
// ------------------------------------------------------------------ //

/// Collect elements of `v` while `pred` returns true.
/// Stop at the first element for which `pred` returns false.
///
/// Practises: `take_while`.
/// TODO: `v.iter().copied().take_while(|&x| pred(x)).collect()`
pub fn keep_while<P>(v: &[i32], mut pred: P) -> Vec<i32>
where
    P: FnMut(i32) -> bool,
{
    todo!()
}

/// Split `v` into two `Vec`s: elements for which `pred` is true, and the rest.
/// Returns `(trues, falses)`.
///
/// Practises: `partition`.
/// TODO: `v.iter().copied().partition(|&x| pred(x))`
pub fn partition_by<P>(v: &[i32], pred: P) -> (Vec<i32>, Vec<i32>)
where
    P: FnMut(&i32) -> bool,
{
    todo!()
}

// ------------------------------------------------------------------ //
//  3. Combinators                                                     //
// ------------------------------------------------------------------ //

/// Pair up elements of `a` and `b` and apply `f` to each pair.
/// The output length equals `min(a.len(), b.len())`.
///
/// Practises: `zip` + `map`.
/// TODO: `a.iter().zip(b.iter()).map(|(&x, &y)| f(x, y)).collect()`
pub fn zip_with<F>(a: &[i32], b: &[i32], f: F) -> Vec<i32>
where
    F: Fn(i32, i32) -> i32,
{
    todo!()
}

/// Apply `f` to each **adjacent pair** `(v[i], v[i+1])`.
/// Result length is `v.len() - 1` (empty if `v.len() <= 1`).
///
/// This is the **zip + skip(1)** idiom from RUSTPATTERNS.md §6:
/// ```ignore
/// v.iter().zip(v.iter().skip(1)).map(|(&a, &b)| f(a, b)).collect()
/// ```
pub fn adjacent_map<F>(v: &[i32], f: F) -> Vec<i32>
where
    F: Fn(i32, i32) -> i32,
{
    todo!()
}

// ------------------------------------------------------------------ //
//  4. Binary search with partition_point                             //
// ------------------------------------------------------------------ //

/// Given a **sorted** slice `sorted`, return the index of the first element
/// that is strictly greater than `threshold`.
/// Returns `sorted.len()` if all elements are ≤ `threshold`.
///
/// This is the `partition_point` pattern from RUSTPATTERNS.md §6:
/// ```ignore
/// sorted.partition_point(|&x| x <= threshold)
/// ```
pub fn partition_threshold(sorted: &[f64], threshold: f64) -> usize {
    todo!()
}

/// Return `true` if `target` is present in the **sorted** slice `sorted`.
///
/// Hint: use `sorted.binary_search(&target).is_ok()`.
pub fn binary_search_contains(sorted: &[i32], target: i32) -> bool {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- apply_n_times ---

    #[test]
    fn apply_n_times_double_three_times() {
        assert_eq!(apply_n_times(|x| x * 2, 1, 3), 8);
    }

    #[test]
    fn apply_n_times_zero_applications() {
        assert_eq!(apply_n_times(|x| x + 100, 5, 0), 5);
    }

    #[test]
    fn apply_n_times_add_one_five_times() {
        assert_eq!(apply_n_times(|x| x + 1, 0, 5), 5);
    }

    // --- compose ---

    #[test]
    fn compose_double_then_add_one() {
        let f = compose(|x| x + 1, |x| x * 2); // f(g(x)) = (x*2) + 1
        assert_eq!(f(3), 7);
    }

    #[test]
    fn compose_identity_left() {
        let f = compose(|x| x, |x| x * 3);
        assert_eq!(f(5), 15);
    }

    // --- keep_while ---

    #[test]
    fn keep_while_stops_at_first_false() {
        assert_eq!(keep_while(&[1, 3, 5, 2, 7], |x| x % 2 != 0), vec![1, 3, 5]);
    }

    #[test]
    fn keep_while_all_true() {
        assert_eq!(keep_while(&[2, 4, 6], |x| x % 2 == 0), vec![2, 4, 6]);
    }

    #[test]
    fn keep_while_first_false_returns_empty() {
        assert_eq!(keep_while(&[1, 2, 3], |x| x > 5), vec![]);
    }

    // --- partition_by ---

    #[test]
    fn partition_by_evens_odds() {
        let (evens, odds) = partition_by(&[1, 2, 3, 4, 5, 6], |x| x % 2 == 0);
        assert_eq!(evens, vec![2, 4, 6]);
        assert_eq!(odds, vec![1, 3, 5]);
    }

    #[test]
    fn partition_by_all_true() {
        let (yes, no) = partition_by(&[2, 4, 6], |x| x % 2 == 0);
        assert_eq!(yes.len(), 3);
        assert_eq!(no.len(), 0);
    }

    // --- zip_with ---

    #[test]
    fn zip_with_addition() {
        assert_eq!(zip_with(&[1, 2, 3], &[10, 20, 30], |a, b| a + b), vec![11, 22, 33]);
    }

    #[test]
    fn zip_with_different_lengths() {
        assert_eq!(zip_with(&[1, 2, 3, 4], &[10, 20], |a, b| a * b), vec![10, 40]);
    }

    #[test]
    fn zip_with_empty() {
        assert_eq!(zip_with(&[], &[1, 2, 3], |a, b| a + b), vec![]);
    }

    // --- adjacent_map ---

    #[test]
    fn adjacent_map_differences() {
        assert_eq!(adjacent_map(&[1, 4, 9, 16], |a, b| b - a), vec![3, 5, 7]);
    }

    #[test]
    fn adjacent_map_single_element() {
        assert_eq!(adjacent_map(&[42], |a, b| a + b), vec![]);
    }

    #[test]
    fn adjacent_map_empty() {
        assert_eq!(adjacent_map(&[], |a, b| a + b), vec![]);
    }

    // --- partition_threshold ---

    #[test]
    fn partition_threshold_middle() {
        let sorted = vec![1.0, 2.0, 3.0, 4.0, 5.0];
        assert_eq!(partition_threshold(&sorted, 3.0), 3); // index 3 is 4.0 > 3.0
    }

    #[test]
    fn partition_threshold_all_below() {
        let sorted = vec![1.0, 2.0, 3.0];
        assert_eq!(partition_threshold(&sorted, 5.0), 3);
    }

    #[test]
    fn partition_threshold_all_above() {
        let sorted = vec![5.0, 6.0, 7.0];
        assert_eq!(partition_threshold(&sorted, 0.0), 0);
    }

    // --- binary_search_contains ---

    #[test]
    fn binary_search_contains_found() {
        assert!(binary_search_contains(&[1, 3, 5, 7, 9], 5));
    }

    #[test]
    fn binary_search_contains_not_found() {
        assert!(!binary_search_contains(&[1, 3, 5, 7, 9], 4));
    }

    #[test]
    fn binary_search_contains_empty() {
        assert!(!binary_search_contains(&[], 1));
    }
}
