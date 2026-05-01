"""Tests for Module 8 -- Offline Evaluation.

**TDD Red Phase** -- all tests fail until stubs are implemented.
"""

from __future__ import annotations

from evaluation import (
    EvalResult,
    completeness_scorer,
    exact_fact_scorer,
    no_hedge_scorer,
    run_evaluation,
    threshold_scorer,
)


# --- exact_fact_scorer ---


def test_exact_fact_all_present():
    response = "The capital of France is Paris and the population is 67 million."
    expected = {"expected_facts": {"capital": "Paris", "population": "67 million"}}
    assert exact_fact_scorer(response, expected) == 1.0


def test_exact_fact_partial():
    response = "The capital of France is Paris."
    expected = {"expected_facts": {"capital": "Paris", "population": "67 million"}}
    assert exact_fact_scorer(response, expected) == 0.5


def test_exact_fact_none():
    response = "I have no idea about this topic."
    expected = {"expected_facts": {"capital": "Paris", "currency": "Euro"}}
    assert exact_fact_scorer(response, expected) == 0.0


# --- completeness_scorer ---


def test_completeness_all_items():
    response = "We offer apples, bananas, and cherries."
    expected = {"expected_items": ["apples", "bananas", "cherries"]}
    assert completeness_scorer(response, expected) == 1.0


def test_completeness_partial():
    response = "We offer apples and cherries."
    expected = {"expected_items": ["apples", "bananas", "cherries"]}
    score = completeness_scorer(response, expected)
    assert abs(score - 2 / 3) < 1e-9


def test_completeness_empty():
    response = "Nothing relevant here."
    expected = {"expected_items": ["apples", "bananas"]}
    assert completeness_scorer(response, expected) == 0.0


# --- no_hedge_scorer ---


def test_no_hedge_clean():
    response = "The temperature will be 25 degrees."
    expected = {"must_not_contain": ["maybe", "possibly", "I think"]}
    assert no_hedge_scorer(response, expected) == 1.0


def test_no_hedge_found():
    response = "I think the temperature will possibly be around 25 degrees."
    expected = {"must_not_contain": ["maybe", "possibly", "I think"]}
    assert no_hedge_scorer(response, expected) == 0.0


# --- threshold_scorer ---


def test_threshold_pass():
    wrapped = threshold_scorer(exact_fact_scorer, 0.5)
    score, passed = wrapped(
        "Paris is the capital.",
        {"expected_facts": {"capital": "Paris"}},
    )
    assert score == 1.0
    assert passed is True


def test_threshold_fail():
    wrapped = threshold_scorer(exact_fact_scorer, 0.8)
    score, passed = wrapped(
        "Something unrelated.",
        {"expected_facts": {"capital": "Paris", "currency": "Euro"}},
    )
    assert score == 0.0
    assert passed is False


# --- run_evaluation ---


def test_run_evaluation_all_pass():
    samples = [
        {
            "sample_id": "s1",
            "response": "Paris is the capital of France.",
            "expected": {"expected_facts": {"capital": "Paris"}},
        },
    ]
    scorers = [("exact_fact", exact_fact_scorer, 0.5)]
    results = run_evaluation(samples, scorers)
    assert len(results) == 1
    assert isinstance(results[0], EvalResult)
    assert results[0].passed is True
    assert results[0].score == 1.0


def test_run_evaluation_mixed():
    samples = [
        {
            "sample_id": "s1",
            "response": "Paris is great.",
            "expected": {
                "expected_facts": {"capital": "Paris", "currency": "Euro"},
            },
        },
        {
            "sample_id": "s2",
            "response": "Euro is used in France.",
            "expected": {
                "expected_facts": {"capital": "Paris", "currency": "Euro"},
            },
        },
    ]
    scorers = [("exact_fact", exact_fact_scorer, 0.8)]
    results = run_evaluation(samples, scorers)
    assert len(results) == 2
    # s1: only Paris found -> 0.5 < 0.8 -> fail
    assert results[0].passed is False
    # s2: only Euro found -> 0.5 < 0.8 -> fail
    assert results[1].passed is False
