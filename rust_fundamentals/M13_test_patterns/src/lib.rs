//! # Module 13 — Test Patterns & Ownership Philosophy
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! This module exercises RUSTPATTERNS.md §8, §10, §11, §12:
//!
//! | Pattern | Types / Functions | RUSTPATTERNS.md reference |
//! |---------|-------------------|--------------------------|
//! | Custom `PartialEq` with epsilon | `Kpi` | §8 — "Epsilon comparison instead of bitwise equality" |
//! | Factory helpers | `make_config`, `make_kpi` | §10 — "`make_<type>()` functions with sensible defaults" |
//! | `rstest` parametrisation | test functions | §10 — "`#[rstest]` parametrisation for table-driven tests" |
//! | `debug_assert!` for invariants | `binary_search_first_ge` | §12 — "checked in debug builds, elided in release" |
//! | `unreachable!` for constraints | `classify_score` | §12 — "`unreachable!` documents constraints, not just panics" |
//! | Closures borrow, never own | `apply_to_all` | §12 — "Inner closures capture by reference, mutate only local accumulators" |
//! | Surgical `.clone()` | `process_items` | §12 — "only where necessary" |

// ------------------------------------------------------------------ //
//  1. Custom PartialEq with epsilon — RUSTPATTERNS.md §8              //
// ------------------------------------------------------------------ //

/// A named KPI with a floating-point value.
///
/// `PartialEq` uses epsilon comparison (1e-6) instead of bitwise equality,
/// because floating-point arithmetic is not exact.
///
/// RUSTPATTERNS.md §8:
/// > "Epsilon comparison is used instead of bitwise equality."
/// > "`Eq` is a marker impl after the custom `PartialEq` — required for `assert_eq!`."
#[derive(Debug, Clone)]
pub struct Kpi {
    pub name: String,
    pub value: f64,
}

impl PartialEq for Kpi {
    /// TODO:
    /// - Return `false` if `self.name != other.name`.
    /// - Return `(self.value - other.value).abs() < 1e-6`.
    fn eq(&self, other: &Self) -> bool {
        todo!()
    }
}

/// Required so `Kpi` can be used with `assert_eq!` and in `HashSet`s.
impl Eq for Kpi {}

// ------------------------------------------------------------------ //
//  2. Factory helpers — RUSTPATTERNS.md §10                          //
// ------------------------------------------------------------------ //

/// A server configuration struct with many fields.
#[derive(Debug, Clone)]
pub struct ServerConfig {
    pub host: String,
    pub port: u16,
    pub timeout_ms: u32,
    pub max_retries: u32,
    pub enable_tls: bool,
}

/// Construct a `ServerConfig` with sensible defaults, overriding only `host`.
/// Used as a factory helper in tests — callers then use struct update syntax
/// to override only the fields that matter for a given test.
///
/// Defaults: port=8080, timeout_ms=5000, max_retries=3, enable_tls=false.
///
/// RUSTPATTERNS.md §10:
/// > "test-local constructors that return a fully-populated struct with sensible defaults"
///
/// TODO: construct `ServerConfig` with the given host and the documented defaults.
pub fn make_config(host: &str) -> ServerConfig {
    todo!()
}

/// Construct a `Kpi` with the given name and value.
///
/// TODO: `Kpi { name: name.to_string(), value }`.
pub fn make_kpi(name: &str, value: f64) -> Kpi {
    todo!()
}

// ------------------------------------------------------------------ //
//  3. debug_assert! for invariants — RUSTPATTERNS.md §12             //
// ------------------------------------------------------------------ //

/// Return the index of the first element ≥ `target` in the **sorted** slice.
/// Returns `sorted.len()` if no such element exists.
///
/// Precondition: `sorted` must be non-decreasing.
/// This is checked with `debug_assert!` in debug builds and elided in release.
///
/// RUSTPATTERNS.md §12:
/// > "`debug_assert!` for internal invariants. Checked in debug builds, elided in release —
/// > used where the invariant is guaranteed by surrounding logic."
///
/// TODO:
/// 1. `debug_assert!(sorted.windows(2).all(|w| w[0] <= w[1]), "slice must be sorted");`
/// 2. Return `sorted.partition_point(|&x| x < target)`.
pub fn binary_search_first_ge(sorted: &[i32], target: i32) -> usize {
    todo!()
}

// ------------------------------------------------------------------ //
//  4. unreachable! — RUSTPATTERNS.md §12                             //
// ------------------------------------------------------------------ //

/// Classify a score in `[0, 100]` into a band.
///
/// - 0–49  → "failing"
/// - 50–74 → "passing"
/// - 75–89 → "merit"
/// - 90–100 → "distinction"
///
/// Scores > 100 are **impossible by construction** (enforced by the caller).
/// `unreachable!` documents this — if it ever fires, it's a bug.
///
/// RUSTPATTERNS.md §12:
/// > "`unreachable!` documents constraints, not just panics. The DP is designed
/// > so this branch is impossible. If it ever fires, it indicates a bug."
///
/// TODO: implement the match and add `unreachable!("score > 100 is impossible")` for the _ arm.
pub fn classify_score(score: u8) -> &'static str {
    todo!()
}

// ------------------------------------------------------------------ //
//  5. Closures borrow, never own — RUSTPATTERNS.md §12               //
// ------------------------------------------------------------------ //

