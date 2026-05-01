"""Module 12 — Testing Patterns

**TDD Red Phase** — all tests fail. Implement each stub.

This module provides reusable test helpers for session state testing,
Pydantic model factories, and assertion utilities.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Session mock | make_session | Factory for session state dicts with defaults |
| Pydantic factory | StationFactory, ReadingFactory | polyfactory ModelFactory subclasses |
| Patch helper | patch_session | Context manager that patches session_state |
| Assertion helpers | assert_session_has, assert_session_missing | Readable assertions |
"""

from __future__ import annotations

from contextlib import contextmanager
from typing import Any

from polyfactory.factories.pydantic_factory import ModelFactory
from pydantic import BaseModel


# ------------------------------------------------------------------ #
#  Domain Models (inline for self-contained testing)                  #
# ------------------------------------------------------------------ #


class Station(BaseModel):
    """Weather station metadata."""

    station_id: str
    name: str
    region: str
    elevation_m: int


class Reading(BaseModel):
    """A single weather reading from a station."""

    station_id: str
    timestamp: str
    temperature_c: float
    humidity_pct: int
    wind_speed_kmh: float


# ------------------------------------------------------------------ #
#  Sentinel for optional value checks                                 #
# ------------------------------------------------------------------ #

_SENTINEL = object()


# ------------------------------------------------------------------ #
#  1. Session Mock Factory                                            #
# ------------------------------------------------------------------ #


def make_session(overrides: dict | None = None) -> dict:
    """Create a session state dict with sensible defaults.

    TODO: Return a dict with these defaults:
      - "chat_sessions": {}
      - "chat_processing": False
      - "files_scope": None
      - "generated_files": {}
    Then merge any overrides on top.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  2. Pydantic Factories                                              #
# ------------------------------------------------------------------ #


class StationFactory(ModelFactory):
    """Factory for generating Station instances.

    TODO: Override station_id to generate "WS" + 3-digit random number
    (e.g., "WS042"). The default factory generates arbitrary strings.
    """

    __model__ = Station


class ReadingFactory(ModelFactory):
    """Factory for generating Reading instances.

    TODO: Constrain temperature_c to the range [-20, 45].
    The default factory generates arbitrary floats.
    """

    __model__ = Reading


# ------------------------------------------------------------------ #
#  3. Patch Helper                                                    #
# ------------------------------------------------------------------ #


@contextmanager
def patch_session(target_module: Any, session: dict):
    """Context manager that patches target_module.st.session_state.

    TODO:
      - Save original st.session_state on target_module.
      - Set target_module.st.session_state = session.
      - Yield.
      - Restore original value.

    This enables Streamlit mock testing without a running server.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Assertion Helpers                                               #
# ------------------------------------------------------------------ #


def assert_session_has(
    session: dict, key: str, value: Any = _SENTINEL
) -> None:
    """Assert that a key exists in session (and optionally matches a value).

    TODO:
      - Assert key is in session.
      - If value is not _SENTINEL, assert session[key] == value.
    """
    raise NotImplementedError


def assert_session_missing(session: dict, key: str) -> None:
    """Assert that a key is NOT in session.

    TODO: Assert key not in session.
    """
    raise NotImplementedError
