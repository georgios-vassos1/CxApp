"""Tests for Module 2 — Multi-turn Sessions.

**TDD Red Phase** — all tests fail. Implement each stub in sessions.py.
"""

from __future__ import annotations

from datetime import datetime

import pytest

from sessions import Session, SessionNotFoundError, SessionStore


# --- SessionStore.create ---


def test_create_returns_session_with_context_id():
    store = SessionStore()
    session = store.create()
    assert isinstance(session, Session)
    assert isinstance(session.context_id, str)
    assert len(session.context_id) > 0


def test_create_assigns_unique_ids():
    store = SessionStore()
    ids = {store.create().context_id for _ in range(5)}
    assert len(ids) == 5


def test_create_initialises_empty_history():
    store = SessionStore()
    session = store.create()
    assert session.history == []


def test_create_sets_created_at():
    store = SessionStore()
    before = datetime.utcnow()
    session = store.create()
    after = datetime.utcnow()
    assert before <= session.created_at <= after


# --- SessionStore.get ---


def test_get_returns_existing_session():
    store = SessionStore()
    created = store.create()
    fetched = store.get(created.context_id)
    assert fetched is created


def test_get_missing_raises_session_not_found():
    store = SessionStore()
    with pytest.raises(SessionNotFoundError):
        store.get("nonexistent-id")


# --- SessionStore.list_sessions ---


def test_list_sessions_returns_all_context_ids():
    store = SessionStore()
    s1 = store.create()
    s2 = store.create()
    ids = store.list_sessions()
    assert set(ids) == {s1.context_id, s2.context_id}


# --- SessionStore.delete ---


def test_delete_removes_session():
    store = SessionStore()
    session = store.create()
    store.delete(session.context_id)
    assert session.context_id not in store.list_sessions()


def test_delete_missing_raises_session_not_found():
    store = SessionStore()
    with pytest.raises(SessionNotFoundError):
        store.delete("nonexistent-id")


# --- SessionStore.append_message ---


def test_append_message_adds_to_history():
    store = SessionStore()
    session = store.create()
    store.append_message(session.context_id, "user", "hello")
    store.append_message(session.context_id, "assistant", "hi there")
    assert len(session.history) == 2
    assert session.history[0] == {"role": "user", "text": "hello"}
    assert session.history[1] == {"role": "assistant", "text": "hi there"}


def test_append_message_missing_session_raises():
    store = SessionStore()
    with pytest.raises(SessionNotFoundError):
        store.append_message("bad-id", "user", "hello")


# --- SessionStore.get_or_create ---


def test_get_or_create_none_creates_new():
    store = SessionStore()
    session = store.get_or_create(None)
    assert isinstance(session, Session)
    assert session.context_id in store.list_sessions()


def test_get_or_create_existing_returns_same():
    store = SessionStore()
    created = store.create()
    fetched = store.get_or_create(created.context_id)
    assert fetched is created


def test_get_or_create_unknown_raises():
    store = SessionStore()
    with pytest.raises(SessionNotFoundError):
        store.get_or_create("unknown-id")


# --- SessionStore.evict_oldest ---


def test_evict_oldest_removes_earliest():
    store = SessionStore(max_sessions=10)
    first = store.create()
    _second = store.create()
    evicted_id = store.evict_oldest()
    assert evicted_id == first.context_id
    assert first.context_id not in store.list_sessions()


def test_evict_oldest_empty_returns_none():
    store = SessionStore()
    assert store.evict_oldest() is None


# --- auto-eviction ---


def test_auto_evict_on_create_when_at_capacity():
    store = SessionStore(max_sessions=2)
    s1 = store.create()
    _s2 = store.create()
    s3 = store.create()  # should evict s1
    assert s1.context_id not in store.list_sessions()
    assert s3.context_id in store.list_sessions()
    assert len(store.list_sessions()) == 2


def test_max_sessions_boundary():
    store = SessionStore(max_sessions=3)
    for _ in range(5):
        store.create()
    assert len(store.list_sessions()) <= 3
