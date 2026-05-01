"""Tests for Module 12 — Testing Patterns

Every test should FAIL until the stubs in test_helpers.py are implemented.
"""

from __future__ import annotations

import pytest

from test_helpers import (
    ReadingFactory,
    StationFactory,
    assert_session_has,
    assert_session_missing,
    make_session,
)


# --- make_session --- #


def test_make_session_defaults():
    session = make_session()
    assert session["chat_sessions"] == {}
    assert session["chat_processing"] is False
    assert session["files_scope"] is None
    assert session["generated_files"] == {}


def test_make_session_overrides():
    session = make_session({"chat_processing": True, "extra_key": 42})
    assert session["chat_processing"] is True
    assert session["extra_key"] == 42
    # Defaults still present
    assert session["chat_sessions"] == {}


# --- StationFactory --- #


def test_station_factory_generates_valid():
    station = StationFactory.build()
    assert station.station_id is not None
    assert station.name is not None
    assert station.region is not None
    assert isinstance(station.elevation_m, int)


def test_station_factory_id_format():
    station = StationFactory.build()
    assert station.station_id.startswith("WS")
    assert len(station.station_id) == 5  # "WS" + 3 digits


# --- ReadingFactory --- #


def test_reading_factory_generates_valid():
    reading = ReadingFactory.build()
    assert reading.station_id is not None
    assert reading.timestamp is not None
    assert isinstance(reading.temperature_c, float)
    assert isinstance(reading.humidity_pct, int)
    assert isinstance(reading.wind_speed_kmh, float)


def test_reading_factory_temperature_in_range():
    # Generate several and check bounds
    for _ in range(20):
        reading = ReadingFactory.build()
        assert -20 <= reading.temperature_c <= 45, (
            f"temperature_c={reading.temperature_c} out of [-20, 45] range"
        )


# --- assert_session_has --- #


def test_assert_session_has_present():
    session = {"key": "value"}
    assert_session_has(session, "key")  # Should not raise


def test_assert_session_has_with_value():
    session = {"key": 42}
    assert_session_has(session, "key", 42)  # Should not raise


def test_assert_session_has_wrong_value_raises():
    session = {"key": 42}
    with pytest.raises(AssertionError):
        assert_session_has(session, "key", 99)


# --- assert_session_missing --- #


def test_assert_session_missing_absent():
    session = {"other": 1}
    assert_session_missing(session, "key")  # Should not raise


def test_assert_session_missing_present_raises():
    session = {"key": "exists"}
    with pytest.raises(AssertionError):
        assert_session_missing(session, "key")
