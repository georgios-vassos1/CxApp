//! # Module 9 — Advanced Error-Handling Patterns
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! This module is a direct exercise in RUSTPATTERNS.md §2:
//!
//! | Pattern | Function | RUSTPATTERNS.md reference |
//! |---------|----------|--------------------------|
//! | `let-else` early guard | `extract_port` | "`let Some(target) = ... else { return Ok(()); }`" |
//! | `if-let-err` early return | `validate_and_run` | "`if let Err(e) = req.validate() { return Response::validation_errors(...); }`" |
//! | Guard-then-unwrap | `guarded_computation` | "`if bunker_res.is_err() { return ...; } let bunker = bunker_res.unwrap();`" |
//! | `Option<T>` as three-state | `is_opted_in` | "`None` means data absent, `Some(true/false)` means condition holds/doesn't" |
//! | Response constructor pattern | `Response` | "`impl Response { pub fn failure(...); pub fn validation_errors(...); }`" |

// ------------------------------------------------------------------ //
//  1. let-else for early guards on Option                             //
// ------------------------------------------------------------------ //

/// Extract a port number from an optional config string like `"host:8080"`.
///
/// - If `config` is `None`, return the `default_port`.
/// - If `config` is `Some(s)`, split on `':'` and parse the second part.
/// - If the string has no `':'` or the second part is not a valid u16,
///   return `default_port`.
///
/// Use the **`let-else`** pattern for the early guard:
/// ```ignore
/// let Some(cfg) = config else { return default_port; };
/// let Some(port_str) = cfg.split(':').nth(1) else { return default_port; };
/// ```
pub fn extract_port(config: Option<&str>, default_port: u16) -> u16 {
    todo!()
}

// ------------------------------------------------------------------ //
//  2. if-let-err early return                                         //
// ------------------------------------------------------------------ //

/// Validate `input` then process it.
///
/// Validation rules:
/// - Must not be empty → return `Err("input is empty".to_string())`.
/// - Must not contain spaces → return `Err("input contains spaces".to_string())`.
///
/// If valid, return `Ok(input.to_uppercase())`.
///
/// Use the **`if-let-err`** pattern:
/// ```ignore
/// if let Err(e) = validate(input) { return Err(e); }
/// ```
pub fn validate_and_run(input: &str) -> Result<String, String> {
    todo!()
}

/// Internal validation — returns `Err(message)` on the first violated rule.
///
/// TODO: check empty, then check for spaces.
fn validate(s: &str) -> Result<(), String> {
    todo!()
}

// ------------------------------------------------------------------ //
//  3. Guard-then-unwrap                                               //
// ------------------------------------------------------------------ //

/// Compute a value that depends on `data`.
///
/// Steps:
/// 1. Call `fetch(data)` — returns `Result<f64, String>`.
/// 2. If it errors, return `Err(e)` immediately (guard).
/// 3. Unwrap the result (safe because we just checked) and square it.
/// 4. Return `Ok(squared)`.
///
/// This is the **guard-then-unwrap** pattern from RUSTPATTERNS.md §2.
/// It is appropriate when the error path requires constructing a domain object,
/// but here we keep it simple to practise the pattern.
///
/// TODO:
/// ```ignore
/// let result = fetch(data);
/// if result.is_err() { return Err(result.unwrap_err()); }
/// let value = result.unwrap(); // safe: checked above
/// ```
pub fn guarded_computation(data: &str) -> Result<f64, String> {
    todo!()
}

/// Simulated "fetch" step — parses `s` as f64.
fn fetch(s: &str) -> Result<f64, String> {
    s.trim().parse::<f64>().map_err(|_| format!("cannot parse '{}' as f64", s))
}

// ------------------------------------------------------------------ //
//  4. Option<T> as three-state boolean                               //
// ------------------------------------------------------------------ //

/// Interpret an `Option<bool>` as a three-state flag.
///
/// - `None`        → data absent — treat as *not* opted in (return `false`).
/// - `Some(true)`  → explicitly opted in (return `true`).
/// - `Some(false)` → explicitly opted out (return `false`).
///
/// RUSTPATTERNS.md §2:
/// > `Option<T>` as a three-state boolean — `None` means "data does not exist",
/// > callers use `.unwrap_or(false)` to treat absence as the negative case.
///
/// TODO: return `flag.unwrap_or(false)`.
pub fn is_opted_in(flag: Option<bool>) -> bool {
    todo!()
}

