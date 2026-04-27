//! # Module 8 — Module Organisation & Visibility
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! This module is structured to mirror RUSTPATTERNS.md §1:
//!
//! - `lib.rs` (this file) is the **composition root** — the only place that knows
//!   about both `gateway` and `core` together.
//! - `gateway` is a **thin re-export gateway** — it declares the public API types.
//! - `core` is the **heavy implementation module** — it contains the logic and
//!   uses `pub(crate)` to expose internals only within the crate.
//!
//! ```
//! lib.rs         ← composition root, declares modules, pub use re-exports
//! gateway.rs     ← public types and re-exports only
//! core.rs        ← implementation; pub(crate) items
//! ```
//!
//! Exercises:
//! 1. Implement `core::compute_step` (private) and `core::process_batch` (pub(crate)).
//! 2. Implement `gateway::transform`.
//! 3. Implement `run_pipeline` here in the composition root, calling both.

pub mod gateway;
pub mod core;

// Re-export the public types so users of this crate only need `use m08_module_organisation::*`.
pub use gateway::Request;
pub use gateway::Response;

/// Composition root function — the only function that wires `gateway` and `core` together.
///
/// Pipeline:
/// 1. Extract `inputs` from the `req`.
/// 2. Call `core::process_batch(&inputs)` to compute results.
/// 3. Wrap the results in a `Response` and return it.
///
/// TODO: implement the three steps above.
pub fn run_pipeline(req: &Request) -> Response {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests — exercise the composition root                             //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn run_pipeline_doubles_each_input() {
        let req = Request { inputs: vec![1, 2, 3] };
        let resp = run_pipeline(&req);
        // core::process_batch doubles each input (see core.rs for the contract)
        assert_eq!(resp.outputs, vec![2, 4, 6]);
    }

    #[test]
    fn run_pipeline_empty_inputs() {
        let req = Request { inputs: vec![] };
        let resp = run_pipeline(&req);
        assert_eq!(resp.outputs, vec![]);
    }

    #[test]
    fn run_pipeline_single_value() {
        let req = Request { inputs: vec![21] };
        let resp = run_pipeline(&req);
        assert_eq!(resp.outputs, vec![42]);
    }

    #[test]
    fn gateway_transform_wraps_core() {
        // gateway::transform is a thin wrapper — delegates to core.
        assert_eq!(gateway::transform(5), 10);
        assert_eq!(gateway::transform(0), 0);
    }
}
