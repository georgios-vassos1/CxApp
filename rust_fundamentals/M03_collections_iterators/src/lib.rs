//! # Module 3 — Collections & Iterators
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! | Group | Functions | Concept |
//! |-------|-----------|---------|
//! | HashMap | `word_count` | `entry().or_insert(0)`, `*e += 1` |
//! | Vec operations | `deduplicate`, `top_k` | sort, dedup, truncate |
//! | Iterator chains | `running_sum`, `adjacent_differences`, `flat_filter` | scan, zip+skip(1), flat_map |

use std::collections::HashMap;

// ------------------------------------------------------------------ //
//  1. HashMap & the entry API                                         //
// ------------------------------------------------------------------ //

/// Count how many times each word appears in `words`.
///
/// TODO:
/// - Create an empty `HashMap<String, usize>`.
/// - For each word: `map.entry(word.to_string()).or_insert(0)` then `+= 1`.
/// - Return the map.
pub fn word_count(words: &[&str]) -> HashMap<String, usize> {
    todo!()
}

/// Given a `HashMap`, increment the counter for `key` by `amount`.
/// If `key` does not exist yet, insert it with value `amount`.
///
/// Practises: `entry().and_modify(|v| *v += amount).or_insert(amount)`.
pub fn increment_counter(map: &mut HashMap<String, usize>, key: &str, amount: usize) {
    todo!()
}

// ------------------------------------------------------------------ //
//  2. Vec operations                                                  //
// ------------------------------------------------------------------ //

/// Return `v` sorted ascending with consecutive duplicates removed.
/// Takes ownership of `v` (sorting requires mutation).
///
/// TODO: `v.sort()`, `v.dedup()`, return `v`.
pub fn deduplicate(mut v: Vec<i32>) -> Vec<i32> {
    todo!()
}

/// Return the `k` largest elements of `v`, sorted descending.
/// If `k >= v.len()`, return all elements sorted descending.
///
/// TODO: sort descending, truncate to k, return.
/// Hint: `v.sort_by(|a, b| b.cmp(a))` for descending order.
pub fn top_k(mut v: Vec<i32>, k: usize) -> Vec<i32> {
    todo!()
}

// ------------------------------------------------------------------ //
//  3. Iterator chains                                                 //
// ------------------------------------------------------------------ //

/// Return the running (prefix) sum of `v`.
/// running_sum(&[1, 2, 3, 4]) == [1, 3, 6, 10].
///
/// TODO: use `Iterator::scan` with accumulator starting at 0.
/// `v.iter().scan(0i32, |acc, &x| { *acc += x; Some(*acc) }).collect()`
pub fn running_sum(v: &[i32]) -> Vec<i32> {
    todo!()
}

/// Return a Vec containing the difference between each adjacent pair:
/// `[v[1]-v[0], v[2]-v[1], ..., v[n-1]-v[n-2]]`.
/// Returns an empty Vec for inputs of length 0 or 1.
///
/// This is the **zip + skip(1)** pattern from RUSTPATTERNS.md §6:
/// ```ignore
/// v.iter().zip(v.iter().skip(1)).map(|(a, b)| b - a).collect()
/// ```
pub fn adjacent_differences(v: &[i32]) -> Vec<i32> {
    todo!()
}

