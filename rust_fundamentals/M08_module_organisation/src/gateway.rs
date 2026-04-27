//! `gateway.rs` — thin re-export / public API gateway.
//!
//! This file's only job:
//! - Declare the **public API types** (`Request`, `Response`).
//! - Expose a `transform` function that wraps core logic without exposing `core` internals.
//!
//! It intentionally knows nothing about the full pipeline — that belongs in `lib.rs`.
//!
//! RUSTPATTERNS.md §1:
//! > `dto/mod.rs` is a thin re-export gateway — it only declares pub mods.

use crate::core;

/// An inbound request carrying a list of integer inputs.
#[derive(Debug, Clone)]
pub struct Request {
    pub inputs: Vec<i32>,
}

/// An outbound response carrying processed outputs.
#[derive(Debug, Clone, PartialEq)]
pub struct Response {
    pub outputs: Vec<i32>,
}

/// A thin public wrapper around `core::compute_step`.
/// Callers use `gateway::transform` without knowing that `core` exists.
///
/// TODO: delegate to `core::compute_step(x)`.
pub fn transform(x: i32) -> i32 {
    todo!()
}
