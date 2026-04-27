//! # Module 5 — Traits & Generics
//!
//! **TDD Red Phase** — all non-trivial tests fail. Implement each stub.
//!
//! | Group | Types / Functions | Concept |
//! |-------|-------------------|---------|
//! | Trait definitions | `Area`, `Perimeter` | trait body, default methods |
//! | Trait impls | `Circle`, `Rectangle` | `impl Trait for Type` |
//! | Generic functions | `largest`, `total_area` | `T: Bound`, `impl Trait` in args |
//! | derive macros | `#[derive(Debug, Clone, PartialEq)]` | automatic trait impls |

// ------------------------------------------------------------------ //
//  1. Trait definitions                                               //
// ------------------------------------------------------------------ //

/// A shape that has an area.
pub trait Area {
    /// Return the area of this shape.
    fn area(&self) -> f64;
}

/// A shape that has a perimeter.
pub trait Perimeter {
    /// Return the perimeter of this shape.
    fn perimeter(&self) -> f64;
}

/// A shape that can produce a human-readable description.
/// Has a **default method** — types can override it.
pub trait Describe: Area + Perimeter {
    /// Return a string like `"Circle: area=3.14, perimeter=6.28"`.
    /// Default implementation built from `area()` and `perimeter()`.
    fn describe(&self) -> String {
        format!("area={:.4}, perimeter={:.4}", self.area(), self.perimeter())
    }
}

// ------------------------------------------------------------------ //
//  2. Concrete types with trait implementations                       //
// ------------------------------------------------------------------ //

/// A circle defined by its radius.
#[derive(Debug, Clone)]
pub struct Circle {
    pub radius: f64,
}

impl Circle {
    pub fn new(radius: f64) -> Self {
        Circle { radius }
    }
}

/// Implement `Area` for `Circle`.
///
/// Formula: π * r²
/// Hint: use `std::f64::consts::PI`.
///
/// TODO: replace `todo!()` with the formula.
impl Area for Circle {
    fn area(&self) -> f64 {
        todo!()
    }
}

/// Implement `Perimeter` for `Circle`.
///
/// Formula: 2 * π * r
///
/// TODO: replace `todo!()` with the formula.
impl Perimeter for Circle {
    fn perimeter(&self) -> f64 {
        todo!()
    }
}

/// `Circle` gets the default `describe` implementation for free.
impl Describe for Circle {}

/// A rectangle defined by its width and height.
#[derive(Debug, Clone)]
pub struct Rectangle {
    pub width: f64,
    pub height: f64,
}

impl Rectangle {
    pub fn new(width: f64, height: f64) -> Self {
        Rectangle { width, height }
    }
}

/// Implement `Area` for `Rectangle`.
///
/// Formula: width * height
///
/// TODO: replace `todo!()`.
impl Area for Rectangle {
    fn area(&self) -> f64 {
        todo!()
    }
}

/// Implement `Perimeter` for `Rectangle`.
///
/// Formula: 2 * (width + height)
///
/// TODO: replace `todo!()`.
impl Perimeter for Rectangle {
    fn perimeter(&self) -> f64 {
        todo!()
    }
}

impl Describe for Rectangle {}

// ------------------------------------------------------------------ //
//  3. Generic functions                                               //
// ------------------------------------------------------------------ //

/// Return a reference to the largest element in `list`.
/// Returns `None` if `list` is empty.
///
/// Bound: `T: PartialOrd` — needed to compare elements.
///
/// TODO: iterate, track the running maximum reference, return it.
pub fn largest<T: PartialOrd>(list: &[T]) -> Option<&T> {
    todo!()
}

/// Sum the areas of all shapes in `shapes`.
///
/// `impl Area` in the argument is sugar for `T: Area`.
/// This works for *any* type that implements `Area`.
///
/// TODO: `shapes.iter().map(|s| s.area()).sum()`.
pub fn total_area(shapes: &[impl Area]) -> f64 {
    todo!()
}

/// Return the shapes whose area is strictly greater than `min_area`.
///
/// TODO: filter and collect into a `Vec`.
/// Note: you cannot return `impl Trait` from within a function body easily,
/// so collect into `Vec<&T>` where `T: Area`.
pub fn filter_by_area<T: Area>(shapes: &[T], min_area: f64) -> Vec<&T> {
    todo!()
}

// ------------------------------------------------------------------ //
//  Tests                                                              //
// ------------------------------------------------------------------ //

#[cfg(test)]
mod tests {
    use super::*;
    use std::f64::consts::PI;

    // --- Circle ---

    #[test]
    fn circle_area() {
        let c = Circle::new(2.0);
        assert!((c.area() - PI * 4.0).abs() < 1e-9);
    }

    #[test]
    fn circle_perimeter() {
        let c = Circle::new(3.0);
        assert!((c.perimeter() - 2.0 * PI * 3.0).abs() < 1e-9);
    }

    #[test]
    fn circle_describe_contains_area_and_perimeter() {
        let c = Circle::new(1.0);
        let d = c.describe();
        assert!(d.contains("area="));
        assert!(d.contains("perimeter="));
    }

    // --- Rectangle ---

    #[test]
    fn rectangle_area() {
        let r = Rectangle::new(4.0, 5.0);
        assert!((r.area() - 20.0).abs() < 1e-9);
    }

    #[test]
    fn rectangle_perimeter() {
        let r = Rectangle::new(3.0, 7.0);
        assert!((r.perimeter() - 20.0).abs() < 1e-9);
    }

    #[test]
    fn rectangle_unit_square_area_and_perimeter() {
        let r = Rectangle::new(1.0, 1.0);
        assert!((r.area() - 1.0).abs() < 1e-9);
        assert!((r.perimeter() - 4.0).abs() < 1e-9);
    }

    // --- largest ---

    #[test]
    fn largest_integers() {
        assert_eq!(largest(&[3, 1, 4, 1, 5, 9, 2, 6]), Some(&9));
    }

    #[test]
    fn largest_single_element() {
        assert_eq!(largest(&[42_i32]), Some(&42));
    }

    #[test]
    fn largest_empty() {
        let empty: &[i32] = &[];
        assert_eq!(largest(empty), None);
    }

    #[test]
    fn largest_strings() {
        assert_eq!(largest(&["apple", "banana", "cherry"]), Some(&"cherry"));
    }

    // --- total_area ---

    #[test]
    fn total_area_circles() {
        let shapes = vec![Circle::new(1.0), Circle::new(2.0)];
        let expected = PI * 1.0 + PI * 4.0;
        assert!((total_area(&shapes) - expected).abs() < 1e-9);
    }

    #[test]
    fn total_area_rectangles() {
        let shapes = vec![Rectangle::new(2.0, 3.0), Rectangle::new(4.0, 5.0)];
        assert!((total_area(&shapes) - 26.0).abs() < 1e-9);
    }

    #[test]
    fn total_area_empty() {
        let shapes: &[Circle] = &[];
        assert_eq!(total_area(shapes), 0.0);
    }

    // --- filter_by_area ---

    #[test]
    fn filter_by_area_returns_matching_shapes() {
        let shapes = vec![Rectangle::new(1.0, 1.0), Rectangle::new(5.0, 5.0), Rectangle::new(2.0, 2.0)];
        let big = filter_by_area(&shapes, 4.0);
        assert_eq!(big.len(), 2); // 25 and 4 but not 1
    }

    #[test]
    fn filter_by_area_none_match() {
        let shapes = vec![Rectangle::new(1.0, 1.0)];
        assert_eq!(filter_by_area(&shapes, 100.0).len(), 0);
    }
}
