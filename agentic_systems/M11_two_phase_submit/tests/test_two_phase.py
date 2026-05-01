"""Tests for Module 11 — Two-Phase Submit & Callbacks

Every test should FAIL until the stubs in two_phase.py are implemented.
"""

from __future__ import annotations

from unittest.mock import MagicMock

import pytest

from two_phase import (
    ChatSession,
    complete_chat_turn,
    is_processing,
    start_chat_turn,
    toggle_panel,
)


@pytest.fixture
def empty_session() -> dict:
    return {"chat_sessions": {}, "chat_processing": False}


# --- start_chat_turn --- #


def test_start_appends_user_message(empty_session):
    start_chat_turn(empty_session, "hello", "run-1")
    cs = empty_session["chat_sessions"]["run-1"]
    assert len(cs.history) == 1
    assert cs.history[0].role == "user"
    assert cs.history[0].text == "hello"


def test_start_sets_processing_true(empty_session):
    start_chat_turn(empty_session, "hi", "run-1")
    assert empty_session["chat_processing"] is True


def test_start_stores_pending(empty_session):
    start_chat_turn(empty_session, "question", "run-1")
    assert empty_session["chat_pending"] == {
        "prompt": "question",
        "run_id": "run-1",
    }


def test_start_creates_session_if_absent(empty_session):
    assert "run-1" not in empty_session["chat_sessions"]
    start_chat_turn(empty_session, "first msg", "run-1")
    assert "run-1" in empty_session["chat_sessions"]
    assert isinstance(empty_session["chat_sessions"]["run-1"], ChatSession)


# --- complete_chat_turn --- #


def _setup_pending_session(session: dict) -> None:
    """Helper: start a turn so we can complete it."""
    start_chat_turn(session, "user prompt", "run-1")


def test_complete_appends_agent_response(empty_session):
    _setup_pending_session(empty_session)

    def agent_fn(prompt, ctx):
        return ("agent reply", "ctx-123")

    complete_chat_turn(empty_session, agent_fn)
    cs = empty_session["chat_sessions"]["run-1"]
    assert cs.history[-1].role == "agent"
    assert cs.history[-1].text == "agent reply"


def test_complete_clears_processing(empty_session):
    _setup_pending_session(empty_session)

    def agent_fn(prompt, ctx):
        return ("reply", "ctx-1")

    complete_chat_turn(empty_session, agent_fn)
    assert empty_session["chat_processing"] is False


def test_complete_clears_pending(empty_session):
    _setup_pending_session(empty_session)

    def agent_fn(prompt, ctx):
        return ("reply", "ctx-1")

    complete_chat_turn(empty_session, agent_fn)
    assert "chat_pending" not in empty_session


def test_complete_updates_context_id(empty_session):
    _setup_pending_session(empty_session)

    def agent_fn(prompt, ctx):
        return ("reply", "ctx-new")

    complete_chat_turn(empty_session, agent_fn)
    cs = empty_session["chat_sessions"]["run-1"]
    assert cs.context_id == "ctx-new"


def test_complete_calls_agent_fn_with_prompt(empty_session):
    _setup_pending_session(empty_session)
    mock_fn = MagicMock(return_value=("response", "ctx-42"))
    complete_chat_turn(empty_session, mock_fn)
    mock_fn.assert_called_once_with("user prompt", None)


# --- toggle_panel --- #


def test_toggle_absent_sets_true():
    session: dict = {}
    toggle_panel(session)
    assert session["panel_open"] is True


def test_toggle_true_sets_false():
    session = {"panel_open": True}
    toggle_panel(session)
    assert session["panel_open"] is False


def test_toggle_twice_roundtrips():
    session: dict = {}
    toggle_panel(session)
    toggle_panel(session)
    assert session["panel_open"] is False


# --- is_processing --- #


def test_is_processing_default_false():
    assert is_processing({}) is False


def test_is_processing_true_when_set():
    assert is_processing({"chat_processing": True}) is True
