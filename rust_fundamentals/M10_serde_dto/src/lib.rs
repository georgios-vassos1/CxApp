//! # Module 10 — Serde & DTO Patterns
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! This module exercises RUSTPATTERNS.md §4:
//!
//! | Pattern | Where | RUSTPATTERNS.md reference |
//! |---------|-------|--------------------------|
//! | `#[serde(alias)]` | `CreateJobRequest` | "`#[serde(alias = \"prefered_berthing_windows\")]` — typo in the JSON contract" |
//! | `#[serde(default = "fn")]` | `CreateJobRequest` | "Named default functions preferred over `Default::default()`" |
//! | `#[serde(default)]` | `CreateJobRequest` | "`in_scope: bool`" |
//! | Strict request/response split | `CreateJobRequest` / `JobResponse` | "No type crosses the boundary" |
//! | Validation on request only | `validate()` | "`#[derive(Validate)]` appears on Request types only" |

use serde::{Deserialize, Serialize};

// ------------------------------------------------------------------ //
//  1. Request DTO                                                     //
// ------------------------------------------------------------------ //

/// Inbound request to create a processing job.
///
/// Serde notes:
/// - `priority` accepts the alias `"priority_level"` for backward compatibility.
/// - `timeout_seconds` defaults to `default_timeout()` if absent from JSON.
/// - `enabled` defaults to `false` (the type-level default for `bool`) if absent.
/// - `tags` defaults to an empty `Vec` if absent.
#[derive(Debug, Serialize, Deserialize, PartialEq)]
pub struct CreateJobRequest {
    pub name: String,

    #[serde(alias = "priority_level")]
    pub priority: u8,

    #[serde(default = "default_timeout")]
    pub timeout_seconds: u32,

    #[serde(default)]
    pub enabled: bool,

    #[serde(default)]
    pub tags: Vec<String>,
}

fn default_timeout() -> u32 { 30 }

/// Validate a `CreateJobRequest`.
///
/// Rules:
/// - `name` must not be empty.
/// - `priority` must be in the range `[1, 10]`.
/// - `timeout_seconds` must be > 0.
///
/// Return `Ok(())` if valid, or `Err(message)` describing the first violation.
///
/// TODO: check each rule in order and return early on the first violation.
pub fn validate(req: &CreateJobRequest) -> Result<(), String> {
    todo!()
}

// ------------------------------------------------------------------ //
//  2. Response DTO                                                    //
// ------------------------------------------------------------------ //

/// Outbound response — no type crosses the request/response boundary.
/// `JobResponse` knows nothing about `CreateJobRequest`.
///
/// RUSTPATTERNS.md §4:
/// > "Strict request/response split. No type crosses the boundary in either direction."
#[derive(Debug, Serialize, Deserialize, PartialEq)]
pub struct JobResponse {
    pub id: u64,
    pub name: String,
    pub priority: u8,
    pub status: String,
}

// ------------------------------------------------------------------ //
//  3. Conversion & serialization                                      //
// ------------------------------------------------------------------ //

/// Convert a validated request into a response, assigning it the given `id`.
/// Status is always `"queued"` for a freshly created job.
///
/// TODO: build a `JobResponse` from the request fields plus `id`.
pub fn request_to_response(req: &CreateJobRequest, id: u64) -> JobResponse {
    todo!()
}

/// Serialize a `JobResponse` to a JSON string.
///
/// TODO: call `serde_json::to_string(resp).map_err(|e| e.to_string())`.
pub fn serialize_response(resp: &JobResponse) -> Result<String, String> {
    todo!()
}

/// Deserialize a `CreateJobRequest` from a JSON string.
///
/// TODO: call `serde_json::from_str(json).map_err(|e| e.to_string())`.
pub fn deserialize_request(json: &str) -> Result<CreateJobRequest, String> {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    fn sample_request() -> CreateJobRequest {
        CreateJobRequest {
            name: "indexer".to_string(),
            priority: 5,
            timeout_seconds: 60,
            enabled: true,
            tags: vec!["prod".to_string()],
        }
    }

    // --- serde(alias) ---

    #[test]
    fn deserialize_accepts_priority_level_alias() {
        let json = r#"{"name":"job","priority_level":3}"#;
        let req: CreateJobRequest = serde_json::from_str(json).unwrap();
        assert_eq!(req.priority, 3);
    }

    #[test]
    fn deserialize_accepts_canonical_priority() {
        let json = r#"{"name":"job","priority":7}"#;
        let req: CreateJobRequest = serde_json::from_str(json).unwrap();
        assert_eq!(req.priority, 7);
    }

    // --- serde(default) ---

    #[test]
    fn deserialize_default_timeout_when_absent() {
        let json = r#"{"name":"job","priority":1}"#;
        let req: CreateJobRequest = serde_json::from_str(json).unwrap();
        assert_eq!(req.timeout_seconds, 30); // default_timeout()
    }

    #[test]
    fn deserialize_default_enabled_is_false() {
        let json = r#"{"name":"job","priority":1}"#;
        let req: CreateJobRequest = serde_json::from_str(json).unwrap();
        assert!(!req.enabled);
    }

    #[test]
    fn deserialize_default_tags_is_empty() {
        let json = r#"{"name":"job","priority":1}"#;
        let req: CreateJobRequest = serde_json::from_str(json).unwrap();
        assert!(req.tags.is_empty());
    }

    // --- validate ---

    #[test]
    fn validate_valid_request() {
        assert!(validate(&sample_request()).is_ok());
    }

    #[test]
    fn validate_empty_name_fails() {
        let req = CreateJobRequest { name: "".to_string(), ..sample_request() };
        assert!(validate(&req).is_err());
    }

    #[test]
    fn validate_priority_zero_fails() {
        let req = CreateJobRequest { priority: 0, ..sample_request() };
        assert!(validate(&req).is_err());
    }

    #[test]
    fn validate_priority_eleven_fails() {
        let req = CreateJobRequest { priority: 11, ..sample_request() };
        assert!(validate(&req).is_err());
    }

    #[test]
    fn validate_zero_timeout_fails() {
        let req = CreateJobRequest { timeout_seconds: 0, ..sample_request() };
        assert!(validate(&req).is_err());
    }

    // --- request_to_response ---

    #[test]
    fn request_to_response_sets_id_and_status() {
        let resp = request_to_response(&sample_request(), 42);
        assert_eq!(resp.id, 42);
        assert_eq!(resp.status, "queued");
        assert_eq!(resp.name, "indexer");
        assert_eq!(resp.priority, 5);
    }

    // --- serialize / deserialize round-trip ---

    #[test]
    fn round_trip_serialize_deserialize() {
        let original = sample_request();
        let json = serde_json::to_string(&original).unwrap();
        let recovered: CreateJobRequest = serde_json::from_str(&json).unwrap();
        assert_eq!(original, recovered);
    }

    #[test]
    fn serialize_response_produces_valid_json() {
        let resp = JobResponse { id: 1, name: "job".to_string(), priority: 5, status: "queued".to_string() };
        let json = serialize_response(&resp).unwrap();
        assert!(json.contains("\"id\":1"));
        assert!(json.contains("\"status\":\"queued\""));
    }

    #[test]
    fn deserialize_request_from_json() {
        let json = r#"{"name":"worker","priority":2,"timeout_seconds":10}"#;
        let req = deserialize_request(json).unwrap();
        assert_eq!(req.name, "worker");
        assert_eq!(req.priority, 2);
        assert_eq!(req.timeout_seconds, 10);
    }

    #[test]
    fn deserialize_request_invalid_json_returns_err() {
        assert!(deserialize_request("{not valid json}").is_err());
    }
}
