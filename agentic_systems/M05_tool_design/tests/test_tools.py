"""Tests for Module 5 -- Tool Design.

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

from langgraph.types import Command

from tools import add_numbers, get_loaded_data, load_dataset


# --- add_numbers ---


def test_add_numbers_basic():
    result = add_numbers.invoke({"a": 2, "b": 3})
    assert result == 5


def test_add_numbers_negative():
    result = add_numbers.invoke({"a": -4, "b": 7})
    assert result == 3


def test_add_numbers_zero():
    result = add_numbers.invoke({"a": 0, "b": 0})
    assert result == 0


# --- get_loaded_data ---


def test_get_loaded_data_empty_state():
    # InjectedState tools can be tested by calling the underlying function directly
    result = get_loaded_data.func(state={})
    assert result == "No data loaded"


def test_get_loaded_data_with_data():
    state = {"dataset": [{"id": 1}, {"id": 2}, {"id": 3}]}
    result = get_loaded_data.func(state=state)
    assert "3" in result  # should mention the count


# --- load_dataset ---


def test_load_dataset_returns_command():
    result = load_dataset.func(dataset={"key": "value"}, tool_call_id="call-123")
    assert isinstance(result, Command)


def test_load_dataset_command_has_dataset_key():
    result = load_dataset.func(dataset={"key": "value"}, tool_call_id="call-123")
    assert "dataset" in result.update


# --- tool metadata ---


def test_add_numbers_tool_name():
    assert add_numbers.name == "add_numbers"


def test_get_loaded_data_tool_name():
    assert get_loaded_data.name == "get_loaded_data"


def test_load_dataset_tool_name():
    assert load_dataset.name == "load_dataset"
