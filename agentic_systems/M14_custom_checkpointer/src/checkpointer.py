"""Module 14 -- Custom Checkpointer (Preloaded / Zero-I/O Pattern)

**TDD Red Phase** -- all tests fail. Implement each stub.

This module builds a PreloadedCheckpointer that models the pattern used
when LangGraph runs inside a durable execution runtime (like Restate).
The outer runtime handles persistence; the checkpointer only serves
state that was loaded *before* graph execution -- it never performs I/O
during the graph run.

Key ideas:
  - Base64 encoding makes checkpoint blobs JSON-safe for journal storage
  - The checkpointer is initialised with pre-fetched state (or empty)
  - During graph execution it returns preloaded state and captures writes
  - After execution the caller retrieves pending writes for external save

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Serialisation | encode_checkpoint, decode_checkpoint | dict <-> base64 JSON blob |
| Checkpoint tuple | CheckpointTuple (dataclass) | checkpoint dict + metadata |
| Checkpointer | PreloadedCheckpointer | Zero-I/O, preloaded state, pending writes |
| Factory | create_preloaded | Build a PreloadedCheckpointer from raw bytes or None |
"""

from __future__ import annotations

import base64  # noqa: F401
import json  # noqa: F401
from dataclasses import dataclass, field
from typing import Any


# ------------------------------------------------------------------ #
#  1. Serialisation Helpers                                           #
# ------------------------------------------------------------------ #


def encode_checkpoint(checkpoint: dict[str, Any]) -> str:
    """Serialise a checkpoint dict to a base64-encoded JSON string.

    TODO:
      1. JSON-serialise the dict to a string.
      2. UTF-8 encode to bytes.
      3. Base64-encode and return the result as a UTF-8 string.
    """
    raise NotImplementedError


def decode_checkpoint(blob: str) -> dict[str, Any]:
    """Deserialise a base64-encoded JSON string back to a dict.

    TODO:
      1. Base64-decode the blob string to bytes.
      2. UTF-8 decode to a JSON string.
      3. Parse the JSON and return the dict.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  2. Checkpoint Tuple                                                #
# ------------------------------------------------------------------ #


@dataclass
class CheckpointTuple:
    """A checkpoint with metadata, modelling LangGraph's CheckpointTuple.

    TODO: Define three fields:
      - config (dict) -- configurable metadata (e.g. thread_id)
      - checkpoint (dict) -- the serialisable agent state
      - metadata (dict) -- optional extra info (defaults to empty dict)
    """

    config: dict = field(default_factory=dict)
    checkpoint: dict = field(default_factory=dict)
    metadata: dict = field(default_factory=dict)


# ------------------------------------------------------------------ #
#  3. PreloadedCheckpointer                                           #
# ------------------------------------------------------------------ #


class PreloadedCheckpointer:
    """A zero-I/O checkpointer that serves preloaded state.

    On construction it receives an optional CheckpointTuple representing
    state fetched by the outer runtime *before* graph execution began.
    During the graph run:
      - ``get_tuple()`` returns the preloaded state (once, then None)
      - ``put()`` captures writes into a pending-writes list
    After the run the caller reads ``pending_writes`` and persists them
    externally (e.g. via Restate's journal or a database).

    TODO: The constructor accepts an optional ``preloaded``
    (CheckpointTuple | None). Maintain a ``_pending_writes`` list and
    a ``_served`` flag that tracks whether get_tuple has been called.
    """

    def __init__(self, preloaded: CheckpointTuple | None = None) -> None:
        """Initialise the checkpointer.

        TODO:
          - Store the preloaded tuple (or None).
          - Create an empty list for pending writes.
          - Set _served = False.
        """
        raise NotImplementedError

    async def get_tuple(self, config: dict | None = None) -> CheckpointTuple | None:
        """Return preloaded state on first call, None thereafter.

        TODO:
          - If not yet served and preloaded is not None, set _served = True
            and return the preloaded CheckpointTuple.
          - Otherwise return None.
        """
        raise NotImplementedError

    async def put(self, config: dict, checkpoint: dict, metadata: dict | None = None) -> None:
        """Capture a write without performing I/O.

        TODO:
          - Create a CheckpointTuple from the arguments.
          - Append it to _pending_writes.
        """
        raise NotImplementedError

    @property
    def pending_writes(self) -> list[CheckpointTuple]:
        """Return the list of writes captured during graph execution.

        TODO: Return self._pending_writes.
        """
        raise NotImplementedError

    @property
    def has_preloaded(self) -> bool:
        """Return True if this checkpointer was initialised with state.

        TODO: Return whether self._preloaded is not None.
        """
        raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Factory                                                         #
# ------------------------------------------------------------------ #


def create_preloaded(
    raw_blob: str | None = None,
    config: dict | None = None,
) -> PreloadedCheckpointer:
    """Build a PreloadedCheckpointer from a raw base64 blob or None.

    TODO:
      - If raw_blob is None or empty, return PreloadedCheckpointer()
        (no preloaded state).
      - Otherwise decode the blob with decode_checkpoint, wrap it in a
        CheckpointTuple (using config or empty dict), and return a
        PreloadedCheckpointer with that tuple.
    """
    raise NotImplementedError
