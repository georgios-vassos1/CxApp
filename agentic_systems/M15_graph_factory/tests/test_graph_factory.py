"""Tests for Module 15 -- Graph Factory & State Optimisation.

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

from langgraph.graph import StateGraph

from graph_factory import (
    GraphState,
    build_graph,
    compile_with_checkpointer,
    farewell_node,
    get_or_build_graph,
    greet_node,
    run_graph,
)


# --- GraphState ---


def test_state_has_greeting_key():
    assert "greeting" in GraphState.__annotations__


def test_state_has_steps_key():
    assert "steps" in GraphState.__annotations__


# --- greet_node ---


def test_greet_node_produces_greeting():
    state: GraphState = {"greeting": "Alice", "steps": []}
    result = greet_node(state)
    assert result["greeting"] == "Hello, Alice!"


def test_greet_node_default_name():
    state: GraphState = {"greeting": "", "steps": []}
    result = greet_node(state)
    assert result["greeting"] == "Hello, World!"


def test_greet_node_records_step():
    state: GraphState = {"greeting": "Bob", "steps": []}
    result = greet_node(state)
    assert "greet" in result["steps"]


# --- farewell_node ---


def test_farewell_node_appends_goodbye():
    state: GraphState = {"greeting": "Hello, Alice!", "steps": ["greet"]}
    result = farewell_node(state)
    assert result["greeting"] == "Hello, Alice! Goodbye!"


def test_farewell_node_records_step():
    state: GraphState = {"greeting": "Hi", "steps": ["greet"]}
    result = farewell_node(state)
    assert "farewell" in result["steps"]


# --- build_graph ---


def test_build_graph_returns_state_graph():
    graph = build_graph()
    assert isinstance(graph, StateGraph)


def test_build_graph_is_not_compiled():
    graph = build_graph()
    assert not hasattr(graph, "invoke")


def test_build_graph_can_compile():
    graph = build_graph()
    compiled = graph.compile()
    assert hasattr(compiled, "invoke")


# --- build_graph end-to-end ---


def test_compiled_graph_runs():
    graph = build_graph()
    compiled = graph.compile()
    result = compiled.invoke({"greeting": "Eve", "steps": []})
    assert "Goodbye!" in result["greeting"]
    assert "greet" in result["steps"]
    assert "farewell" in result["steps"]


# --- get_or_build_graph cache ---


def test_get_or_build_returns_state_graph():
    import graph_factory

    graph_factory._cached_graph = None
    graph = get_or_build_graph()
    assert isinstance(graph, StateGraph)


def test_get_or_build_caches():
    import graph_factory

    graph_factory._cached_graph = None
    first = get_or_build_graph()
    second = get_or_build_graph()
    assert first is second


def test_get_or_build_reuses_existing():
    import graph_factory

    sentinel = StateGraph(GraphState)
    graph_factory._cached_graph = sentinel
    result = get_or_build_graph()
    assert result is sentinel
    graph_factory._cached_graph = None


# --- compile_with_checkpointer ---


def test_compile_without_checkpointer():
    graph = build_graph()
    compiled = compile_with_checkpointer(graph)
    assert hasattr(compiled, "invoke")


def test_compile_with_checkpointer_accepted():
    from langgraph.checkpoint.memory import MemorySaver

    graph = build_graph()
    compiled = compile_with_checkpointer(graph, checkpointer=MemorySaver())
    assert hasattr(compiled, "invoke")


def test_compile_result_runs_correctly():
    graph = build_graph()
    compiled = compile_with_checkpointer(graph)
    result = compiled.invoke({"greeting": "Test", "steps": []})
    assert result["greeting"] == "Hello, Test! Goodbye!"


# --- run_graph ---


async def test_run_graph_returns_result():
    graph = build_graph()
    compiled = compile_with_checkpointer(graph)
    result = await run_graph(compiled, {"greeting": "Async", "steps": []})
    assert "Goodbye!" in result["greeting"]


async def test_run_graph_with_thread_id():
    from langgraph.checkpoint.memory import MemorySaver

    graph = build_graph()
    compiled = compile_with_checkpointer(graph, checkpointer=MemorySaver())
    result = await run_graph(
        compiled,
        {"greeting": "Thread", "steps": []},
        thread_id="test-thread-42",
    )
    assert result["greeting"] == "Hello, Thread! Goodbye!"
