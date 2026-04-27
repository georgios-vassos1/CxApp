//! # Module 1 — Ownership & Borrowing
//!
//! **Reference implementation** — all tests pass.
//!
//! This module builds the mental model for Rust's ownership system.
//! Every function here is a deliberate exercise in one aspect of that system:
//!
//! | Group | Functions | Concept |
//! |-------|-----------|---------|
//! | Slices & lifetimes | `first_word`, `longer` | borrowed slices, lifetime annotations |
//! | Immutable borrows   | `sum_slice`, `count_above` | `&[T]` — read-only, non-owning |
//! | Mutable borrows     | `double_in_place`, `clamp_slice` | `&mut [T]` — write through a borrow |
//! | Ownership transfer  | `clone_vec`, `dedup_sorted` | when and why to copy heap data |

// ------------------------------------------------------------------ //
//  1. Slices & Lifetimes                                              //
// ------------------------------------------------------------------ //

/// Returns a slice of `s` up to (but not including) the first space.
/// If `s` contains no spaces, the entire string is returned.
/// If `s` is empty, an empty string slice is returned.
///
/// The returned slice borrows from `s`, which is why the lifetime of
/// the return value is tied to the lifetime of the input.
pub fn first_word(s: &str) -> &str {
    match s.find(' ') {
        Some(i) => &s[..i],
        None    => s,
    }
}

/// Returns the longer of the two string slices.
/// If they are equal in length, `x` is returned.
///
/// The explicit lifetime `'a` tells the compiler:
/// "the returned reference lives at least as long as *both* inputs."
pub fn longer<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() >= y.len() { x } else { y }
}

// ------------------------------------------------------------------ //
//  2. Immutable borrows                                               //
// ------------------------------------------------------------------ //

/// Returns the sum of all elements in `nums`.
/// `nums` is borrowed — the caller retains ownership.
pub fn sum_slice(nums: &[i32]) -> i32 {
    nums.iter().sum()
}

/// Returns the count of elements strictly greater than `threshold`.
pub fn count_above(nums: &[i32], threshold: i32) -> usize {
    nums.iter().filter(|&&x| x > threshold).count()
}

// ------------------------------------------------------------------ //
//  3. Mutable borrows                                                 //
// ------------------------------------------------------------------ //

/// Doubles every element of `nums` in place.
/// Takes a mutable borrow — no allocation needed.
pub fn double_in_place(nums: &mut [i32]) {
    for n in nums.iter_mut() {
        *n *= 2;
    }
}

/// Clamps every element of `nums` to the range `[lo, hi]` in place.
pub fn clamp_slice(nums: &mut [i32], lo: i32, hi: i32) {
    for n in nums.iter_mut() {
        if *n < lo { *n = lo; }
        else if *n > hi { *n = hi; }
    }
}

// ------------------------------------------------------------------ //
//  4. Ownership transfer                                              //
// ------------------------------------------------------------------ //

/// Returns a new `Vec<i32>` that is an independent copy of `v`.
/// Practises: `.to_vec()` (clone-from-slice) and the "caller owns the result" contract.
pub fn clone_vec(v: &[i32]) -> Vec<i32> {
    v.to_vec()
}

/// Takes ownership of `v`, sorts it, removes consecutive duplicates,
/// and returns the deduplicated `Vec`.
/// Practises: taking ownership by value, in-place mutation, returning ownership.
pub fn dedup_sorted(mut v: Vec<i32>) -> Vec<i32> {
    v.sort();
    v.dedup();
    v
}

// ------------------------------------------------------------------ //
//  Tests — all must pass (this is the reference implementation)      //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- first_word ---

    #[test]
    fn first_word_typical_sentence() {
        assert_eq!(first_word("hello world"), "hello");
    }

    #[test]
    fn first_word_single_word() {
        assert_eq!(first_word("rust"), "rust");
    }

    #[test]
    fn first_word_empty() {
        assert_eq!(first_word(""), "");
    }

    #[test]
    fn first_word_leading_space() {
        assert_eq!(first_word(" hello"), "");
    }

    // --- longer ---

    #[test]
    fn longer_returns_longer_string() {
        assert_eq!(longer("abc", "de"), "abc");
        assert_eq!(longer("de", "abc"), "abc");
    }

    #[test]
    fn longer_equal_length_returns_first() {
        assert_eq!(longer("ab", "cd"), "ab");
    }

    // --- sum_slice ---

    #[test]
    fn sum_slice_empty() {
        assert_eq!(sum_slice(&[]), 0);
    }

    #[test]
    fn sum_slice_positive() {
        assert_eq!(sum_slice(&[1, 2, 3, 4, 5]), 15);
    }

    #[test]
    fn sum_slice_mixed_signs() {
        assert_eq!(sum_slice(&[-3, 7, -2, 8]), 10);
    }

    // --- count_above ---

    #[test]
    fn count_above_none() {
        assert_eq!(count_above(&[1, 2, 3], 10), 0);
    }

    #[test]
    fn count_above_some() {
        assert_eq!(count_above(&[1, 5, 3, 8, 2], 4), 2);
    }

    #[test]
    fn count_above_boundary_is_exclusive() {
        assert_eq!(count_above(&[4, 5, 6], 5), 1); // only 6
    }

    // --- double_in_place ---

    #[test]
    fn double_in_place_typical() {
        let mut v = vec![1, 2, 3, 4];
        double_in_place(&mut v);
        assert_eq!(v, vec![2, 4, 6, 8]);
    }

    #[test]
    fn double_in_place_empty() {
        let mut v: Vec<i32> = vec![];
        double_in_place(&mut v);
        assert_eq!(v, vec![]);
    }

    // --- clamp_slice ---

    #[test]
    fn clamp_slice_all_in_range() {
        let mut v = vec![3, 4, 5];
        clamp_slice(&mut v, 0, 10);
        assert_eq!(v, vec![3, 4, 5]);
    }

    #[test]
    fn clamp_slice_below_lo() {
        let mut v = vec![-5, 0, 3];
        clamp_slice(&mut v, 0, 10);
        assert_eq!(v, vec![0, 0, 3]);
    }

    #[test]
    fn clamp_slice_above_hi() {
        let mut v = vec![8, 11, 100];
        clamp_slice(&mut v, 0, 10);
        assert_eq!(v, vec![8, 10, 10]);
    }

    // --- clone_vec ---

    #[test]
    fn clone_vec_returns_equal_but_independent() {
        let original = vec![10, 20, 30];
        let mut copy = clone_vec(&original);
        copy[0] = 999;
        assert_eq!(original[0], 10); // original unaffected
        assert_eq!(copy, vec![999, 20, 30]);
    }

    #[test]
    fn clone_vec_empty() {
        let v: Vec<i32> = vec![];
        assert_eq!(clone_vec(&v), vec![]);
    }

    // --- dedup_sorted ---

    #[test]
    fn dedup_sorted_removes_duplicates() {
        assert_eq!(dedup_sorted(vec![3, 1, 2, 1, 3]), vec![1, 2, 3]);
    }

    #[test]
    fn dedup_sorted_no_duplicates() {
        assert_eq!(dedup_sorted(vec![5, 3, 1]), vec![1, 3, 5]);
    }

    #[test]
    fn dedup_sorted_all_same() {
        assert_eq!(dedup_sorted(vec![7, 7, 7, 7]), vec![7]);
    }

    #[test]
    fn dedup_sorted_empty() {
        assert_eq!(dedup_sorted(vec![]), vec![]);
    }
}
