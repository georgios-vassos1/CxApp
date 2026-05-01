"""Module 5 -- Tool Design

| Group | Functions | Concept |
|-------|-----------|---------|
| Simple tool | add_numbers | @tool, plain args, plain return |
| State-reading tool | get_loaded_data | InjectedState, read from state dict |
| State-mutating tool | load_dataset | Command return, update state fields |
| Tool node wiring | build_tool_graph | ToolNode, bind_tools, ReAct loop |

**TDD Red Phase** -- all tests fail. Implement each stub.
"""

from __future__ import annotations

from typing import Annotated

from langchain_core.tools import InjectedToolCallId, tool
from langgraph.prebuilt import InjectedState
from langgraph.types import Command


# ------------------------------------------------------------------ #
#  1. Simple Tool                                                     #
# ------------------------------------------------------------------ #


@tool
def add_numbers(a: int, b: int) -> int:
    """Add two integers and return the result.

    TODO: Return ``a + b``.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  2. State-Reading Tool                                              #
# ------------------------------------------------------------------ #


@tool
def get_loaded_data(state: Annotated[dict, InjectedState]) -> str:
    """Return a summary of the loaded dataset from state.

    TODO: If ``state`` has no ``"dataset"`` key, return
    ``"No data loaded"``. Otherwise return a summary string such as
    ``"Loaded {n} records"`` where *n* is ``len(state["dataset"])``.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. State-Mutating Tool                                             #
# ------------------------------------------------------------------ #


@tool
def load_dataset(
    dataset: dict,
    tool_call_id: Annotated[str, InjectedToolCallId],
) -> Command:
    """Store a dataset in the graph state via a Command.

    TODO: Return a ``Command`` whose ``update`` dict sets the
    ``"dataset"`` key to *dataset* and includes a ``ToolMessage``
    confirming the load.
    """
    raise NotImplementedError
