"""Tests for Module 6 -- System Prompt Engineering.

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

from prompts import (
    build_system_prompt,
    glossary_section,
    inject_context,
    role_section,
    rules_section,
    validate_prompt,
)


# --- role_section ---


def test_role_section_format():
    result = role_section("data analyst", "weather monitoring")
    assert result == "You are a data analyst for weather monitoring."


# --- glossary_section ---


def test_glossary_section_includes_all_terms():
    terms = {"API": "Application Programming Interface", "LLM": "Large Language Model"}
    result = glossary_section(terms)
    assert "Key concepts" in result
    assert "API" in result
    assert "Application Programming Interface" in result
    assert "LLM" in result
    assert "Large Language Model" in result


# --- rules_section ---


def test_rules_section_formatted():
    rules = ["Always cite sources", "Never hallucinate"]
    result = rules_section(rules)
    assert "Tool call discipline" in result
    assert "- Always cite sources" in result
    assert "- Never hallucinate" in result


# --- build_system_prompt ---


def test_build_system_prompt_contains_all_sections():
    prompt = build_system_prompt(
        role="analyst",
        domain="weather",
        terms={"API": "Interface"},
        rules=["Be precise"],
    )
    assert "You are a analyst for weather." in prompt
    assert "Key concepts" in prompt
    assert "Tool call discipline" in prompt
    assert "API" in prompt
    assert "Be precise" in prompt


# --- validate_prompt ---


def test_validate_prompt_passes_valid():
    prompt = "Key concepts\n---\nTool call discipline\n---\nYou are an analyst."
    errors = validate_prompt(prompt, ["Key concepts", "Tool call discipline"])
    assert errors == []


def test_validate_prompt_missing_section():
    prompt = "Key concepts\n---\nSome content"
    errors = validate_prompt(prompt, ["Key concepts", "Tool call discipline"])
    assert any("Missing section: Tool call discipline" in e for e in errors)


def test_validate_prompt_too_long():
    prompt = "x" * 6000
    errors = validate_prompt(prompt, [], max_length=5000)
    assert any("too long" in e.lower() for e in errors)


# --- inject_context ---


def test_inject_context_replaces_placeholders():
    template = "Hello {user}, run {run_id} started."
    context = {"user": "Alice", "run_id": "R-42"}
    result = inject_context(template, context)
    assert result == "Hello Alice, run R-42 started."


def test_inject_context_missing_key_preserved():
    template = "Hello {user}, session {session_id}."
    context = {"user": "Bob"}
    result = inject_context(template, context)
    assert "{session_id}" in result
    assert "Bob" in result


def test_inject_context_empty_context():
    template = "No placeholders here."
    result = inject_context(template, {})
    assert result == "No placeholders here."
