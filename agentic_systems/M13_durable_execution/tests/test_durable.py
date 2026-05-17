"""Tests for Module 13 -- Durable Execution Fundamentals.

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

import pytest

from durable import (
    DurableRuntime,
    Journal,
    JournalEntry,
    JournalMismatchError,
    VirtualObject,
)


# --- JournalEntry ---


def test_journal_entry_defaults():
    entry = JournalEntry()
    assert entry.index == 0
    assert entry.action_name == ""
    assert entry.result is None


def test_journal_entry_custom_values():
    entry = JournalEntry(index=3, action_name="fetch", result={"ok": True})
    assert entry.index == 3
    assert entry.action_name == "fetch"
    assert entry.result == {"ok": True}


# --- JournalMismatchError ---


def test_mismatch_error_message():
    err = JournalMismatchError(index=2, expected="fetch", actual="store")
    assert "2" in str(err)
    assert "fetch" in str(err)
    assert "store" in str(err)


def test_mismatch_error_attributes():
    err = JournalMismatchError(index=5, expected="a", actual="b")
    assert err.index == 5
    assert err.expected == "a"
    assert err.actual == "b"


def test_mismatch_error_is_exception():
    with pytest.raises(JournalMismatchError):
        raise JournalMismatchError(index=0, expected="x", actual="y")


# --- Journal ---


def test_journal_starts_empty():
    j = Journal()
    assert len(j) == 0


def test_journal_append_returns_entry():
    j = Journal()
    entry = j.append("fetch", 42)
    assert isinstance(entry, JournalEntry)
    assert entry.index == 0
    assert entry.action_name == "fetch"
    assert entry.result == 42


def test_journal_append_increments_index():
    j = Journal()
    e0 = j.append("a", 1)
    e1 = j.append("b", 2)
    assert e0.index == 0
    assert e1.index == 1
    assert len(j) == 2


def test_journal_get_valid_index():
    j = Journal()
    j.append("fetch", "data")
    entry = j.get(0)
    assert entry is not None
    assert entry.action_name == "fetch"


def test_journal_get_out_of_bounds():
    j = Journal()
    assert j.get(0) is None
    assert j.get(99) is None


# --- DurableRuntime first run ---


async def test_runtime_first_run_calls_fn():
    rt = DurableRuntime()
    result = await rt.run("add", lambda: 2 + 3)
    assert result == 5


async def test_runtime_first_run_records_journal():
    rt = DurableRuntime()
    await rt.run("add", lambda: 10)
    assert len(rt.journal) == 1
    assert rt.journal.get(0).action_name == "add"
    assert rt.journal.get(0).result == 10


async def test_runtime_first_run_multiple_actions():
    rt = DurableRuntime()
    await rt.run("step1", lambda: "a")
    await rt.run("step2", lambda: "b")
    assert len(rt.journal) == 2
    assert rt.journal.get(1).action_name == "step2"


async def test_runtime_replayed_count_is_zero_on_first_run():
    rt = DurableRuntime()
    await rt.run("x", lambda: 1)
    assert rt.replayed_count == 0


# --- DurableRuntime replay ---


async def test_runtime_replay_returns_stored_result():
    prior = Journal()
    prior.append("fetch", {"status": "ok"})

    rt = DurableRuntime(prior_journal=prior)
    result = await rt.run("fetch", lambda: {"status": "should not be called"})
    assert result == {"status": "ok"}


async def test_runtime_replay_does_not_call_fn():
    prior = Journal()
    prior.append("fetch", 42)

    called = []
    rt = DurableRuntime(prior_journal=prior)
    await rt.run("fetch", lambda: called.append(1) or 99)
    assert len(called) == 0


async def test_runtime_replay_increments_replayed_count():
    prior = Journal()
    prior.append("a", 1)
    prior.append("b", 2)

    rt = DurableRuntime(prior_journal=prior)
    await rt.run("a", lambda: None)
    assert rt.replayed_count == 1
    await rt.run("b", lambda: None)
    assert rt.replayed_count == 2


async def test_runtime_replay_records_in_new_journal():
    prior = Journal()
    prior.append("fetch", "cached")

    rt = DurableRuntime(prior_journal=prior)
    await rt.run("fetch", lambda: None)
    assert len(rt.journal) == 1
    assert rt.journal.get(0).result == "cached"


# --- DurableRuntime determinism check ---


async def test_runtime_replay_mismatch_raises():
    prior = Journal()
    prior.append("fetch", 42)

    rt = DurableRuntime(prior_journal=prior)
    with pytest.raises(JournalMismatchError):
        await rt.run("store", lambda: None)


# --- DurableRuntime mixed replay + new ---


async def test_runtime_replay_then_new():
    prior = Journal()
    prior.append("step1", "old")

    rt = DurableRuntime(prior_journal=prior)
    r1 = await rt.run("step1", lambda: "should skip")
    r2 = await rt.run("step2", lambda: "new_result")
    assert r1 == "old"
    assert r2 == "new_result"
    assert len(rt.journal) == 2
    assert rt.replayed_count == 1


# --- DurableRuntime async fn support ---


async def test_runtime_handles_async_fn():
    async def async_add():
        return 7 + 8

    rt = DurableRuntime()
    result = await rt.run("async_add", async_add)
    assert result == 15


# --- VirtualObject ---


def test_virtual_object_key():
    vo = VirtualObject("order-123")
    assert vo.key == "order-123"


def test_virtual_object_empty_state():
    vo = VirtualObject("k")
    assert vo.get_state() == {}


def test_virtual_object_set_and_get_state():
    vo = VirtualObject("k")
    vo.set_state({"count": 5})
    assert vo.get_state() == {"count": 5}


def test_virtual_object_state_is_copy():
    vo = VirtualObject("k")
    vo.set_state({"items": [1, 2]})
    state = vo.get_state()
    state["items"].append(3)
    assert vo.get_state() == {"items": [1, 2]}


def test_virtual_object_has_journal():
    vo = VirtualObject("k")
    assert isinstance(vo.journal, Journal)
    assert len(vo.journal) == 0
