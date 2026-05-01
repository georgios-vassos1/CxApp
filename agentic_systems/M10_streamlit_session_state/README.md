# M10 -- Streamlit Session State

## Learning Objectives

- Model Streamlit session state patterns without importing streamlit
- Build scope-keyed caching with automatic invalidation on scope change
- Use frozen dataclasses for hashable, immutable parameter scopes
- Implement recursive data structure conversion for cache key hashing

## Key Concepts

| Concept | Description |
|---------|-------------|
| Session state | Dict-based key-value store persisting across Streamlit reruns |
| Scope-keyed cache | Files are valid only when the scope (region, dates, terminals) matches |
| Cache invalidation | Scope change triggers automatic wipe of stale cached files |
| Hashable conversion | Lists and dicts must be converted to tuples/frozensets for use as keys |

## Files

| File | Purpose |
|------|---------|
| `src/state_manager.py` | Stub implementations (raise NotImplementedError) |
| `tests/test_state_manager.py` | 12 tests covering scope, cache, clearing, and hashing |

## Getting Started

```bash
# Run tests (all should fail initially)
poetry run pytest M10_streamlit_session_state/tests/ -v
```

Implement each stub in `src/state_manager.py` until all tests pass.
