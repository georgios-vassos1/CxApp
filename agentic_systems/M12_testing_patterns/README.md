# M12 -- Testing Patterns

## Learning Objectives

- Build reusable test helper factories for session state dicts
- Use polyfactory ModelFactory for generating Pydantic model instances
- Create context managers for patching Streamlit session state in tests
- Write readable assertion helpers that improve test clarity

## Key Concepts

| Concept | Description |
|---------|-------------|
| Session factory | Consistent defaults with override support for test isolation |
| Model factory | polyfactory generates valid Pydantic instances with custom constraints |
| Patch helper | Context manager swaps session_state for controlled testing |
| Assertion helpers | Domain-specific assertions that read like specifications |

## Files

| File | Purpose |
|------|---------|
| `src/test_helpers.py` | Stub implementations (raise NotImplementedError) |
| `tests/test_test_helpers.py` | 10 tests covering factories, assertions, and patching |

## Getting Started

```bash
# Run tests (all should fail initially)
poetry run pytest M12_testing_patterns/tests/ -v
```

Implement each stub in `src/test_helpers.py` until all tests pass.
