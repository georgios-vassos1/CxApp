"""Module 1 — JSON-RPC 2.0 Server

**Reference implementation** — all tests pass.

This module builds a minimal JSON-RPC 2.0 dispatcher on top of FastAPI.
The dispatcher validates request envelopes, routes method calls to registered
handlers, and formats responses and errors according to the JSON-RPC 2.0
specification.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Error model | JsonRpcError | Typed exception with code + message + optional data |
| Request model | ParsedRequest | Validated envelope: jsonrpc, method, params, id |
| Envelope parsing | parse_request | JSON-RPC 2.0 request validation |
| Response builders | make_response, make_error_response | Spec-compliant result and error envelopes |
| Method dispatch | Dispatcher | Register handlers, route by method string |
| FastAPI app | create_app | Mount the dispatcher on a POST endpoint |
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Callable

from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse


# ------------------------------------------------------------------ #
#  1. Error Model                                                     #
# ------------------------------------------------------------------ #


class JsonRpcError(Exception):
    """A JSON-RPC 2.0 error with a numeric code and human-readable message.

    Standard codes:
      -32700  Parse error
      -32600  Invalid Request
      -32601  Method not found
      -32602  Invalid params
      -32603  Internal error
    """

    def __init__(self, code: int, message: str, data: Any = None) -> None:
        super().__init__(message)
        self.code = code
        self.message = message
        self.data = data


# ------------------------------------------------------------------ #
#  2. Request Model                                                   #
# ------------------------------------------------------------------ #


@dataclass
class ParsedRequest:
    """A validated JSON-RPC 2.0 request envelope."""

    method: str
    params: dict[str, Any]
    id: str | int


# ------------------------------------------------------------------ #
#  3. Envelope Parsing                                                #
# ------------------------------------------------------------------ #


def parse_request(raw: dict[str, Any]) -> ParsedRequest:
    """Parse and validate a JSON-RPC 2.0 request envelope.

    Checks:
      - ``jsonrpc`` field is exactly ``"2.0"``.
      - ``method`` field is a non-empty string.
      - ``id`` field is present (string or int).
      - ``params`` defaults to ``{}`` when absent.

    Raises JsonRpcError(-32600) for invalid requests.
    """
    if raw.get("jsonrpc") != "2.0":
        raise JsonRpcError(-32600, "Invalid Request: missing or wrong jsonrpc version")

    method = raw.get("method")
    if not isinstance(method, str) or not method:
        raise JsonRpcError(-32600, "Invalid Request: method must be a non-empty string")

    request_id = raw.get("id")
    if request_id is None:
        raise JsonRpcError(-32600, "Invalid Request: id is required")

    params = raw.get("params", {})
    if not isinstance(params, dict):
        raise JsonRpcError(-32602, "Invalid params: params must be an object")

    return ParsedRequest(method=method, params=params, id=request_id)


# ------------------------------------------------------------------ #
#  4. Response Builders                                               #
# ------------------------------------------------------------------ #


def make_response(request_id: str | int, result: Any) -> dict[str, Any]:
    """Build a JSON-RPC 2.0 success response envelope.

    The envelope contains exactly three keys: ``jsonrpc``, ``id``, ``result``.
    """
    return {
        "jsonrpc": "2.0",
        "id": request_id,
        "result": result,
    }


def make_error_response(
    request_id: str | int | None,
    error: JsonRpcError,
) -> dict[str, Any]:
    """Build a JSON-RPC 2.0 error response envelope.

    The error object contains ``code``, ``message``, and optionally ``data``.
    """
    err: dict[str, Any] = {"code": error.code, "message": error.message}
    if error.data is not None:
        err["data"] = error.data
    return {
        "jsonrpc": "2.0",
        "id": request_id,
        "error": err,
    }


# ------------------------------------------------------------------ #
#  5. Method Dispatch                                                 #
# ------------------------------------------------------------------ #


@dataclass
class Dispatcher:
    """A JSON-RPC method dispatcher.

    Register named handlers with ``register()``, then call ``dispatch()``
    with a raw request dict. The dispatcher parses the envelope, finds the
    handler, calls it with ``params``, and returns a response envelope.
    """

    _handlers: dict[str, Callable] = field(default_factory=dict)

    def register(self, method: str, handler: Callable) -> None:
        """Register a handler function for the given method name."""
        self._handlers[method] = handler

    def dispatch(self, raw: dict[str, Any]) -> dict[str, Any]:
        """Parse, route, and return a JSON-RPC 2.0 response.

        Returns an error response for invalid requests or unknown methods.
        Handler exceptions are caught and returned as internal errors.
        """
        request_id = raw.get("id")
        try:
            req = parse_request(raw)
            handler = self._handlers.get(req.method)
            if handler is None:
                raise JsonRpcError(-32601, f"Method not found: {req.method}")
            result = handler(req.params)
            return make_response(req.id, result)
        except JsonRpcError as e:
            return make_error_response(request_id, e)
        except Exception as e:
            return make_error_response(
                request_id,
                JsonRpcError(-32603, f"Internal error: {e}"),
            )


# ------------------------------------------------------------------ #
#  6. FastAPI App                                                     #
# ------------------------------------------------------------------ #


def create_app(dispatcher: Dispatcher | None = None) -> FastAPI:
    """Create a FastAPI application that serves JSON-RPC 2.0 on POST /.

    If no dispatcher is provided, a default one is created with a
    ``ping`` method that echoes back ``{"pong": true}``.
    """
    if dispatcher is None:
        dispatcher = Dispatcher()
        dispatcher.register("ping", lambda params: {"pong": True})

    app = FastAPI(title="JSON-RPC 2.0 Server")

    @app.post("/")
    async def jsonrpc_endpoint(request: Request) -> JSONResponse:
        try:
            body = await request.json()
        except Exception:
            error = JsonRpcError(-32700, "Parse error: invalid JSON")
            return JSONResponse(make_error_response(None, error))
        response = dispatcher.dispatch(body)
        return JSONResponse(response)

    return app
