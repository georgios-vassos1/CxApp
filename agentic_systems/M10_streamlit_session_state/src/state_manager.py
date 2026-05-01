"""Module 10 — Streamlit Session State

**TDD Red Phase** — all tests fail. Implement each stub.

This module teaches Streamlit session state patterns WITHOUT importing
streamlit. It uses a plain dict as a stand-in for st.session_state so
tests can run without a Streamlit server.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Scope model | Scope | Dataclass: region, terminals, start_date, end_date |
| Scope-keyed cache | is_cached, get_cached, set_cached | Check/get/set files under a scope key |
| Scope clearing | clear_on_scope_change | Wipe cache when scope changes |
| Cache data helper | make_hashable | Convert lists to tuples for cache keys |
"""

from __future__ import annotations

from dataclasses import dataclass
from datetime import date
from typing import Any


# ------------------------------------------------------------------ #
#  1. Scope Model                                                     #
# ------------------------------------------------------------------ #


@dataclass(frozen=True)
class Scope:
    """Immutable scope that defines a parameter context for cached data.

    Fields are defined so the dataclass can be constructed in tests.
    The functions that operate on Scope are the stubs to implement.
    """

    region: str
    terminals: tuple[str, ...]
    start_date: date
    end_date: date


# ------------------------------------------------------------------ #
#  2. Scope-Keyed Cache                                               #
# ------------------------------------------------------------------ #


def is_cached(session: dict, file_key: str, scope: Scope) -> bool:
    """Check if a file is cached under the current scope.

    TODO: Return True only if:
      - session["generated_files"][file_key] exists, AND
      - session["files_scope"] == scope
    """
    raise NotImplementedError


def get_cached(session: dict, file_key: str) -> bytes | None:
    """Retrieve cached file data.

    TODO: Return session["generated_files"][file_key] if present, else None.
    """
    raise NotImplementedError


def set_cached(
    session: dict, file_key: str, data: bytes, scope: Scope
) -> None:
    """Store file data in the cache and update the scope.

    TODO:
      - Store data in session["generated_files"][file_key].
      - Set session["files_scope"] = scope.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. Scope Clearing                                                  #
# ------------------------------------------------------------------ #


def clear_on_scope_change(session: dict, current_scope: Scope) -> bool:
    """Wipe cached files if the scope has changed.

    TODO:
      - If session["files_scope"] != current_scope:
        - Delete "generated_files" and "optimized_files" keys from session.
        - Return True.
      - Otherwise return False.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Cache Data Helper                                               #
# ------------------------------------------------------------------ #


def make_hashable(value: Any) -> Any:
    """Recursively convert mutable containers to hashable equivalents.

    TODO:
      - list -> tuple (recursively)
      - dict -> frozenset of (key, make_hashable(value)) pairs
      - everything else -> passthrough
    """
    raise NotImplementedError
