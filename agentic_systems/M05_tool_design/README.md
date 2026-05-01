# M05 -- Tool Design

## Learning Objectives

1. Create simple tools with the `@tool` decorator
2. Read graph state from within a tool using `InjectedState`
3. Mutate graph state from a tool using `Command` returns
4. Understand tool metadata (name, description) and how LangGraph uses it

## Key Concepts

- **@tool decorator**: Wraps a plain function into a LangChain tool with automatic schema generation
- **InjectedState**: An annotation that tells LangGraph to pass the current graph state to the tool
- **InjectedToolCallId**: An annotation for receiving the tool call ID (used in `Command` responses)
- **Command**: A return type that lets tools update graph state beyond their normal return value

## TDD Workflow

All stubs in `src/tools.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M05_tool_design/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
