//! `core.rs` — heavy implementation module.
//!
//! Contains the actual logic.  Key visibility decisions:
//! - `compute_step` is **private** (not `pub`) — only this module can call it.
//! - `process_batch` is **`pub(crate)`** — visible anywhere in the crate, but NOT
//!   to external users of the crate.  `lib.rs` (the composition root) calls it;
//!   crate users cannot.
//!
//! RUSTPATTERNS.md §1:
//! > `solver/mod.rs` is the heavy implementation file — it contains the DP algorithm,
//! > domain structs, and public traits.

/// Private helper — compute the result for a single input value.
/// Contract: doubles the input.
///
/// Visibility: private — callers outside this module must go through `process_batch`.
///
/// TODO: return `x * 2`.
fn compute_step(x: i32) -> i32 {
    todo!()
}

/// Process a batch of inputs, applying `compute_step` to each.
///
/// Visibility: `pub(crate)` — accessible from `lib.rs` and `gateway.rs`,
/// but NOT from outside the crate.
///
/// TODO: `inputs.iter().map(|&x| compute_step(x)).collect()`.
pub(crate) fn process_batch(inputs: &[i32]) -> Vec<i32> {
    todo!()
}

// ------------------------------------------------------------------ //
//  Internal tests — directly test private functions                  //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn compute_step_doubles_input() {
        assert_eq!(compute_step(5), 10);
        assert_eq!(compute_step(0), 0);
        assert_eq!(compute_step(-3), -6);
    }

    #[test]
    fn process_batch_applies_to_all() {
        assert_eq!(process_batch(&[1, 2, 3]), vec![2, 4, 6]);
    }

    #[test]
    fn process_batch_empty() {
        assert_eq!(process_batch(&[]), vec![]);
    }
}
