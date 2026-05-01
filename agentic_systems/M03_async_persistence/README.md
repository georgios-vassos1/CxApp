# M03 — Async Persistence

**TDD Red Phase** — all tests fail. Implement each stub.

## Learning Objectives

- Model checkpoints as immutable snapshots with versioning
- Implement an in-memory checkpointer for fast test feedback
- Build an async SQLite-backed checkpointer using aiosqlite
- Use a factory function to select backends by configuration
- Defer construction with a lazy wrapper to avoid import-time event-loop issues

## Run

```bash
poetry run pytest M03_async_persistence/ -v
```
