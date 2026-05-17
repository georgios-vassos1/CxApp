"""Module 13 -- Durable Execution Fundamentals

**TDD Red Phase** -- all tests fail. Implement each stub.

This module builds a minimal durable execution engine that models Restate's
core concepts: an append-only journal that records side-effects, replay
that returns journaled results instead of re-executing, and a determinism
contract that rejects handlers whose call sequence diverges on replay.

No Restate dependency is needed -- the entire model is in-process.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Journal entry | JournalEntry (dataclass) | index, action name, result blob |
| Journal | Journal | Append-only log, lookup by index |
| Runtime | DurableRuntime | Execute or replay side-effects, determinism check |
| Handler protocol | Handler (Protocol) | async callable(runtime) -> result |
| VirtualObject | VirtualObject | Keyed entity with serial execution, state dict |
| Journal mismatch | JournalMismatchError | Raised when replay diverges from journal |
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Callable, Protocol


# ------------------------------------------------------------------ #
#  1. Journal Entry                                                   #
# ------------------------------------------------------------------ #


@dataclass
class JournalEntry:
    """A single recorded side-effect in the journal.

    TODO: Define three fields:
      - index (int) -- position in the journal
      - action_name (str) -- identifies the operation
      - result (Any) -- the stored return value
    """

    index: int = 0
    action_name: str = ""
    result: Any = None


# ------------------------------------------------------------------ #
#  2. Journal Mismatch Error                                          #
# ------------------------------------------------------------------ #


class JournalMismatchError(Exception):
    """Raised when a replayed action does not match the journal.

    TODO: Accept ``index``, ``expected`` (action name from journal), and
    ``actual`` (action name from handler). Store all three as attributes
    and build a message like:
      "Journal mismatch at index {index}: expected '{expected}', got '{actual}'"
    """

    def __init__(self, index: int, expected: str, actual: str) -> None:
        raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. Journal                                                         #
# ------------------------------------------------------------------ #


class Journal:
    """An append-only log of side-effect entries.

    TODO: Internally store entries in a list. Provide ``append()``,
    ``get(index)``, and ``__len__()`` methods.
    """

    def __init__(self) -> None:
        """Initialise an empty journal.

        TODO: Create an empty list of JournalEntry objects.
        """
        raise NotImplementedError

    def append(self, action_name: str, result: Any) -> JournalEntry:
        """Record a new entry at the next index.

        TODO: Create a JournalEntry with index = current length,
        append it, and return it.
        """
        raise NotImplementedError

    def get(self, index: int) -> JournalEntry | None:
        """Return the entry at the given index, or None if out of bounds.

        TODO: Return self._entries[index] if valid, else None.
        """
        raise NotImplementedError

    def __len__(self) -> int:
        """Return the number of entries in the journal.

        TODO: Return the length of the entries list.
        """
        raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Durable Runtime                                                 #
# ------------------------------------------------------------------ #


class DurableRuntime:
    """Executes side-effects on first run, replays them on subsequent runs.

    On first run (no prior journal), ``run()`` executes the callable and
    records the result. On replay (prior journal provided), ``run()``
    checks that the action name matches the journal entry at the current
    index, returns the stored result without calling the function, and
    advances the index.

    TODO: The constructor accepts an optional ``prior_journal`` (Journal).
    Maintain a ``_replay_index`` counter and a ``_journal`` for new entries.
    """

    def __init__(self, prior_journal: Journal | None = None) -> None:
        """Initialise the runtime.

        TODO:
          - Store prior_journal (or None).
          - Create a new Journal for this run.
          - Set _replay_index = 0.
        """
        raise NotImplementedError

    async def run(self, action_name: str, fn: Callable[[], Any]) -> Any:
        """Execute or replay a named side-effect.

        TODO:
        If replaying (prior_journal exists and _replay_index < len(prior)):
          1. Get the journal entry at _replay_index.
          2. If entry.action_name != action_name, raise JournalMismatchError.
          3. Record the same result in the new journal (for continuity).
          4. Increment _replay_index.
          5. Return the stored result (do NOT call fn).

        If not replaying (no prior journal, or past the end):
          1. Call fn() and await it if it's a coroutine.
          2. Record the result in the new journal.
          3. Return the result.
        """
        raise NotImplementedError

    @property
    def journal(self) -> Journal:
        """Return the journal built during this run.

        TODO: Return self._journal.
        """
        raise NotImplementedError

    @property
    def replayed_count(self) -> int:
        """Return the number of entries replayed so far.

        TODO: Return self._replay_index.
        """
        raise NotImplementedError


# ------------------------------------------------------------------ #
#  5. VirtualObject                                                   #
# ------------------------------------------------------------------ #


class VirtualObject:
    """A keyed stateful entity with serial execution.

    Models a Restate VirtualObject: each key maps to an isolated state
    dict and a journal. Concurrent calls to the same key are queued
    (modelled here by simple sequential execution).

    TODO: The constructor accepts a ``key`` (str). Maintain an internal
    ``_state`` dict and a ``_journal`` (Journal).
    """

    def __init__(self, key: str) -> None:
        """Initialise the VirtualObject.

        TODO: Store key, create empty state dict, create empty Journal.
        """
        raise NotImplementedError

    @property
    def key(self) -> str:
        """Return the VirtualObject key.

        TODO: Return self._key.
        """
        raise NotImplementedError

    def get_state(self) -> dict[str, Any]:
        """Return a copy of the current state.

        TODO: Return a shallow copy of self._state.
        """
        raise NotImplementedError

    def set_state(self, state: dict[str, Any]) -> None:
        """Replace the current state.

        TODO: Set self._state to a shallow copy of state.
        """
        raise NotImplementedError

    @property
    def journal(self) -> Journal:
        """Return the VirtualObject's journal.

        TODO: Return self._journal.
        """
        raise NotImplementedError
