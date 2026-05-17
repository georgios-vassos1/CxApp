"""Module 15 -- Graph Factory & State Optimisation

**TDD Red Phase** -- all tests fail. Implement each stub.

This module implements the graph factory pattern used in production
agents: ``build_graph()`` returns an *uncompiled* ``StateGraph`` so the
caller can attach their own checkpointer at compile time.  A module-level
cache avoids rebuilding the graph definition on every request.

Also covered: a ``compile_with_checkpointer()`` helper that wires
compile options, and a ``run_graph()`` coroutine that executes a compiled
graph with proper config (thread_id, recursion_limit).

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| State | GraphState (TypedDict) | Minimal typed state for the graph |
| Nodes | greet_node, farewell_node | Pure functions that transform state |
| Factory | build_graph | Returns uncompiled StateGraph (caller owns compile) |
| Cache | get_or_build_graph | Module-level cache avoids redundant graph construction |
| Compile helper | compile_with_checkpointer | Attach checkpointer + config at compile time |
| Runner | run_graph | Invoke a compiled graph with thread_id and recursion_limit |
"""

from __future__ import annotations

from typing import Any, TypedDict

from langgraph.graph import StateGraph


# ------------------------------------------------------------------ #
#  1. Graph State                                                     #
# ------------------------------------------------------------------ #


class GraphState(TypedDict):
    """Minimal graph state.

    TODO: Define two keys:
      - greeting (str) -- a greeting message
      - steps (list[str]) -- log of node names visited
    """

    greeting: str
    steps: list[str]


# ------------------------------------------------------------------ #
#  2. Node Functions                                                  #
# ------------------------------------------------------------------ #


def greet_node(state: GraphState) -> dict[str, Any]:
    """Produce a greeting and record the visit.

    TODO:
      - Set greeting to "Hello, {name}!" where name comes from
        state.get("greeting", "World"). If greeting is empty, use "World".
      - Append "greet" to the steps list.
      - Return a dict with both keys.
    """
    raise NotImplementedError


def farewell_node(state: GraphState) -> dict[str, Any]:
    """Append a farewell to the greeting and record the visit.

    TODO:
      - Set greeting to state["greeting"] + " Goodbye!"
      - Append "farewell" to the steps list.
      - Return a dict with both keys.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. Graph Factory                                                   #
# ------------------------------------------------------------------ #


def build_graph() -> StateGraph:
    """Build and return an *uncompiled* StateGraph.

    TODO:
      1. Create a StateGraph(GraphState).
      2. Add greet_node as "greet" and farewell_node as "farewell".
      3. Set entry point to "greet".
      4. Add edge from "greet" to "farewell".
      5. Set finish point to "farewell".
      6. Return the StateGraph (do NOT call .compile()).
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Module-level Cache                                              #
# ------------------------------------------------------------------ #

_cached_graph: StateGraph | None = None


def get_or_build_graph() -> StateGraph:
    """Return a cached StateGraph, building it on first call.

    TODO:
      - Use the module-level ``_cached_graph`` variable.
      - If it is None, call build_graph() and cache the result.
      - Return the cached graph.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  5. Compile Helper                                                  #
# ------------------------------------------------------------------ #


def compile_with_checkpointer(
    graph: StateGraph,
    checkpointer: Any = None,
) -> Any:
    """Compile a StateGraph with an optional checkpointer.

    TODO:
      - If checkpointer is not None, call graph.compile(checkpointer=checkpointer).
      - Otherwise call graph.compile().
      - Return the compiled graph.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  6. Runner                                                          #
# ------------------------------------------------------------------ #


async def run_graph(
    compiled_graph: Any,
    input_state: dict[str, Any],
    thread_id: str = "default",
    recursion_limit: int = 25,
) -> dict[str, Any]:
    """Invoke a compiled graph with config.

    TODO:
      1. Build a config dict: {"configurable": {"thread_id": thread_id},
         "recursion_limit": recursion_limit}.
      2. Call compiled_graph.ainvoke(input_state, config=config).
      3. Return the result.
    """
    raise NotImplementedError
