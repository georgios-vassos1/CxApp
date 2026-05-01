# Agentic Systems — Course Plan

A TDD-driven Python curriculum that builds from protocol fundamentals
through LangGraph agent implementation to Streamlit dashboard integration.

Each module is a standalone directory with `src/` and `tests/`.
Run a single module with:

    poetry run pytest M01_json_rpc/ -v

Run the entire course:

    poetry run pytest -v

---

## Part 1 — Platform Fundamentals

| Module | Topic | Key Concepts |
|--------|-------|--------------|
| M01 | JSON-RPC 2.0 Server | Request/response envelope, method dispatch, error codes, FastAPI mount |
| M02 | Multi-turn Sessions | contextId routing, in-memory session store, session lifecycle |
| M03 | Async Persistence | aiosqlite, checkpoint save/load, lazy init, fallback to in-memory |

---

## Part 2 — Agent Implementation

| Module | Topic | Key Concepts |
|--------|-------|--------------|
| M04 | LangGraph Basics | StateGraph, TypedDict state, nodes, edges, conditional routing, END |
| M05 | Tool Design | @tool decorator, InjectedState, Command for state mutation, ToolNode |
| M06 | System Prompt Engineering | Structured prompts, tool call discipline, reasoning protocols |
| M07 | Pydantic Data Contracts | Input/output models, model_validate, joins, pure analysis functions |
| M08 | Offline Evaluation | MLflow experiment tracking, custom scorers, threshold pass/fail |

---

## Part 3 — Application Integration

| Module | Topic | Key Concepts |
|--------|-------|--------------|
| M09 | A2A Client Adapter | httpx POST, Pydantic request/response, contextId carry-forward |
| M10 | Streamlit Session State | st.session_state, @st.cache_data, scope-keyed caches, hashable args |
| M11 | Two-Phase Submit & Callbacks | on_click callbacks, st.rerun() semantics, chat blocking |
| M12 | Testing Patterns | Mocking session_state, patch.object, polyfactory, TDD for UI state |

---

## Domain

Weather station analysis — local JSON data, no external API dependencies.
Tools: `list_stations`, `get_station_summary`, `get_anomalies`, `compare_stations`.

---

## TDD Workflow

1. `poetry run pytest MNN_topic/ -v` — all non-trivial tests fail (red).
2. Implement each stub function in `src/*.py`.
3. Re-run until all tests pass (green).
4. Move to the next module.

M01 is the **reference implementation** — all tests already pass.
M02–M12 are in the **red phase** — stubs raise `NotImplementedError`.
