# M15 -- Graph Factory & State Optimisation

## Learning Objectives

1. Implement the graph factory pattern: `build_graph()` returns an uncompiled `StateGraph`
2. Understand why the caller -- not the factory -- owns the `.compile(checkpointer=...)` call
3. Use a module-level cache to avoid redundant graph construction
4. Wire a checkpointer at compile time and invoke the graph with proper config

## Key Concepts

- **Graph factory**: A function that builds and returns an uncompiled `StateGraph`, decoupling graph definition from compilation
- **Caller-owned compile**: The caller attaches checkpointer, interrupt points, and other options at compile time
- **Module-level cache**: A `_cached_graph` variable avoids rebuilding the graph definition on every request
- **compile_with_checkpointer**: A helper that standardises the compile call with optional checkpointer
- **run_graph**: An async runner that invokes a compiled graph with `thread_id` and `recursion_limit` config

## TDD Workflow

All stubs in `src/graph_factory.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M15_graph_factory/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
