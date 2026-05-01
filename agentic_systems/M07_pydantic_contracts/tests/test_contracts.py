"""Tests for Module 7 -- Pydantic Data Contracts.

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

import pytest

from contracts import (
    Reading,
    Station,
    WeatherData,
    compare_stations,
    enrich_readings,
    station_summary,
)


# --- Station model ---


def test_station_model_valid():
    s = Station(station_id="WS001", name="Alpine Summit", region="mountain", elevation_m=2450)
    assert s.station_id == "WS001"
    assert s.name == "Alpine Summit"
    assert s.region == "mountain"
    assert s.elevation_m == 2450


# --- Reading model ---


def test_reading_model_valid():
    r = Reading(
        station_id="WS001",
        timestamp="2025-01-15T06:00:00Z",
        temperature_c=-8.2,
        humidity_pct=42,
        wind_speed_kmh=65.0,
    )
    assert r.station_id == "WS001"
    assert r.temperature_c == -8.2


# --- WeatherData from JSON ---


def test_weather_data_from_json(stations_data):
    wd = WeatherData.model_validate(stations_data)
    assert len(wd.stations) == 6
    assert len(wd.readings) == 36


# --- station_summary ---


def test_station_summary_counts(stations_data):
    wd = WeatherData.model_validate(stations_data)
    summary = station_summary(wd, "WS001")
    assert summary["station_id"] == "WS001"
    assert summary["reading_count"] == 6


def test_station_summary_temperature_stats(stations_data):
    wd = WeatherData.model_validate(stations_data)
    summary = station_summary(wd, "WS001")
    assert summary["min_temperature"] == -12.3
    assert summary["max_temperature"] == -5.1
    assert isinstance(summary["avg_temperature"], float)


def test_station_summary_humidity(stations_data):
    wd = WeatherData.model_validate(stations_data)
    summary = station_summary(wd, "WS001")
    assert isinstance(summary["avg_humidity"], float)


# --- enrich_readings ---


def test_enrich_readings_has_name_and_region(stations_data):
    wd = WeatherData.model_validate(stations_data)
    enriched = enrich_readings(wd)
    for record in enriched:
        assert "name" in record
        assert "region" in record


def test_enrich_readings_count_matches(stations_data):
    wd = WeatherData.model_validate(stations_data)
    enriched = enrich_readings(wd)
    assert len(enriched) == 36


# --- compare_stations ---


def test_compare_stations_temperature_sorted(stations_data):
    wd = WeatherData.model_validate(stations_data)
    result = compare_stations(wd, "temperature")
    values = [r["value"] for r in result]
    assert values == sorted(values, reverse=True)


def test_compare_stations_all_stations_present(stations_data):
    wd = WeatherData.model_validate(stations_data)
    result = compare_stations(wd, "temperature")
    station_ids = {r["station_id"] for r in result}
    assert station_ids == {"WS001", "WS002", "WS003", "WS004", "WS005", "WS006"}


def test_compare_stations_unknown_metric_raises(stations_data):
    wd = WeatherData.model_validate(stations_data)
    with pytest.raises(ValueError):
        compare_stations(wd, "pressure")
