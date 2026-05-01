# M02 — Multi-turn Sessions

**TDD Red Phase** — all tests fail. Implement each stub.

## Learning Objectives

- Model multi-turn conversations with an in-memory session dataclass
- Implement CRUD operations on a bounded session pool
- Route requests by context_id (create vs. resume vs. reject)
- Apply LRU eviction to enforce capacity constraints

## Run

```bash
poetry run pytest M02_multi_turn_sessions/ -v
```
