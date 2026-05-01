"""Module 6 -- System Prompt Engineering

| Group | Functions | Concept |
|-------|-----------|---------|
| Prompt builder | build_system_prompt | Compose sections: role, glossary, rules |
| Section templates | role_section, glossary_section, rules_section | Parameterised prompt parts |
| Prompt validation | validate_prompt | Check required sections present, length bounds |
| Prompt injection | inject_context | Insert runtime context into template |

**TDD Red Phase** -- all tests fail. Implement each stub.
"""

from __future__ import annotations


# ------------------------------------------------------------------ #
#  1. Section Templates                                               #
# ------------------------------------------------------------------ #


def role_section(role: str, domain: str) -> str:
    """Build the role preamble.

    TODO: Return ``"You are a {role} for {domain}."``.
    """
    raise NotImplementedError


def glossary_section(terms: dict[str, str]) -> str:
    """Build a glossary block from a term -> definition mapping.

    TODO: Return a formatted block:
        Key concepts
        ---
        term1  definition1
        term2  definition2
        ...
    """
    raise NotImplementedError


def rules_section(rules: list[str]) -> str:
    """Build a rules block from a list of rule strings.

    TODO: Return a formatted block:
        Tool call discipline
        ---
        - rule1
        - rule2
        ...
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  2. Prompt Builder                                                  #
# ------------------------------------------------------------------ #


def build_system_prompt(
    role: str,
    domain: str,
    terms: dict[str, str],
    rules: list[str],
) -> str:
    """Concatenate all prompt sections into a single system prompt.

    TODO: Combine role_section, glossary_section, and rules_section
    with double newlines between them.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  3. Prompt Validation                                               #
# ------------------------------------------------------------------ #


def validate_prompt(
    prompt: str,
    required_sections: list[str],
    max_length: int = 5000,
) -> list[str]:
    """Validate that a prompt contains required sections and is within length.

    TODO: Return a list of error strings (empty list = valid).
    - For each missing section, add ``"Missing section: {section}"``.
    - If prompt length exceeds max_length, add
      ``"Prompt too long: {len} > {max_length}"``.
    """
    raise NotImplementedError


# ------------------------------------------------------------------ #
#  4. Context Injection                                               #
# ------------------------------------------------------------------ #


def inject_context(template: str, context: dict[str, str]) -> str:
    """Replace ``{key}`` placeholders in template with context values.

    TODO: For each key in *context*, replace ``{key}`` in the template
    with the corresponding value. Placeholders not in context should be
    left unchanged.
    """
    raise NotImplementedError
