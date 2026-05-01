# M06 -- System Prompt Engineering

## Learning Objectives

1. Build composable prompt sections (role, glossary, rules)
2. Assemble sections into a complete system prompt
3. Validate prompts for required sections and length constraints
4. Inject runtime context into prompt templates without breaking unresolved placeholders

## Key Concepts

- **Section templates**: Small functions that produce standardised prompt blocks
- **Prompt composition**: Concatenating sections with consistent formatting
- **Validation**: Programmatic checks that a prompt meets structural requirements before use
- **Context injection**: Safe placeholder replacement that preserves unresolved `{keys}`

## TDD Workflow

All stubs in `src/prompts.py` raise `NotImplementedError`. Run tests with:

```bash
pytest M06_system_prompt/tests/ -v
```

Implement each function until all tests pass (Red -> Green -> Refactor).
