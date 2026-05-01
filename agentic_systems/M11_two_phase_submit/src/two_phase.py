"""Module 11 — Two-Phase Submit & Callbacks

**TDD Red Phase** — all tests fail. Implement each stub.

This module models the Streamlit two-phase submit pattern using plain
dicts as session state stand-ins. Phase 1 captures user input and sets
flags; Phase 2 calls the agent and records the response.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Chat model | ChatMessage, ChatSession | role/text, history/context_id |
| Phase 1 | start_chat_turn | Append user message, set processing flag |
| Phase 2 | complete_chat_turn | Call agent callback, append response |
| Toggle | toggle_panel | Flip a boolean flag in session |
| Guard | is_processing | Check if a turn is in progress |
"""

from __future__ import annotations

from dataclasses import dataclass, field  # noqa: F401 — needed for implementation
from typing import Callable, Literal  # noqa: F401 — needed for implementation


# ------------------------------------------------------------------ #
#  1. Chat Models                                                     #
# ------------------------------------------------------------------ #


@dataclass
class ChatMessage:
    """A single chat message.

    TODO: Define two fields:
      - role: Literal["user", "agent"]
      - text: str
    """

    ...


@dataclass
class ChatSession:
    """A chat session with history and optional context tracking.

    TODO: Define two fields:
      - history: list[ChatMessage]  (default_factory=list)
      - context_id: str | None      (default None)
    """

    ...


# ------------------------------------------------------------------ #
#  2. Phase 1 — Start Chat Turn                                       #
# ------------------------------------------------------------------ #


def start_chat_turn(session: dict, prompt: str, run_id: str) -> None:
    """Capture user input and prepare for agent callback.

    TODO:
      - Get or create ChatSession in session["chat_sessions"][run_id].
      - Append ChatMessage(role="user", text=prompt) to the session history.
      - Set session["chat_processing"] = True.
      - Set session["chat_pending"] = {"prompt": prompt, "run_id": run_id}.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. Phase 2 — Complete Chat Turn                                    #
# ------------------------------------------------------------------ #


def complete_chat_turn(
    session: dict,
    agent_fn: Callable[[str, str | None], tuple[str, str]],
) -> None:
    """Execute the agent callback and record the response.

    TODO:
      - Read pending from session["chat_pending"].
      - Get the ChatSession for pending["run_id"].
      - Call agent_fn(prompt, chat_session.context_id) -> (response_text, new_context_id).
      - Append ChatMessage(role="agent", text=response_text) to history.
      - Update chat_session.context_id = new_context_id.
      - Set session["chat_processing"] = False.
      - Delete session["chat_pending"].
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Toggle Panel                                                    #
# ------------------------------------------------------------------ #


def toggle_panel(session: dict, key: str = "panel_open") -> None:
    """Flip a boolean flag in session state.

    TODO: Set session[key] = not session.get(key, False).
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  5. Processing Guard                                                #
# ------------------------------------------------------------------ #


def is_processing(session: dict) -> bool:
    """Check whether a chat turn is currently in progress.

    TODO: Return session.get("chat_processing", False).
    """
    raise NotImplementedError
