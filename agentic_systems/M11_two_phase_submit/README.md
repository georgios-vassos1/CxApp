# M11 -- Two-Phase Submit & Callbacks

## Learning Objectives

- Implement the Streamlit two-phase submit pattern (capture then execute)
- Manage chat history with typed dataclasses (ChatMessage, ChatSession)
- Use callbacks to decouple UI state management from agent invocation
- Track processing flags and pending state for async-safe reruns

## Key Concepts

| Concept | Description |
|---------|-------------|
| Two-phase submit | Phase 1: capture input + set flags; Phase 2: call agent + record response |
| Chat session | Tracks message history and context_id per run_id |
| Processing guard | Boolean flag prevents duplicate submissions during agent calls |
| Toggle pattern | Simple boolean flip for panel visibility or feature flags |

## Files

| File | Purpose |
|------|---------|
| `src/two_phase.py` | Stub implementations (raise NotImplementedError) |
| `tests/test_two_phase.py` | 14 tests covering both phases, toggle, and guard |

## Getting Started

```bash
# Run tests (all should fail initially)
poetry run pytest M11_two_phase_submit/tests/ -v
```

Implement each stub in `src/two_phase.py` until all tests pass.