/// Apply `f` to each element of `items` and collect results.
/// The closure borrows `items` by reference — it never takes ownership.
///
/// RUSTPATTERNS.md §12:
/// > "Closures borrow, never own. Inner closures capture `&input`, `&leg` by reference
/// > and mutate only their local accumulator."
///
/// TODO: `items.iter().map(|x| f(x)).collect()`
pub fn apply_to_all<T, U, F>(items: &[T], f: F) -> Vec<U>
where
    F: Fn(&T) -> U,
{
    todo!()
}

/// Process a list of items:
/// 1. Clone the first item (if it exists) to use as a running "best".
/// 2. Update "best" if a later item is strictly greater.
/// 3. Return the best item, or `None` if `items` is empty.
///
/// RUSTPATTERNS.md §12:
/// > "`.clone()` is used surgically — only where necessary."
/// Here we clone exactly once per call: to extract the initial candidate.
///
/// TODO: clone `items[0]` as the initial best, then iterate from index 1.
pub fn find_best(items: &[i32]) -> Option<i32> {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- Custom PartialEq for Kpi ---

    #[test]
    fn kpi_equal_within_epsilon() {
        let a = Kpi { name: "speed".to_string(), value: 1.0 };
        let b = Kpi { name: "speed".to_string(), value: 1.0 + 1e-9 }; // within 1e-6
        assert_eq!(a, b);
    }

    #[test]
    fn kpi_not_equal_outside_epsilon() {
        let a = Kpi { name: "speed".to_string(), value: 1.0 };
        let b = Kpi { name: "speed".to_string(), value: 1.01 }; // outside 1e-6
        assert_ne!(a, b);
    }

    #[test]
    fn kpi_different_names_not_equal() {
        let a = Kpi { name: "speed".to_string(), value: 1.0 };
        let b = Kpi { name: "cost".to_string(), value: 1.0 };
        assert_ne!(a, b);
    }

    // --- Factory helpers ---

    #[test]
    fn make_config_sets_host() {
        let c = make_config("example.com");
        assert_eq!(c.host, "example.com");
    }

    #[test]
    fn make_config_has_sensible_defaults() {
        let c = make_config("localhost");
        assert_eq!(c.port, 8080);
        assert_eq!(c.timeout_ms, 5000);
        assert_eq!(c.max_retries, 3);
        assert!(!c.enable_tls);
    }

    #[test]
    fn make_config_struct_update_syntax_overrides_one_field() {
        // Struct update syntax — only override the field under test.
        let c = ServerConfig { port: 443, enable_tls: true, ..make_config("api.example.com") };
        assert_eq!(c.host, "api.example.com");
        assert_eq!(c.port, 443);
        assert!(c.enable_tls);
        assert_eq!(c.timeout_ms, 5000); // default unchanged
    }

    #[test]
    fn make_kpi_constructs_correctly() {
        let k = make_kpi("latency", 42.5);
        assert_eq!(k.name, "latency");
        assert!((k.value - 42.5).abs() < 1e-9);
    }

    // --- rstest parametrised tests ---
    // RUSTPATTERNS.md §10: "`#[rstest]` parametrisation for table-driven tests"

    #[cfg(test)]
    mod parametrised {
        use rstest::rstest;
        use super::super::classify_score;

        #[rstest]
        #[case(0,   "failing")]
        #[case(49,  "failing")]
        #[case(50,  "passing")]
        #[case(74,  "passing")]
        #[case(75,  "merit")]
        #[case(89,  "merit")]
        #[case(90,  "distinction")]
        #[case(100, "distinction")]
        fn classify_score_table(#[case] score: u8, #[case] expected: &'static str) {
            assert_eq!(classify_score(score), expected);
        }

        #[rstest]
        #[case(&[],      5,  0)]
        #[case(&[3, 7],  5,  1)]
        #[case(&[1,2,3], 1,  0)]
        #[case(&[1,2,3], 4,  3)]
        fn binary_search_first_ge_table(
            #[case] sorted: &[i32],
            #[case] target: i32,
            #[case] expected: usize,
        ) {
            use super::super::binary_search_first_ge;
            assert_eq!(binary_search_first_ge(sorted, target), expected);
        }
    }

    // --- debug_assert! ---

    #[test]
    fn binary_search_first_ge_basic() {
        assert_eq!(binary_search_first_ge(&[1, 3, 5, 7, 9], 5), 2);
    }

    #[test]
    fn binary_search_first_ge_all_less_than_target() {
        assert_eq!(binary_search_first_ge(&[1, 2, 3], 10), 3);
    }

    #[test]
    fn binary_search_first_ge_all_ge_target() {
        assert_eq!(binary_search_first_ge(&[5, 6, 7], 3), 0);
    }

    // --- apply_to_all ---

    #[test]
    fn apply_to_all_doubles() {
        assert_eq!(apply_to_all(&[1, 2, 3], |x| x * 2), vec![2, 4, 6]);
    }

    #[test]
    fn apply_to_all_empty() {
        let result = apply_to_all(&[] as &[i32], |x| x * 2);
        assert_eq!(result, vec![]);
    }

    #[test]
    fn apply_to_all_borrows_do_not_consume_slice() {
        let items = vec![1, 2, 3];
        let _doubled = apply_to_all(&items, |x| x * 2);
        // `items` is still accessible after the call — the closure borrowed, not moved.
        assert_eq!(items.len(), 3);
    }

    // --- find_best ---

    #[test]
    fn find_best_returns_maximum() {
        assert_eq!(find_best(&[3, 1, 4, 1, 5, 9, 2, 6]), Some(9));
    }

    #[test]
    fn find_best_single_element() {
        assert_eq!(find_best(&[42]), Some(42));
    }

    #[test]
    fn find_best_empty_returns_none() {
        assert_eq!(find_best(&[]), None);
    }
}
