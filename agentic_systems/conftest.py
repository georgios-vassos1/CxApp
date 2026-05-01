"""Shared pytest fixtures for the agentic_systems course."""

from __future__ import annotations

import json
import sys
from pathlib import Path

import pytest

# Add each module's src/ to the import path so tests can do `from server import ...`
_ROOT = Path(__file__).parent
for module_dir in sorted(_ROOT.glob("M*")):
    src_dir = module_dir / "src"
    if src_dir.is_dir() and str(src_dir) not in sys.path:
        sys.path.insert(0, str(src_dir))


@pytest.fixture
def stations_path() -> Path:
    return _ROOT / "data" / "stations.json"


@pytest.fixture
def stations_data(stations_path: Path) -> dict:
    return json.loads(stations_path.read_text())
