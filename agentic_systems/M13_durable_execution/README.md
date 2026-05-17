# M13 -- Durable Execution Fundamentals

## Learning Objectives

1. Understand the journal as an append-only log of side-effects
2. Implement replay semantics that return stored results without re-executing
3. Enforce a determinism contract that rejects divergent call sequences
4. Model a VirtualObject with keyed state and serial execution

## Key Concepts

- **JournalEntry**: A recorded side-effect with an index, action name, and stored result
- **Journal**: An append-only log that enables replay by looking up entries by index
- **DurableRuntime**: Executes side-effects on first run and replays them on subsequent runs, enforcing determinism
- **JournalMismatchError**: Raised when a replayed action name does not match the journal, signalling non-determinism
- **VirtualObject**: A keyed stateful entity (like a Restate Virtual Object) with isolated state and journal

## TDD Workflow

All stubs in `src/durable.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M13_durable_execution/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
