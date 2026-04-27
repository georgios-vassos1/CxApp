//! # Module 4 — Error Handling Basics
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! | Group | Types / Functions | Concept |
//! |-------|-------------------|---------|
//! | Custom error type | `ParseError` | enums as errors, `#[derive(Debug)]` |
//! | Result with `?`   | `parse_positive`, `double_positive` | propagation via `?`, `map_err` |
//! | Collecting Results | `parse_all` | `Iterator::collect::<Result<Vec<_>, _>>()` |
//! | Chained validation | `validated_sqrt` | multiple failure modes in one pipeline |

// ------------------------------------------------------------------ //
//  1. Custom error type                                               //
// ------------------------------------------------------------------ //

/// The error variants that can occur during number parsing.
#[derive(Debug, PartialEq)]
pub enum ParseError {
    /// The input string was empty.
    Empty,
    /// The string contained a character that is not a decimal digit.
    /// Carries the offending character.
    InvalidChar(char),
    /// The parsed value exceeded `u32::MAX`.
    Overflow,
    /// The value was zero, but a strictly positive number was required.
    Zero,
}

// ------------------------------------------------------------------ //
//  2. Parsing with Result and ?                                       //
// ------------------------------------------------------------------ //

/// Parse a strictly positive (> 0) decimal integer from `s`.
///
/// Errors:
/// - `ParseError::Empty`      — `s` is empty or whitespace-only.
/// - `ParseError::InvalidChar(c)` — `s` contains a non-digit character `c`.
/// - `ParseError::Overflow`   — the value exceeds `u32::MAX`.
/// - `ParseError::Zero`       — the value parses to 0.
///
/// TODO:
/// 1. Trim `s`; if empty return `Err(ParseError::Empty)`.
/// 2. For each character, check `c.is_ascii_digit()`; if not return `Err(ParseError::InvalidChar(c))`.
/// 3. Use `s.parse::<u32>().map_err(|_| ParseError::Overflow)` with `?`.
/// 4. If the result is 0, return `Err(ParseError::Zero)`.
/// 5. Return `Ok(n)`.
pub fn parse_positive(s: &str) -> Result<u32, ParseError> {
    todo!()
}

/// Parse `s` and double the result.
///
/// TODO: call `parse_positive(s)?` and multiply by 2.
/// Use the `?` operator so any error is forwarded automatically.
pub fn double_positive(s: &str) -> Result<u32, ParseError> {
    todo!()
}

// ------------------------------------------------------------------ //
//  3. Collecting Results                                              //
// ------------------------------------------------------------------ //

/// Parse every string in `inputs` as a positive integer.
/// Returns `Ok(Vec<u32>)` if all succeed, or the first `Err` encountered.
///
/// TODO: use the iterator `collect::<Result<Vec<_>, _>>()` pattern:
/// ```ignore
/// inputs.iter()
///     .map(|s| parse_positive(s))
///     .collect()
/// ```
pub fn parse_all(inputs: &[&str]) -> Result<Vec<u32>, ParseError> {
    todo!()
}

// ------------------------------------------------------------------ //
//  4. Chained validation                                              //
// ------------------------------------------------------------------ //

/// Compute the square root of `x`, subject to validation.
///
/// Errors:
/// - `ParseError::Zero` — `x` is exactly 0.0.
/// - `ParseError::InvalidChar('n')` — `x` is negative (borrowing the variant to signal "domain error").
///
/// TODO:
/// 1. If `x == 0.0` return `Err(ParseError::Zero)`.
/// 2. If `x < 0.0`  return `Err(ParseError::InvalidChar('n'))`.
/// 3. Return `Ok(x.sqrt())`.
pub fn validated_sqrt(x: f64) -> Result<f64, ParseError> {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;

    // --- parse_positive ---

    #[test]
    fn parse_positive_valid() {
        assert_eq!(parse_positive("42"), Ok(42));
    }

    #[test]
    fn parse_positive_one() {
        assert_eq!(parse_positive("1"), Ok(1));
    }

    #[test]
    fn parse_positive_empty_string() {
        assert_eq!(parse_positive(""), Err(ParseError::Empty));
    }

    #[test]
    fn parse_positive_whitespace_only() {
        assert_eq!(parse_positive("   "), Err(ParseError::Empty));
    }

    #[test]
    fn parse_positive_invalid_char() {
        assert_eq!(parse_positive("12a3"), Err(ParseError::InvalidChar('a')));
    }

    #[test]
    fn parse_positive_zero_is_error() {
        assert_eq!(parse_positive("0"), Err(ParseError::Zero));
    }

    #[test]
    fn parse_positive_overflow() {
        // 2^32 = 4294967296 exceeds u32::MAX
        assert_eq!(parse_positive("4294967296"), Err(ParseError::Overflow));
    }

    // --- double_positive ---

    #[test]
    fn double_positive_valid() {
        assert_eq!(double_positive("5"), Ok(10));
    }

    #[test]
    fn double_positive_propagates_error() {
        assert_eq!(double_positive(""), Err(ParseError::Empty));
        assert_eq!(double_positive("abc"), Err(ParseError::InvalidChar('a')));
    }

    // --- parse_all ---

    #[test]
    fn parse_all_all_valid() {
        assert_eq!(parse_all(&["1", "2", "3"]), Ok(vec![1, 2, 3]));
    }

    #[test]
    fn parse_all_empty_slice() {
        assert_eq!(parse_all(&[]), Ok(vec![]));
    }

    #[test]
    fn parse_all_first_error_returned() {
        assert_eq!(parse_all(&["1", "", "3"]), Err(ParseError::Empty));
    }

    #[test]
    fn parse_all_error_in_middle() {
        assert_eq!(parse_all(&["1", "bad", "3"]), Err(ParseError::InvalidChar('b')));
    }

    // --- validated_sqrt ---

    #[test]
    fn validated_sqrt_positive() {
        let r = validated_sqrt(9.0).unwrap();
        assert!((r - 3.0).abs() < 1e-9);
    }

    #[test]
    fn validated_sqrt_zero_is_error() {
        assert_eq!(validated_sqrt(0.0), Err(ParseError::Zero));
    }

    #[test]
    fn validated_sqrt_negative_is_error() {
        assert_eq!(validated_sqrt(-1.0), Err(ParseError::InvalidChar('n')));
    }
}
