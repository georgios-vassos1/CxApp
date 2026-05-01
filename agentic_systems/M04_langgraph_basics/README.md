# M04 -- LangGraph Basics

## Learning Objectives

1. Define graph state using `TypedDict` with the `add_messages` reducer
2. Write node functions that transform state and return update dicts
3. Implement conditional routing logic to branch graph execution
4. Build and compile simple linear graphs (START -> node -> END)
5. Build graphs with conditional edges based on message content

## Key Concepts

- **MessageState**: A `TypedDict` with an `Annotated[list, add_messages]` field that LangGraph uses to accumulate messages
- **Node functions**: Pure functions that receive state and return a partial update dict
- **Conditional edges**: Functions that inspect state and return the name of the next node (or `END`)
- **StateGraph**: The builder API for wiring nodes and edges into a compiled, invocable graph

## TDD Workflow

All stubs in `src/graph_basics.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M04_langgraph_basics/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
