# M08 -- Offline Evaluation

## Learning Objectives

1. Define a scorer protocol for consistent evaluation interfaces
2. Build pattern-matching scorers (exact fact, completeness, hedge detection)
3. Wrap scorers with threshold logic for pass/fail decisions
4. Run batch evaluation across multiple samples and scorers
5. Collect structured results using a dataclass

## Key Concepts

- **Scorer protocol**: A callable `(response, expected) -> float` that scores LLM output
- **Exact fact scorer**: Checks if specific key-value facts appear in the response
- **Completeness scorer**: Measures what fraction of expected items are mentioned
- **No-hedge scorer**: Binary check that hedging language is absent
- **Threshold wrapper**: Higher-order function that adds pass/fail semantics to any scorer
- **EvalResult**: Structured output capturing score, pass status, and rationale per sample

## TDD Workflow

All stubs in `src/evaluation.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M08_offline_evaluation/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
