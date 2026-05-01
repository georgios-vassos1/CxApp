# M07 -- Pydantic Data Contracts

## Learning Objectives

1. Define strict data models using Pydantic `BaseModel`
2. Validate and parse JSON data with `model_validate`
3. Write pure analysis functions over typed data containers
4. Join and enrich data across related models
5. Implement sorted comparisons with error handling for invalid inputs

## Key Concepts

- **BaseModel**: Pydantic's core class for defining typed, validated data structures
- **model_validate**: Class method to parse dicts/JSON into model instances with full validation
- **Pure functions**: Stateless functions that take typed inputs and return dicts -- easy to test
- **Domain: weather stations**: The shared `data/stations.json` dataset with stations and readings

## TDD Workflow

All stubs in `src/contracts.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M07_pydantic_contracts/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
