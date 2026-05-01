"""Tests for Module 1 — JSON-RPC 2.0 Server.

Tests — all must pass (this is the reference implementation).
"""

from __future__ import annotations

import pytest
from fastapi.testclient import TestClient

from server import (
    Dispatcher,
    JsonRpcError,
    ParsedRequest,
    create_app,
    make_error_response,
    make_response,
    parse_request,
)


# --- parse_request ---


def test_parse_request_valid_envelope():
    req = parse_request({
        "jsonrpc": "2.0",
        "method": "message/send",
        "params": {"text": "hello"},
        "id": "req-1",
    })
    assert isinstance(req, ParsedRequest)
    assert req.method == "message/send"
    assert req.params == {"text": "hello"}
    assert req.id == "req-1"


def test_parse_request_defaults_params_to_empty_dict():
    req = parse_request({"jsonrpc": "2.0", "method": "ping", "id": 1})
    assert req.params == {}


def test_parse_request_accepts_integer_id():
    req = parse_request({"jsonrpc": "2.0", "method": "ping", "id": 42})
    assert req.id == 42


def test_parse_request_missing_jsonrpc_raises():
    with pytest.raises(JsonRpcError) as exc_info:
        parse_request({"method": "ping", "id": "1"})
    assert exc_info.value.code == -32600


def test_parse_request_wrong_jsonrpc_version_raises():
    with pytest.raises(JsonRpcError) as exc_info:
        parse_request({"jsonrpc": "1.0", "method": "ping", "id": "1"})
    assert exc_info.value.code == -32600


def test_parse_request_missing_method_raises():
    with pytest.raises(JsonRpcError) as exc_info:
        parse_request({"jsonrpc": "2.0", "id": "1"})
    assert exc_info.value.code == -32600


def test_parse_request_empty_method_raises():
    with pytest.raises(JsonRpcError) as exc_info:
        parse_request({"jsonrpc": "2.0", "method": "", "id": "1"})
    assert exc_info.value.code == -32600


def test_parse_request_missing_id_raises():
    with pytest.raises(JsonRpcError) as exc_info:
        parse_request({"jsonrpc": "2.0", "method": "ping"})
    assert exc_info.value.code == -32600


def test_parse_request_non_dict_params_raises():
    with pytest.raises(JsonRpcError) as exc_info:
        parse_request({"jsonrpc": "2.0", "method": "ping", "id": "1", "params": [1, 2]})
    assert exc_info.value.code == -32602


# --- make_response ---


def test_make_response_has_correct_shape():
    resp = make_response("req-1", {"status": "ok"})
    assert resp["jsonrpc"] == "2.0"
    assert resp["id"] == "req-1"
    assert resp["result"] == {"status": "ok"}
    assert "error" not in resp


def test_make_response_preserves_integer_id():
    resp = make_response(42, "pong")
    assert resp["id"] == 42


# --- make_error_response ---


def test_make_error_response_has_correct_shape():
    err = JsonRpcError(-32601, "Method not found")
    resp = make_error_response("req-1", err)
    assert resp["jsonrpc"] == "2.0"
    assert resp["id"] == "req-1"
    assert resp["error"]["code"] == -32601
    assert resp["error"]["message"] == "Method not found"
    assert "result" not in resp


def test_make_error_response_includes_data_when_present():
    err = JsonRpcError(-32603, "Internal error", data={"trace": "line 42"})
    resp = make_error_response("req-1", err)
    assert resp["error"]["data"] == {"trace": "line 42"}


def test_make_error_response_omits_data_when_none():
    err = JsonRpcError(-32601, "Not found")
    resp = make_error_response("req-1", err)
    assert "data" not in resp["error"]


def test_make_error_response_none_id():
    err = JsonRpcError(-32700, "Parse error")
    resp = make_error_response(None, err)
    assert resp["id"] is None


# --- Dispatcher ---


def test_dispatcher_routes_to_registered_handler():
    d = Dispatcher()
    d.register("echo", lambda params: params)
    resp = d.dispatch({"jsonrpc": "2.0", "method": "echo", "params": {"x": 1}, "id": "1"})
    assert resp["result"] == {"x": 1}


def test_dispatcher_unknown_method_returns_error():
    d = Dispatcher()
    resp = d.dispatch({"jsonrpc": "2.0", "method": "no_such_method", "id": "1"})
    assert resp["error"]["code"] == -32601


def test_dispatcher_handler_exception_returns_internal_error():
    d = Dispatcher()
    d.register("boom", lambda params: 1 / 0)
    resp = d.dispatch({"jsonrpc": "2.0", "method": "boom", "id": "1"})
    assert resp["error"]["code"] == -32603


def test_dispatcher_invalid_envelope_returns_error():
    d = Dispatcher()
    resp = d.dispatch({"method": "ping", "id": "1"})  # missing jsonrpc
    assert resp["error"]["code"] == -32600


def test_dispatcher_multiple_handlers():
    d = Dispatcher()
    d.register("add", lambda p: {"sum": p["a"] + p["b"]})
    d.register("mul", lambda p: {"product": p["a"] * p["b"]})
    r1 = d.dispatch({"jsonrpc": "2.0", "method": "add", "params": {"a": 2, "b": 3}, "id": "1"})
    r2 = d.dispatch({"jsonrpc": "2.0", "method": "mul", "params": {"a": 4, "b": 5}, "id": "2"})
    assert r1["result"]["sum"] == 5
    assert r2["result"]["product"] == 20


# --- FastAPI app (integration) ---


def test_app_ping_returns_pong():
    client = TestClient(create_app())
    resp = client.post("/", json={"jsonrpc": "2.0", "method": "ping", "id": "1"})
    assert resp.status_code == 200
    body = resp.json()
    assert body["result"] == {"pong": True}


def test_app_invalid_json_returns_parse_error():
    client = TestClient(create_app())
    resp = client.post("/", content=b"not json", headers={"content-type": "application/json"})
    assert resp.status_code == 200
    body = resp.json()
    assert body["error"]["code"] == -32700


def test_app_custom_dispatcher():
    d = Dispatcher()
    d.register("greet", lambda p: {"greeting": f"Hello, {p['name']}!"})
    client = TestClient(create_app(d))
    resp = client.post(
        "/",
        json={"jsonrpc": "2.0", "method": "greet", "params": {"name": "World"}, "id": "1"},
    )
    assert resp.json()["result"]["greeting"] == "Hello, World!"
