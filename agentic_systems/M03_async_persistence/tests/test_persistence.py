"""Tests for Module 3 — Async Persistence.

**TDD Red Phase** — all tests fail. Implement each stub in persistence.py.
"""

from __future__ import annotations

from persistence import (
    Checkpoint,
    LazyCheckpointer,
    MemoryCheckpointer,
    SqliteCheckpointer,
    create_checkpointer,
)


# --- MemoryCheckpointer.save / load ---


async def test_memory_save_load():
    cp = MemoryCheckpointer()
    saved = await cp.save("thread-1", {"messages": ["hi"]})
    assert isinstance(saved, Checkpoint)
    assert saved.thread_id == "thread-1"
    assert saved.state == {"messages": ["hi"]}
    loaded = await cp.load("thread-1")
    assert loaded is not None
    assert loaded.state == {"messages": ["hi"]}


async def test_memory_load_missing_returns_none():
    cp = MemoryCheckpointer()
    result = await cp.load("nonexistent")
    assert result is None


# --- MemoryCheckpointer.list_threads ---


async def test_memory_list_threads():
    cp = MemoryCheckpointer()
    await cp.save("t1", {"a": 1})
    await cp.save("t2", {"b": 2})
    threads = await cp.list_threads()
    assert set(threads) == {"t1", "t2"}


# --- MemoryCheckpointer version increments ---


async def test_memory_version_increments():
    cp = MemoryCheckpointer()
    v1 = await cp.save("thread-1", {"step": 1})
    assert v1.version == 1
    v2 = await cp.save("thread-1", {"step": 2})
    assert v2.version == 2
    v3 = await cp.save("thread-1", {"step": 3})
    assert v3.version == 3


# --- SqliteCheckpointer.save / load ---


async def test_sqlite_save_load(tmp_path):
    db_path = str(tmp_path / "test.db")
    cp = SqliteCheckpointer(db_path)
    saved = await cp.save("thread-1", {"key": "value"})
    assert isinstance(saved, Checkpoint)
    assert saved.thread_id == "thread-1"
    loaded = await cp.load("thread-1")
    assert loaded is not None
    assert loaded.state == {"key": "value"}


async def test_sqlite_load_missing(tmp_path):
    db_path = str(tmp_path / "test.db")
    cp = SqliteCheckpointer(db_path)
    result = await cp.load("nonexistent")
    assert result is None


# --- SqliteCheckpointer persists across instances ---


async def test_sqlite_persists_across_instances(tmp_path):
    db_path = str(tmp_path / "test.db")
    cp1 = SqliteCheckpointer(db_path)
    await cp1.save("thread-1", {"saved": True})
    # New instance, same db file
    cp2 = SqliteCheckpointer(db_path)
    loaded = await cp2.load("thread-1")
    assert loaded is not None
    assert loaded.state == {"saved": True}


# --- create_checkpointer factory ---


async def test_factory_none_returns_memory():
    cp = create_checkpointer(None)
    assert isinstance(cp, MemoryCheckpointer)


async def test_factory_empty_returns_memory():
    cp = create_checkpointer("")
    assert isinstance(cp, MemoryCheckpointer)


async def test_factory_with_path_returns_sqlite(tmp_path):
    cp = create_checkpointer(str(tmp_path))
    assert isinstance(cp, SqliteCheckpointer)


# --- LazyCheckpointer ---


async def test_lazy_defers_construction():
    calls = []

    def factory():
        calls.append(1)
        return MemoryCheckpointer()

    lazy = LazyCheckpointer(factory)
    assert len(calls) == 0  # not yet called
    instance = await lazy.get()
    assert len(calls) == 1
    assert isinstance(instance, MemoryCheckpointer)


async def test_lazy_caches_instance():
    def factory():
        return MemoryCheckpointer()

    lazy = LazyCheckpointer(factory)
    first = await lazy.get()
    second = await lazy.get()
    assert first is second
