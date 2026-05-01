"""Module 9 — A2A Client Adapter

**TDD Red Phase** — all tests fail. Implement each stub.

This module builds a typed client adapter for the Agent-to-Agent (A2A)
protocol. It handles JSON-RPC 2.0 envelope construction, response
parsing, and HTTP transport via httpx.

| Group | Functions / Classes | Concept |
|-------|---------------------|---------|
| Request model | ChatRequest | Pydantic model: text, context_id (optional) |
| Response model | ChatResponse | Pydantic model: text, context_id |
| Envelope builder | build_a2a_envelope | JSON-RPC 2.0 request with message/send method |
| Response parser | parse_a2a_response | Extract text and contextId from response JSON |
| Client function | send_message | httpx POST, build envelope, parse response |
"""

from __future__ import annotations

import uuid  # noqa: F401 — needed for implementation
from typing import Any

import httpx  # noqa: F401 — needed for implementation
from pydantic import BaseModel


# ------------------------------------------------------------------ #
#  1. Request Model                                                   #
# ------------------------------------------------------------------ #


class ChatRequest(BaseModel):
    """A2A chat request.

    TODO: Define two fields:
      - text: str
      - context_id: str | None with default None
    """

    ...


# ------------------------------------------------------------------ #
#  2. Response Model                                                  #
# ------------------------------------------------------------------ #


class ChatResponse(BaseModel):
    """A2A chat response.

    TODO: Define two fields:
      - text: str
      - context_id: str
    """

    ...


# ------------------------------------------------------------------ #
#  3. Envelope Builder                                                #
# ------------------------------------------------------------------ #


def build_a2a_envelope(request: ChatRequest) -> dict[str, Any]:
    """Build a JSON-RPC 2.0 envelope for the A2A message/send method.

    The envelope structure:
      {
        "jsonrpc": "2.0",
        "id": <uuid>,
        "method": "message/send",
        "params": {
          "message": {
            "role": "user",
            "messageId": <uuid>,
            "parts": [{"kind": "text", "text": request.text}]
          }
        }
      }

    TODO: If request.context_id is not None, add "contextId" inside the
    message dict (NOT at params level).
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Response Parser                                                 #
# ------------------------------------------------------------------ #


def parse_a2a_response(data: dict[str, Any]) -> ChatResponse:
    """Parse an A2A JSON-RPC response into a ChatResponse.

    Expected structure:
      data["result"]["status"]["message"]["parts"][0]["text"]  -> text
      data["result"]["contextId"]                               -> context_id

    TODO: Raise ValueError if the structure is malformed (missing keys).
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  5. Client Function                                                 #
# ------------------------------------------------------------------ #


def send_message(
    url: str,
    request: ChatRequest,
    api_key: str | None = None,
) -> ChatResponse:
    """Send a chat message to an A2A agent endpoint.

    TODO:
      - Build the envelope with build_a2a_envelope.
      - POST it to url using httpx.post with timeout=120.
      - Add Authorization: Bearer <api_key> header if api_key is provided.
      - Parse the response JSON with parse_a2a_response.
      - Return the ChatResponse.
    """
    raise NotImplementedError
