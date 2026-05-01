"""Module 2 — Multi-turn Sessions

**TDD Red Phase** — all tests fail. Implement each stub.

This module builds an in-memory session manager for multi-turn JSON-RPC
conversations.  Each session tracks a conversation history (list of messages)
keyed by a unique ``context_id``.  The ``SessionStore`` supports bounded pools
with LRU-style eviction when a maximum session count is reached.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Custom error | SessionNotFoundError | Raised when context_id is unknown |
| Session model | Session (dataclass) | history list, context_id, created_at |
| Session store | SessionStore | In-memory dict, create/get/list/delete sessions |
| Context routing | get_or_create | If context_id is None -> new; exists -> return; unknown -> raise |
| Lifecycle | evict_oldest, max_sessions | Bounded session pool, LRU eviction |
"""

from __future__ import annotations

import uuid  # noqa: F401 — needed for implementation
from dataclasses import dataclass, field
from datetime import datetime


# ------------------------------------------------------------------ #
#  1. Custom Error                                                    #
# ------------------------------------------------------------------ #


class SessionNotFoundError(Exception):
    """Raised when a session lookup fails for an unknown context_id.

    TODO: Inherit from Exception. No extra logic needed — the message
    is the context_id that was not found.
    """

    pass


# ------------------------------------------------------------------ #
#  2. Session Model                                                   #
# ------------------------------------------------------------------ #


@dataclass
class Session:
    """A single multi-turn conversation session.

    TODO: Define three fields:
      - context_id (str) — unique identifier, typically a uuid4 hex string
      - history (list[dict]) — list of message dicts, default empty list
      - created_at (datetime) — timestamp, default datetime.utcnow()
    """

    context_id: str = ""
    history: list[dict] = field(default_factory=list)
    created_at: datetime = field(default_factory=datetime.utcnow)


# ------------------------------------------------------------------ #
#  3. Session Store                                                   #
# ------------------------------------------------------------------ #


class SessionStore:
    """In-memory session pool with bounded capacity.

    TODO: The constructor accepts ``max_sessions`` (int, default 5).
    Internally store sessions in an ordered dict or plain dict keyed
    by context_id.
    """

    def __init__(self, max_sessions: int = 5) -> None:
        """Initialise the store.

        TODO: Store max_sessions and create an empty dict for sessions.
        """
        raise NotImplementedError

    # -- CRUD -------------------------------------------------------- #

    def create(self) -> Session:
        """Create a new session with a uuid4 context_id.

        TODO: If the store is at capacity (len >= max_sessions), call
        ``evict_oldest()`` first. Then create a Session, store it,
        and return it.
        """
        raise NotImplementedError

    def get(self, context_id: str) -> Session:
        """Return the session for *context_id*.

        TODO: Raise ``SessionNotFoundError`` if the context_id is not
        in the store.
        """
        raise NotImplementedError

    def list_sessions(self) -> list[str]:
        """Return a list of all active context_ids.

        TODO: Return the keys of the internal dict as a list.
        """
        raise NotImplementedError

    def delete(self, context_id: str) -> None:
        """Remove a session by context_id.

        TODO: Delete the session. Raise ``SessionNotFoundError`` if
        the context_id is not found.
        """
        raise NotImplementedError

    # -- Message handling -------------------------------------------- #

    def append_message(self, context_id: str, role: str, text: str) -> None:
        """Append a message dict to the session's history.

        TODO: Look up the session (raising if missing) and append
        ``{"role": role, "text": text}`` to its history list.
        """
        raise NotImplementedError

    # -- Context routing --------------------------------------------- #

    def get_or_create(self, context_id: str | None) -> Session:
        """Return an existing session or create a new one.

        TODO:
          - If context_id is None -> create() and return new session.
          - If context_id exists in store -> return it.
          - If context_id is non-None but unknown -> raise SessionNotFoundError.
        """
        raise NotImplementedError

    # -- Lifecycle --------------------------------------------------- #

    def evict_oldest(self) -> str | None:
        """Remove and return the oldest session's context_id.

        TODO: Find the session with the earliest ``created_at``,
        remove it from the store, and return its context_id.
        Return None if the store is empty.
        """
        raise NotImplementedError
