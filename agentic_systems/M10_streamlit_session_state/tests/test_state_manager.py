"""Tests for Module 10 — Streamlit Session State

Every test should FAIL until the stubs in state_manager.py are implemented.
"""

from __future__ import annotations

from datetime import date

import pytest

from state_manager import (
    Scope,
    clear_on_scope_change,
    get_cached,
    is_cached,
    make_hashable,
    set_cached,
)


@pytest.fixture
def scope_a() -> Scope:
    return Scope(
        region="WCSA",
        terminals=("CLVAP", "PECLL"),
        start_date=date(2025, 1, 1),
        end_date=date(2025, 3, 31),
    )


@pytest.fixture
def scope_b() -> Scope:
    return Scope(
        region="ECSA",
        terminals=("BRSSZ",),
        start_date=date(2025, 4, 1),
        end_date=date(2025, 6, 30),
    )


# --- is_cached --- #


def test_is_cached_true(scope_a):
    session = {
        "generated_files": {"report.pdf": b"data"},
        "files_scope": scope_a,
    }
    assert is_cached(session, "report.pdf", scope_a) is True


def test_is_cached_false_missing_key(scope_a):
    session = {
        "generated_files": {},
        "files_scope": scope_a,
    }
    assert is_cached(session, "report.pdf", scope_a) is False


def test_is_cached_false_wrong_scope(scope_a, scope_b):
    session = {
        "generated_files": {"report.pdf": b"data"},
        "files_scope": scope_b,
    }
    assert is_cached(session, "report.pdf", scope_a) is False


# --- get_cached --- #


def test_get_cached_present():
    session = {"generated_files": {"file.xlsx": b"excel-bytes"}}
    assert get_cached(session, "file.xlsx") == b"excel-bytes"


def test_get_cached_missing():
    session = {"generated_files": {}}
    assert get_cached(session, "missing.pdf") is None


# --- set_cached --- #


def test_set_cached_stores_data(scope_a):
    session: dict = {"generated_files": {}}
    set_cached(session, "output.csv", b"csv-data", scope_a)
    assert session["generated_files"]["output.csv"] == b"csv-data"


def test_set_cached_sets_scope(scope_a):
    session: dict = {"generated_files": {}}
    set_cached(session, "output.csv", b"csv-data", scope_a)
    assert session["files_scope"] == scope_a


# --- clear_on_scope_change --- #


def test_clear_on_scope_change_different_scope(scope_a, scope_b):
    session = {
        "files_scope": scope_a,
        "generated_files": {"a.pdf": b"a"},
        "optimized_files": {"b.pdf": b"b"},
        "other_key": "preserved",
    }
    result = clear_on_scope_change(session, scope_b)
    assert result is True
    assert "generated_files" not in session
    assert "optimized_files" not in session


def test_clear_on_scope_change_same_scope(scope_a):
    session = {
        "files_scope": scope_a,
        "generated_files": {"a.pdf": b"a"},
    }
    result = clear_on_scope_change(session, scope_a)
    assert result is False
    assert "generated_files" in session


def test_clear_preserves_other_keys(scope_a, scope_b):
    session = {
        "files_scope": scope_a,
        "generated_files": {"x": b"x"},
        "optimized_files": {"y": b"y"},
        "chat_sessions": {"run-1": {}},
    }
    clear_on_scope_change(session, scope_b)
    assert session["chat_sessions"] == {"run-1": {}}


# --- make_hashable --- #


def test_make_hashable_list():
    result = make_hashable([1, 2, 3])
    assert result == (1, 2, 3)
    assert isinstance(result, tuple)


def test_make_hashable_nested():
    result = make_hashable({"a": [1, 2], "b": {"c": [3]}})
    # Should be a frozenset of items, recursively converted
    assert isinstance(result, frozenset)
    # The nested list should become a tuple
    result_dict = dict(result)
    assert result_dict["a"] == (1, 2)
