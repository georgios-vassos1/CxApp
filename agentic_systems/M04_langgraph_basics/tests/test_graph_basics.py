"""Tests for Module 4 -- LangGraph Basics.

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

from langchain_core.messages import AIMessage, HumanMessage
from langgraph.graph import END

from graph_basics import (
    MessageState,
    build_branching_graph,
    build_echo_graph,
    echo_node,
    should_continue,
    upper_node,
)


# --- echo_node ---


def test_echo_node_returns_echo():
    state: MessageState = {"messages": [HumanMessage(content="hello")]}
    result = echo_node(state)
    assert "messages" in result
    assert result["messages"][0].content == "Echo: hello"


def test_echo_node_returns_ai_message():
    state: MessageState = {"messages": [HumanMessage(content="test")]}
    result = echo_node(state)
    assert isinstance(result["messages"][0], AIMessage)


# --- upper_node ---


def test_upper_node_uppercases():
    state: MessageState = {"messages": [HumanMessage(content="hello world")]}
    result = upper_node(state)
    assert result["messages"][0].content == "HELLO WORLD"


def test_upper_node_returns_ai_message():
    state: MessageState = {"messages": [HumanMessage(content="test")]}
    result = upper_node(state)
    assert isinstance(result["messages"][0], AIMessage)


# --- should_continue ---


def test_should_continue_returns_end_for_normal():
    state: MessageState = {"messages": [AIMessage(content="Echo: hello")]}
    assert should_continue(state) == END


def test_should_continue_returns_upper_for_more():
    state: MessageState = {"messages": [AIMessage(content="Echo: tell me MORE")]}
    assert should_continue(state) == "upper"


def test_should_continue_case_insensitive():
    state: MessageState = {"messages": [AIMessage(content="Echo: more please")]}
    assert should_continue(state) == "upper"


# --- build_echo_graph ---


def test_echo_graph_returns_echo():
    graph = build_echo_graph()
    result = graph.invoke({"messages": [HumanMessage(content="hi")]})
    assert result["messages"][-1].content == "Echo: hi"


def test_echo_graph_preserves_history():
    graph = build_echo_graph()
    result = graph.invoke({"messages": [HumanMessage(content="hi")]})
    # Should have the original HumanMessage plus the echo AIMessage
    assert len(result["messages"]) == 2


# --- build_branching_graph ---


def test_branching_more_triggers_upper():
    graph = build_branching_graph()
    result = graph.invoke({"messages": [HumanMessage(content="say MORE")]})
    last = result["messages"][-1]
    # upper_node uppercases the echo, so last message should be uppercased
    assert last.content == last.content.upper()


def test_branching_no_more_goes_to_end():
    graph = build_branching_graph()
    result = graph.invoke({"messages": [HumanMessage(content="just hello")]})
    last = result["messages"][-1]
    assert last.content == "Echo: just hello"


# --- state type ---


def test_state_has_correct_type():
    """MessageState must be a TypedDict with a 'messages' key."""
    assert "messages" in MessageState.__annotations__
