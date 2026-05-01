"""Tests for Module 9 — A2A Client Adapter

Every test should FAIL until the stubs in adapter.py are implemented.
"""

from __future__ import annotations

from unittest.mock import MagicMock, patch

import pytest

from adapter import (
    ChatRequest,
    ChatResponse,
    build_a2a_envelope,
    parse_a2a_response,
    send_message,
)


# --- ChatRequest model --- #


def test_chat_request_model():
    req = ChatRequest(text="hello")
    assert req.text == "hello"
    assert req.context_id is None


def test_chat_request_model_with_context():
    req = ChatRequest(text="hi", context_id="ctx-1")
    assert req.context_id == "ctx-1"


# --- ChatResponse model --- #


def test_chat_response_model():
    resp = ChatResponse(text="world", context_id="ctx-2")
    assert resp.text == "world"
    assert resp.context_id == "ctx-2"


# --- build_a2a_envelope --- #


def test_build_envelope_has_jsonrpc():
    req = ChatRequest(text="ping")
    env = build_a2a_envelope(req)
    assert env["jsonrpc"] == "2.0"
    assert env["method"] == "message/send"
    assert "id" in env


def test_build_envelope_first_turn_no_context_id():
    req = ChatRequest(text="first turn")
    env = build_a2a_envelope(req)
    message = env["params"]["message"]
    assert "contextId" not in message


def test_build_envelope_follow_up_has_context_id():
    req = ChatRequest(text="follow up", context_id="ctx-99")
    env = build_a2a_envelope(req)
    message = env["params"]["message"]
    assert message["contextId"] == "ctx-99"


def test_build_envelope_context_id_inside_message_not_params():
    req = ChatRequest(text="check placement", context_id="ctx-50")
    env = build_a2a_envelope(req)
    assert "contextId" not in env["params"]
    assert env["params"]["message"]["contextId"] == "ctx-50"


def test_build_envelope_parts_structure():
    req = ChatRequest(text="hello agent")
    env = build_a2a_envelope(req)
    parts = env["params"]["message"]["parts"]
    assert len(parts) == 1
    assert parts[0]["kind"] == "text"
    assert parts[0]["text"] == "hello agent"


def test_build_envelope_message_role():
    req = ChatRequest(text="test")
    env = build_a2a_envelope(req)
    assert env["params"]["message"]["role"] == "user"


# --- parse_a2a_response --- #


def _make_a2a_response(text: str = "reply", context_id: str = "ctx-1") -> dict:
    """Helper to build a valid A2A response dict."""
    return {
        "jsonrpc": "2.0",
        "id": "abc",
        "result": {
            "contextId": context_id,
            "status": {
                "message": {
                    "role": "agent",
                    "parts": [{"kind": "text", "text": text}],
                }
            },
        },
    }


def test_parse_response_valid():
    data = _make_a2a_response("hello back", "ctx-42")
    resp = parse_a2a_response(data)
    assert resp.text == "hello back"
    assert resp.context_id == "ctx-42"


def test_parse_response_missing_parts_raises():
    data = {"result": {"contextId": "ctx-1", "status": {"message": {}}}}
    with pytest.raises((ValueError, KeyError, TypeError)):
        parse_a2a_response(data)


# --- send_message --- #


def test_send_message_integration():
    """Mock httpx.post and verify end-to-end flow."""
    mock_response = MagicMock()
    mock_response.json.return_value = _make_a2a_response("agent says hi", "ctx-new")
    mock_response.raise_for_status = MagicMock()

    with patch("adapter.httpx.post", return_value=mock_response) as mock_post:
        req = ChatRequest(text="hello")
        resp = send_message("http://localhost:8000", req, api_key="key-123")

    assert resp.text == "agent says hi"
    assert resp.context_id == "ctx-new"

    # Verify the call was made correctly
    call_args = mock_post.call_args
    assert call_args[0][0] == "http://localhost:8000"
    assert "Authorization" in call_args[1].get("headers", {})


def test_send_message_no_api_key():
    """Without api_key, no Authorization header should be set."""
    mock_response = MagicMock()
    mock_response.json.return_value = _make_a2a_response("ok", "ctx-0")
    mock_response.raise_for_status = MagicMock()

    with patch("adapter.httpx.post", return_value=mock_response) as mock_post:
        req = ChatRequest(text="no auth")
        resp = send_message("http://localhost:8000", req)

    assert resp.text == "ok"
    call_args = mock_post.call_args
    headers = call_args[1].get("headers", {})
    assert "Authorization" not in headers
