"""Module 8 -- Offline Evaluation

| Group | Functions | Concept |
|-------|-----------|---------|
| Scorer protocol | Scorer (Protocol) | Callable with (response, expected) -> float |
| Built-in scorers | exact_fact_scorer, completeness_scorer, no_hedge_scorer | Pattern matching, substring checks |
| Threshold wrapper | threshold_scorer | Wraps a scorer, returns pass/fail based on threshold |
| Eval runner | run_evaluation | Run all scorers on a list of samples |
| Result model | EvalResult | sample_id, scorer_name, score, passed, rationale |

**TDD Red Phase** -- all tests fail. Implement each stub.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Callable, Protocol


# ------------------------------------------------------------------ #
#  1. Result Model                                                    #
# ------------------------------------------------------------------ #


@dataclass
class EvalResult:
    """Result of running a single scorer on a single sample.

    TODO: Define fields:
    - sample_id: str
    - scorer_name: str
    - score: float
    - passed: bool
    - rationale: str
    """

    pass  # TODO: replace with field definitions


# ------------------------------------------------------------------ #
#  2. Scorer Protocol                                                 #
# ------------------------------------------------------------------ #


class Scorer(Protocol):
    """Protocol for scorer callables."""

    def __call__(self, response: str, expected: dict) -> float: ...


# ------------------------------------------------------------------ #
#  3. Built-in Scorers                                                #
# ------------------------------------------------------------------ #


def exact_fact_scorer(response: str, expected: dict) -> float:
    """Score based on exact fact presence in the response.

    TODO: ``expected["expected_facts"]`` is a dict of key:value pairs.
    For each pair, check if *value* appears in *response* (word-boundary
    or simple substring match). Return the fraction found.
    """
    raise NotImplementedError


def completeness_scorer(response: str, expected: dict) -> float:
    """Score based on completeness of expected items in the response.

    TODO: ``expected["expected_items"]`` is a list of strings.
    Return the fraction of items found in *response*
    (case-insensitive substring match).
    """
    raise NotImplementedError


def no_hedge_scorer(response: str, expected: dict) -> float:
    """Score based on absence of hedging language.

    TODO: ``expected["must_not_contain"]`` is a list of strings.
    Return 1.0 if none are found in *response*, 0.0 if any are found.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Threshold Wrapper                                               #
# ------------------------------------------------------------------ #


def threshold_scorer(
    scorer: Callable[[str, dict], float],
    threshold: float,
) -> Callable[[str, dict], tuple[float, bool]]:
    """Wrap a scorer to also return a pass/fail boolean.

    TODO: Return a new callable that calls *scorer* and returns
    ``(score, score >= threshold)``.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  5. Evaluation Runner                                               #
# ------------------------------------------------------------------ #


def run_evaluation(
    samples: list[dict],
    scorers: list[tuple[str, Callable[[str, dict], float], float]],
) -> list[EvalResult]:
    """Run all scorers on all samples and collect results.

    TODO: For each sample (has ``"sample_id"``, ``"response"``,
    ``"expected"``), run each scorer (tuple of name, fn, threshold)
    and produce an ``EvalResult`` per scorer-sample pair.
    """
    raise NotImplementedError
