# M14 -- Custom Checkpointer (Preloaded / Zero-I/O Pattern)

## Learning Objectives

1. Serialise and deserialise checkpoint state using base64-encoded JSON
2. Build a zero-I/O checkpointer that serves preloaded state to LangGraph
3. Capture pending writes for external persistence by the caller
4. Use a factory function to construct a checkpointer from raw bytes or None

## Key Concepts

- **Base64 encoding**: Makes arbitrary checkpoint dicts JSON-safe for storage in journals or databases
- **PreloadedCheckpointer**: A checkpointer initialised with pre-fetched state; performs no I/O during graph execution
- **Pending writes**: Writes captured during graph execution, retrieved afterwards for external persistence
- **Serve-once semantics**: `get_tuple()` returns preloaded state on the first call only, preventing stale reads
- **Factory pattern**: `create_preloaded()` decodes a raw blob and wraps it in the appropriate checkpointer

## TDD Workflow

All stubs in `src/checkpointer.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M14_custom_checkpointer/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
