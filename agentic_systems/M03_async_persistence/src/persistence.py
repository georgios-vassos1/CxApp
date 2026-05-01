"""Module 3 — Async Persistence

**TDD Red Phase** — all tests fail. Implement each stub.

This module builds an async checkpoint store that mirrors the LangGraph
``AsyncSqliteSaver`` pattern.  Two backends are provided: an in-memory
dict-based checkpointer (for tests) and an aiosqlite-backed checkpointer
(for production).  A factory function selects the backend based on
configuration, and a ``LazyCheckpointer`` wrapper defers construction
to avoid import-time event-loop issues.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Checkpoint model | Checkpoint (dataclass) | thread_id, state (dict), version (int) |
| In-memory backend | MemoryCheckpointer | dict-based, save/load/list, for tests |
| SQLite backend | SqliteCheckpointer | aiosqlite, async context manager, CREATE TABLE |
| Factory | create_checkpointer | DATA_DIR -> SQLite; empty/None -> MemoryCheckpointer |
| Lazy init | LazyCheckpointer | Deferred construction, avoids import-time event loop |
"""

from __future__ import annotations

import json  # noqa: F401 — needed for implementation
from dataclasses import dataclass, field
from typing import Callable

import aiosqlite


# ------------------------------------------------------------------ #
#  1. Checkpoint Model                                                #
# ------------------------------------------------------------------ #


@dataclass
class Checkpoint:
    """An immutable snapshot of agent state for a given thread.

    TODO: Define three fields:
      - thread_id (str) — identifies the conversation thread
      - state (dict) — arbitrary JSON-serialisable agent state
      - version (int) — monotonically increasing version counter
    """

    thread_id: str = ""
    state: dict = field(default_factory=dict)
    version: int = 0


# ------------------------------------------------------------------ #
#  2. In-memory Backend                                               #
# ------------------------------------------------------------------ #


class MemoryCheckpointer:
    """Dict-backed checkpoint store for unit tests.

    TODO: Internally store checkpoints in a dict keyed by thread_id.
    Each save increments the version for that thread.
    """

    def __init__(self) -> None:
        """Initialise the in-memory store.

        TODO: Create an empty dict to hold Checkpoint objects.
        """
        raise NotImplementedError

    async def save(self, thread_id: str, state: dict) -> Checkpoint:
        """Save a checkpoint, incrementing the version.

        TODO: If thread_id already exists, increment its version by 1.
        Otherwise start at version 1. Store and return the Checkpoint.
        """
        raise NotImplementedError

    async def load(self, thread_id: str) -> Checkpoint | None:
        """Load the latest checkpoint for a thread.

        TODO: Return the Checkpoint if it exists, otherwise None.
        """
        raise NotImplementedError

    async def list_threads(self) -> list[str]:
        """Return all thread_ids that have checkpoints.

        TODO: Return the keys of the internal dict as a list.
        """
        raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. SQLite Backend                                                  #
# ------------------------------------------------------------------ #


class SqliteCheckpointer:
    """aiosqlite-backed checkpoint store.

    TODO: The constructor accepts ``db_path`` (str).  All async methods
    should open a connection via ``async with aiosqlite.connect(db_path)``,
    ensuring the table exists on each call (CREATE TABLE IF NOT EXISTS).

    Table schema:
      checkpoints(thread_id TEXT PRIMARY KEY, state TEXT, version INTEGER)

    The ``state`` column stores JSON-serialised dicts.
    """

    def __init__(self, db_path: str) -> None:
        """Store the database path.

        TODO: Save db_path as an instance attribute.
        """
        raise NotImplementedError

    async def _ensure_table(self, db: aiosqlite.Connection) -> None:
        """Create the checkpoints table if it does not exist.

        TODO: Execute CREATE TABLE IF NOT EXISTS with the schema above.
        """
        raise NotImplementedError

    async def save(self, thread_id: str, state: dict) -> Checkpoint:
        """Save a checkpoint to SQLite.

        TODO: Connect to the database, ensure the table exists, then
        UPSERT the row.  Read back the current version (or default to 1
        for new threads) and return a Checkpoint.
        """
        raise NotImplementedError

    async def load(self, thread_id: str) -> Checkpoint | None:
        """Load the latest checkpoint from SQLite.

        TODO: Connect, ensure table, query by thread_id.  Return None
        if the row does not exist.  Deserialise the state JSON.
        """
        raise NotImplementedError

    async def list_threads(self) -> list[str]:
        """Return all thread_ids from the database.

        TODO: Connect, ensure table, SELECT all thread_ids.
        """
        raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Factory                                                         #
# ------------------------------------------------------------------ #


def create_checkpointer(data_dir: str | None) -> MemoryCheckpointer | SqliteCheckpointer:
    """Select a checkpointer backend based on configuration.

    TODO:
      - If data_dir is None or empty string -> return MemoryCheckpointer()
      - Otherwise -> return SqliteCheckpointer(f"{data_dir}/checkpoints.db")
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  5. Lazy Init                                                       #
# ------------------------------------------------------------------ #


class LazyCheckpointer:
    """Deferred checkpointer construction.

    TODO: Accepts a ``factory`` callable in the constructor. The first
    call to ``get()`` invokes the factory, caches the result, and
    returns it.  Subsequent calls return the cached instance.
    """

    def __init__(self, factory: Callable) -> None:
        """Store the factory and initialise the cache to None.

        TODO: Save factory and set _instance = None.
        """
        raise NotImplementedError

    async def get(self) -> MemoryCheckpointer | SqliteCheckpointer:
        """Return the checkpointer, creating it on first call.

        TODO: If _instance is None, call factory() and cache.  Return
        _instance.
        """
        raise NotImplementedError
