"""Tests for Module 14 -- Custom Checkpointer (Preloaded / Zero-I/O Pattern).

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

import json

import pytest

from checkpointer import (
    CheckpointTuple,
    PreloadedCheckpointer,
    create_preloaded,
    decode_checkpoint,
    encode_checkpoint,
)


# --- encode_checkpoint ---


def test_encode_returns_string():
    result = encode_checkpoint({"key": "value"})
    assert isinstance(result, str)


def test_encode_produces_base64():
    blob = encode_checkpoint({"a": 1})
    import base64

    decoded = base64.b64decode(blob)
    assert json.loads(decoded) == {"a": 1}


def test_encode_empty_dict():
    blob = encode_checkpoint({})
    assert isinstance(blob, str)
    assert len(blob) > 0


# --- decode_checkpoint ---


def test_decode_roundtrip():
    original = {"messages": ["hello"], "count": 3}
    blob = encode_checkpoint(original)
    restored = decode_checkpoint(blob)
    assert restored == original


def test_decode_empty_dict():
    blob = encode_checkpoint({})
    assert decode_checkpoint(blob) == {}


def test_decode_nested_data():
    original = {"outer": {"inner": [1, 2, 3]}}
    blob = encode_checkpoint(original)
    assert decode_checkpoint(blob) == original


# --- CheckpointTuple ---


def test_checkpoint_tuple_defaults():
    ct = CheckpointTuple()
    assert ct.config == {}
    assert ct.checkpoint == {}
    assert ct.metadata == {}


def test_checkpoint_tuple_custom_values():
    ct = CheckpointTuple(
        config={"thread_id": "t1"},
        checkpoint={"state": "data"},
        metadata={"version": 1},
    )
    assert ct.config["thread_id"] == "t1"
    assert ct.checkpoint["state"] == "data"
    assert ct.metadata["version"] == 1


# --- PreloadedCheckpointer init ---


async def test_preloaded_no_state():
    cp = PreloadedCheckpointer()
    assert cp.has_preloaded is False


async def test_preloaded_with_state():
    ct = CheckpointTuple(checkpoint={"k": "v"})
    cp = PreloadedCheckpointer(preloaded=ct)
    assert cp.has_preloaded is True


# --- PreloadedCheckpointer.get_tuple ---


async def test_get_tuple_returns_preloaded():
    ct = CheckpointTuple(checkpoint={"data": 42})
    cp = PreloadedCheckpointer(preloaded=ct)
    result = await cp.get_tuple()
    assert result is not None
    assert result.checkpoint == {"data": 42}


async def test_get_tuple_returns_none_when_empty():
    cp = PreloadedCheckpointer()
    result = await cp.get_tuple()
    assert result is None


async def test_get_tuple_serves_once_only():
    ct = CheckpointTuple(checkpoint={"data": 1})
    cp = PreloadedCheckpointer(preloaded=ct)
    first = await cp.get_tuple()
    assert first is not None
    second = await cp.get_tuple()
    assert second is None


# --- PreloadedCheckpointer.put ---


async def test_put_captures_write():
    cp = PreloadedCheckpointer()
    await cp.put({"thread_id": "t1"}, {"state": "new"})
    assert len(cp.pending_writes) == 1


async def test_put_stores_checkpoint_tuple():
    cp = PreloadedCheckpointer()
    await cp.put({"thread_id": "t1"}, {"state": "new"}, {"v": 1})
    write = cp.pending_writes[0]
    assert isinstance(write, CheckpointTuple)
    assert write.config == {"thread_id": "t1"}
    assert write.checkpoint == {"state": "new"}
    assert write.metadata == {"v": 1}


async def test_put_multiple_writes():
    cp = PreloadedCheckpointer()
    await cp.put({}, {"step": 1})
    await cp.put({}, {"step": 2})
    await cp.put({}, {"step": 3})
    assert len(cp.pending_writes) == 3


# --- PreloadedCheckpointer.pending_writes ---


async def test_pending_writes_initially_empty():
    cp = PreloadedCheckpointer()
    assert cp.pending_writes == []


# --- create_preloaded factory ---


def test_factory_none_returns_empty():
    cp = create_preloaded(raw_blob=None)
    assert cp.has_preloaded is False


def test_factory_empty_string_returns_empty():
    cp = create_preloaded(raw_blob="")
    assert cp.has_preloaded is False


async def test_factory_with_blob_returns_preloaded():
    blob = encode_checkpoint({"messages": ["hi"]})
    cp = create_preloaded(raw_blob=blob, config={"thread_id": "t1"})
    assert cp.has_preloaded is True
    result = await cp.get_tuple()
    assert result is not None
    assert result.checkpoint == {"messages": ["hi"]}
    assert result.config == {"thread_id": "t1"}


def test_factory_with_blob_no_config():
    blob = encode_checkpoint({"x": 1})
    cp = create_preloaded(raw_blob=blob)
    assert cp.has_preloaded is True