/// Return the speed, or 0.0 if the speed data is absent.
///
/// Mirrors `max_speed_kn: Option<f64>` in RUSTPATTERNS.md §3.
///
/// TODO: return `speed.unwrap_or(0.0)`.
pub fn speed_or_zero(speed: Option<f64>) -> f64 {
    todo!()
}

// ------------------------------------------------------------------ //
//  5. Response constructor pattern                                    //
// ------------------------------------------------------------------ //

/// A simple response type whose callers never construct it directly.
/// All construction goes through named constructors on `impl Response`.
///
/// RUSTPATTERNS.md §2:
/// > Callers never construct `Response` directly for error cases — they always
/// > go through these named constructors.
#[derive(Debug, PartialEq)]
pub struct Response {
    pub success: bool,
    pub message: String,
    pub value: Option<f64>,
}

impl Response {
    /// Successful response carrying a computed value.
    ///
    /// TODO: `Response { success: true, message: "ok".to_string(), value: Some(v) }`.
    pub fn success(v: f64) -> Self {
        todo!()
    }

    /// Generic failure response with an error message.
    ///
    /// TODO: `Response { success: false, message: msg.to_string(), value: None }`.
    pub fn failure(msg: &str) -> Self {
        todo!()
    }

    /// Validation error response — includes the failing field name and reason.
    ///
    /// TODO: `Response { success: false, message: format!("{field}: {reason}"), value: None }`.
    pub fn validation_error(field: &str, reason: &str) -> Self {
        todo!()
    }
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- extract_port ---

    #[test]
    fn extract_port_some_valid() {
        assert_eq!(extract_port(Some("localhost:9090"), 80), 9090);
    }

    #[test]
    fn extract_port_none_returns_default() {
        assert_eq!(extract_port(None, 8080), 8080);
    }

    #[test]
    fn extract_port_no_colon_returns_default() {
        assert_eq!(extract_port(Some("localhost"), 80), 80);
    }

    #[test]
    fn extract_port_invalid_number_returns_default() {
        assert_eq!(extract_port(Some("host:abc"), 80), 80);
    }

    // --- validate_and_run ---

    #[test]
    fn validate_and_run_valid_input() {
        assert_eq!(validate_and_run("hello"), Ok("HELLO".to_string()));
    }

    #[test]
    fn validate_and_run_empty_input() {
        assert!(validate_and_run("").is_err());
    }

    #[test]
    fn validate_and_run_input_with_spaces() {
        assert!(validate_and_run("hello world").is_err());
    }

    // --- guarded_computation ---

    #[test]
    fn guarded_computation_valid() {
        let r = guarded_computation("3.0").unwrap();
        assert!((r - 9.0).abs() < 1e-9);
    }

    #[test]
    fn guarded_computation_invalid() {
        assert!(guarded_computation("not_a_number").is_err());
    }

    // --- is_opted_in ---

    #[test]
    fn is_opted_in_none_is_false() {
        assert!(!is_opted_in(None));
    }

    #[test]
    fn is_opted_in_some_true() {
        assert!(is_opted_in(Some(true)));
    }

    #[test]
    fn is_opted_in_some_false() {
        assert!(!is_opted_in(Some(false)));
    }

    // --- speed_or_zero ---

    #[test]
    fn speed_or_zero_some() {
        assert!((speed_or_zero(Some(12.5)) - 12.5).abs() < 1e-9);
    }

    #[test]
    fn speed_or_zero_none() {
        assert_eq!(speed_or_zero(None), 0.0);
    }

    // --- Response constructors ---

    #[test]
    fn response_success_constructor() {
        let r = Response::success(42.0);
        assert!(r.success);
        assert_eq!(r.message, "ok");
        assert!((r.value.unwrap() - 42.0).abs() < 1e-9);
    }

    #[test]
    fn response_failure_constructor() {
        let r = Response::failure("something went wrong");
        assert!(!r.success);
        assert_eq!(r.message, "something went wrong");
        assert!(r.value.is_none());
    }

    #[test]
    fn response_validation_error_constructor() {
        let r = Response::validation_error("speed", "must be positive");
        assert!(!r.success);
        assert!(r.message.contains("speed"));
        assert!(r.message.contains("must be positive"));
    }
}
