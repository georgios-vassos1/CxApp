# M09 -- A2A Client Adapter

## Learning Objectives

- Build Pydantic request/response models for typed API communication
- Construct JSON-RPC 2.0 envelopes for the Agent-to-Agent (A2A) protocol
- Parse nested JSON response structures with proper error handling
- Use httpx for synchronous HTTP transport with timeout and auth headers
- Mock external HTTP calls in tests using `unittest.mock.patch`

## Key Concepts

| Concept | Description |
|---------|-------------|
| JSON-RPC 2.0 | Stateless RPC protocol; envelope contains jsonrpc, id, method, params |
| A2A Protocol | Agent-to-Agent messaging; uses message/send method with parts-based payloads |
| Context tracking | contextId allows multi-turn conversations across stateless requests |
| httpx | Modern Python HTTP client with timeout support and clean API |

## Files

| File | Purpose |
|------|---------|
| `src/adapter.py` | Stub implementations (raise NotImplementedError) |
| `tests/test_adapter.py` | 12 tests covering models, envelope, parsing, and HTTP |

## Getting Started

```bash
# Run tests (all should fail initially)
poetry run pytest M09_a2a_client_adapter/tests/ -v
```

Implement each stub in `src/adapter.py` until all tests pass.
