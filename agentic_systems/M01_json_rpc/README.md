# M01 — JSON-RPC 2.0 Server

**Reference implementation** — all tests pass. Read before starting M02.

## Learning Objectives

- Understand the JSON-RPC 2.0 request/response envelope structure
- Implement a method dispatcher that routes by method string
- Build standard error responses with numeric codes
- Mount a JSON-RPC endpoint on a FastAPI application

## Run

```bash
poetry run pytest M01_json_rpc/ -v
```
