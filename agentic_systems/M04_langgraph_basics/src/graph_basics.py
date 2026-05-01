"""Module 4 -- LangGraph Basics

| Group | Functions | Concept |
|-------|-----------|---------|
| State definition | MessageState (TypedDict) | messages list with add_messages reducer |
| Node functions | echo_node, upper_node | Transform state, return update dict |
| Conditional routing | should_continue | Inspect state, return next node name or END |
| Graph builder | build_echo_graph | Wire START -> echo -> END |
| Graph with branching | build_branching_graph | Conditional edges based on message content |

**TDD Red Phase** -- all tests fail. Implement each stub.
"""

from __future__ import annotations

from typing import Annotated

from typing_extensions import TypedDict

from langgraph.graph import StateGraph, START, END  # noqa: F401 — needed for implementation
from langgraph.graph.message import add_messages
from langchain_core.messages import AIMessage  # noqa: F401 — needed for implementation


# ------------------------------------------------------------------ #
#  1. State Definition                                                #
# ------------------------------------------------------------------ #


class MessageState(TypedDict):
    """Graph state carrying a conversation history.

    TODO: Define a single key ``messages`` typed as
    ``Annotated[list, add_messages]`` so LangGraph applies the
    add_messages reducer automatically.
    """

    messages: Annotated[list, add_messages]


# ------------------------------------------------------------------ #
#  2. Node Functions                                                  #
# ------------------------------------------------------------------ #


def echo_node(state: MessageState) -> dict:
    """Append an AI message that echoes the last user message.

    TODO: Read the last message from ``state["messages"]``, create an
    ``AIMessage`` whose content is ``f"Echo: {last.content}"``, and
    return ``{"messages": [ai_msg]}``.
    """
    raise NotImplementedError


def upper_node(state: MessageState) -> dict:
    """Append an AI message with the last message's content uppercased.

    TODO: Read the last message, uppercase its content, wrap in an
    ``AIMessage``, and return the update dict.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. Conditional Routing                                             #
# ------------------------------------------------------------------ #


def should_continue(state: MessageState) -> str:
    """Decide the next node based on the last message content.

    TODO: If the last message's content contains the word ``"MORE"``
    (case-insensitive), return ``"upper"``; otherwise return ``END``.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Graph Builders                                                  #
# ------------------------------------------------------------------ #


def build_echo_graph() -> StateGraph:
    """Build and compile a simple graph: START -> echo_node -> END.

    TODO:
    1. Create a ``StateGraph(MessageState)``.
    2. Add ``echo_node`` as a node.
    3. Add edges: START -> "echo_node" -> END.
    4. Return the *compiled* graph.
    """
    raise NotImplementedError


def build_branching_graph() -> StateGraph:
    """Build and compile a graph with conditional branching.

    Flow:
        START -> echo_node -> conditional(should_continue)
            "upper" -> upper_node -> END
            END     -> END

    TODO:
    1. Create a ``StateGraph(MessageState)``.
    2. Add ``echo_node`` and ``upper_node`` as nodes.
    3. Add edge START -> "echo_node".
    4. Add conditional edges from "echo_node" using ``should_continue``.
    5. Add edge "upper_node" -> END.
    6. Return the *compiled* graph.
    """
    raise NotImplementedError