/// For each element of `v`, apply `f` — if it returns `Some(x)`, keep `x`.
/// Flatten one level: each element of `v` is a `Vec<i32>`,
/// and `f` maps `i32 -> Option<i32>`.
///
/// Practises: `flat_map` + `filter_map`.
/// TODO: `v.iter().flat_map(|inner| inner.iter().filter_map(|&x| f(x))).collect()`
pub fn flat_filter<F>(v: &[Vec<i32>], f: F) -> Vec<i32>
where
    F: Fn(i32) -> Option<i32>,
{
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- word_count ---

    #[test]
    fn word_count_empty() {
        assert_eq!(word_count(&[]).len(), 0);
    }

    #[test]
    fn word_count_single_word() {
        let m = word_count(&["hello"]);
        assert_eq!(m["hello"], 1);
    }

    #[test]
    fn word_count_repeated_word() {
        let m = word_count(&["a", "b", "a", "a"]);
        assert_eq!(m["a"], 3);
        assert_eq!(m["b"], 1);
    }

    #[test]
    fn word_count_multiple_distinct_words() {
        let words = ["the", "quick", "brown", "fox", "the", "fox"];
        let m = word_count(&words);
        assert_eq!(m["the"], 2);
        assert_eq!(m["fox"], 2);
        assert_eq!(m["quick"], 1);
        assert_eq!(m["brown"], 1);
    }

    // --- increment_counter ---

    #[test]
    fn increment_counter_new_key() {
        let mut map = HashMap::new();
        increment_counter(&mut map, "x", 5);
        assert_eq!(map["x"], 5);
    }

    #[test]
    fn increment_counter_existing_key() {
        let mut map = HashMap::new();
        increment_counter(&mut map, "x", 3);
        increment_counter(&mut map, "x", 7);
        assert_eq!(map["x"], 10);
    }

    // --- deduplicate ---

    #[test]
    fn deduplicate_removes_duplicates() {
        assert_eq!(deduplicate(vec![3, 1, 2, 1, 3, 2]), vec![1, 2, 3]);
    }

    #[test]
    fn deduplicate_no_duplicates() {
        assert_eq!(deduplicate(vec![5, 2, 8, 1]), vec![1, 2, 5, 8]);
    }

    #[test]
    fn deduplicate_all_same() {
        assert_eq!(deduplicate(vec![4, 4, 4]), vec![4]);
    }

    #[test]
    fn deduplicate_empty() {
        assert_eq!(deduplicate(vec![]), vec![]);
    }

    // --- top_k ---

    #[test]
    fn top_k_returns_k_largest_descending() {
        assert_eq!(top_k(vec![3, 1, 4, 1, 5, 9, 2, 6], 3), vec![9, 6, 5]);
    }

    #[test]
    fn top_k_larger_than_len_returns_all_descending() {
        assert_eq!(top_k(vec![3, 1, 2], 10), vec![3, 2, 1]);
    }

    #[test]
    fn top_k_zero_k_returns_empty() {
        assert_eq!(top_k(vec![1, 2, 3], 0), vec![]);
    }

    // --- running_sum ---

    #[test]
    fn running_sum_typical() {
        assert_eq!(running_sum(&[1, 2, 3, 4]), vec![1, 3, 6, 10]);
    }

    #[test]
    fn running_sum_single_element() {
        assert_eq!(running_sum(&[7]), vec![7]);
    }

    #[test]
    fn running_sum_empty() {
        assert_eq!(running_sum(&[]), vec![]);
    }

    #[test]
    fn running_sum_with_negatives() {
        assert_eq!(running_sum(&[5, -2, 3]), vec![5, 3, 6]);
    }

    // --- adjacent_differences ---

    #[test]
    fn adjacent_differences_typical() {
        assert_eq!(adjacent_differences(&[1, 3, 6, 10]), vec![2, 3, 4]);
    }

    #[test]
    fn adjacent_differences_single_element() {
        assert_eq!(adjacent_differences(&[42]), vec![]);
    }

    #[test]
    fn adjacent_differences_empty() {
        assert_eq!(adjacent_differences(&[]), vec![]);
    }

    #[test]
    fn adjacent_differences_decreasing() {
        assert_eq!(adjacent_differences(&[10, 7, 3]), vec![-3, -4]);
    }

    // --- flat_filter ---

    #[test]
    fn flat_filter_keeps_evens() {
        let input = vec![vec![1, 2, 3], vec![4, 5, 6]];
        let result = flat_filter(&input, |x| if x % 2 == 0 { Some(x) } else { None });
        assert_eq!(result, vec![2, 4, 6]);
    }

    #[test]
    fn flat_filter_transforms_and_filters() {
        let input = vec![vec![1, 2, 3]];
        let result = flat_filter(&input, |x| if x > 1 { Some(x * 10) } else { None });
        assert_eq!(result, vec![20, 30]);
    }

    #[test]
    fn flat_filter_empty_input() {
        let result = flat_filter(&[], |x| Some(x));
        assert_eq!(result, vec![]);
    }
}
