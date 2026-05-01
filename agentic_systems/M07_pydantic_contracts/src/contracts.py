"""Module 7 -- Pydantic Data Contracts

| Group | Functions | Concept |
|-------|-----------|---------|
| Input models | Station, Reading | Pydantic BaseModel with typed fields |
| Container model | WeatherData | stations list + readings list, model_validate |
| Analysis functions | station_summary | Pure function: filter readings, compute stats |
| Join operation | enrich_readings | Join readings with station metadata |
| Comparison | compare_stations | Cross-station comparison by metric |

**TDD Red Phase** -- all tests fail. Implement each stub.
"""

from __future__ import annotations

from datetime import datetime  # noqa: F401 — needed for implementation

from pydantic import BaseModel


# ------------------------------------------------------------------ #
#  1. Input Models                                                    #
# ------------------------------------------------------------------ #


class Station(BaseModel):
    """A weather station.

    TODO: Define fields:
    - station_id: str
    - name: str
    - region: str
    - elevation_m: int
    """

    pass  # TODO: replace with field definitions


class Reading(BaseModel):
    """A single weather reading from a station.

    TODO: Define fields:
    - station_id: str
    - timestamp: datetime
    - temperature_c: float
    - humidity_pct: int
    - wind_speed_kmh: float
    """

    pass  # TODO: replace with field definitions


# ------------------------------------------------------------------ #
#  2. Container Model                                                 #
# ------------------------------------------------------------------ #


class WeatherData(BaseModel):
    """Container holding stations and their readings.

    TODO: Define fields:
    - stations: list[Station]
    - readings: list[Reading]
    """

    pass  # TODO: replace with field definitions


# ------------------------------------------------------------------ #
#  3. Analysis Functions                                              #
# ------------------------------------------------------------------ #


def station_summary(data: WeatherData, station_id: str) -> dict:
    """Compute summary statistics for a single station.

    TODO: Filter readings for the given station_id and return:
    {
        "station_id": str,
        "reading_count": int,
        "avg_temperature": float,
        "min_temperature": float,
        "max_temperature": float,
        "avg_humidity": float,
    }
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Join Operation                                                  #
# ------------------------------------------------------------------ #


def enrich_readings(data: WeatherData) -> list[dict]:
    """Join each reading with its station's name and region.

    TODO: For each reading, produce a dict with all reading fields plus
    ``"name"`` and ``"region"`` from the matching station.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  5. Cross-Station Comparison                                        #
# ------------------------------------------------------------------ #


def compare_stations(data: WeatherData, metric: str) -> list[dict]:
    """Compare stations by average of a given metric.

    TODO: Supported metrics: ``"temperature"``, ``"humidity"``,
    ``"wind_speed"``. For each station compute the average of the
    corresponding reading field and return a list of
    ``{"station_id", "name", "value"}`` dicts sorted by value descending.

    Raise ``ValueError`` for unknown metrics.
    """
    raise NotImplementedError
