# Doxygen Documentation Conventions

## Purpose & Documentation Standards

**In-code documentation is a first-class part of Chevron's engineering quality.
It is not an optional polish applied after an implementation is complete**.
Documentation must be written with the same care given to the code it describes
and must remain consistent with Chevron's established terminology, design, and
expectations.

**The primary purpose of source documentation is to give a future reader the
correct mental model for the code they are about to work with. Good
documentation prepares the reader to understand an entity in context without
merely restating information that is already obvious from its declaration or
implementation**. It should capture the nature of the entity, its role, and the
important constraints or behaviors that are not immediately apparent from the
code itself.

**Documentation depth must remain proportional to the conceptual weight of the
entity**. Non-trivial types, functions, and other contract-relevant declarations
may require additional context to communicate their responsibilities and
important design details correctly. Simple declarations may require only a
brief description, and genuinely self-explanatory declarations may exceptionally
warrant no documentation at all. **Omission is the exception rather than the
normal case**. Documentation should provide the context that earns its place
without becoming an exhaustive report on details that the reader can readily
discover from the code.

### For Agents:
**Source documentation must describe Chevron as it exists, not the task,
conversation, review, or design process that happened to produce it**.
Instructions given during implementation and historical reasoning from active
development discussions must not leak into documentation simply because they
influenced the work. When such a process establishes an enduring design fact,
constraint, or behavior that matters to the reader, document the resulting
truth directly and **leave the discussion context behind**.

</br>

## Doxygen Form & Coverage

**Chevron source documentation must use explicit Doxygen documentation forms**.
Ordinary comments or unstructured prose are not substitutes for Doxygen
documentation where documentation is required. **Documentation must be written
with the appropriate Doxygen comment syntax and commands** so its structure and
purpose remain clear to both readers and generated reference documentation.

**Contract-important and structurally significant declarations must use
multiline Doxygen comment blocks**. This includes classes, structs, functions,
methods, enums, source-file documentation, and other declarations of
comparable importance. The `///` form is not permitted for these entities.
Its only accepted use is the trailing `///<` form for lightweight declarations
such as data members, enumerators, loose variables, and similar entities where
a compact description is appropriate.

**Documentation is the expected default for declarations throughout Chevron**.
An entity should normally be documented whenever its purpose, behavior, role,
or contract is not immediately and completely apparent from the declaration
itself. Authors must not treat documentation as something that is added only
when a declaration appears unusually complex. The normal expectation is to
document the declaration, with **omission reserved for cases where additional
documentation would provide no meaningful information**.

This expectation does not extend to ordinary local variables or incidental
implementation details within a scope. A genuinely trivial helper, one-line
function, simple member, or similarly self-explanatory declaration may
exceptionally remain undocumented. Such omissions should be uncommon and
based on genuine triviality rather than convenience or an author's preference
for less documentation.

</br>

## Documentation Composition

**Chevron documentation comments must be composed from the information the
documented entity actually warrants**. A comment is not a checklist of Doxygen
commands that must be populated mechanically. The brief establishes *what* the
reader is looking at, additional details build the necessary mental model,
notes and warnings isolate important focused statements, and other Doxygen
commands provide contract information when that information earns its place.

### Brief
Every documentation block begins with a `@brief` that **identifies the entity
at the highest useful level**. The brief should let a reader immediately
understand *what* they are looking at without explaining implementation details,
edge cases, design rationale, or other specifics. Begin the brief **directly**
with the descriptive language rather than an article such as "A", "An", or
"The". **Brief text remains on the same line as the `@brief` command and should
be concise enough that wrapping is not necessary**.

```cpp
/*!
 * @brief Process-level application coordinator.
 */
```

### Details
**When additional context is warranted, use a `@details` section to develop the
reader's understanding of the entity**. Begin with the nature, responsibility,
or role of the entity **before** moving into relevant mechanics or other
specifics. **The details should emphasize information that is non-trivial,
non-obvious, or necessary to understand the entity correctly**. They should not
become an exhaustive report of behavior that can be readily discovered from
the declaration or implementation.

```cpp
/*!
 * @brief Process-level application coordinator.
 *
 * @details
 * Owns the process-scoped services required by a Chevron application and
 * coordinates their lifetime with the surrounding application process.
 * ...
 */
```

### Notes/Warnings
**Use `@note` and `@warning` to separate important focused statements from the
general description when those statements deserve additional visibility**.
Each note or warning should address a particular concern rather than becoming
another general-purpose details section. **These statements are commonly short,
but their length must follow the information being conveyed rather than an
arbitrary line limit**.

```cpp
/*!
 * @brief Window-scoped runtime state.
 *
 * @details
 * Provides the state required to coordinate a single Chevron window with
 * the active GUI runtime.
 *
 * @note
 * The GUI runtime must outlive every window state associated with it.
 *
 * @warning
 * Access after GUI shutdown has begun is invalid.
 */
```

---

**Body-oriented commands such as `@details`, `@note`, and `@warning` must appear
on their own line, with their text beginning on the following line**. Separate
distinct documentation bodies with a blank comment line so that each logical
section remains visually clear in source. The compact `@brief` form is the
intentional exception because its text is brief by definition.

```cpp
State procState; ///< Current lifecycle state.
```

**Other Doxygen commands must be included when the information they express is
useful to understanding the entity or its contract**. Commands such as
`@param`, `@tparam`, `@return`, and `@throws` must not be added merely because
they are technically applicable. **Repeating an obvious parameter name or
restating information already expressed clearly by the declaration adds noise
and creates additional documentation that can become stale. Use these commands
when they communicate information that the declaration alone does not convey**.

</br>

## Writing for the Reader

**Source documentation must be written for the developer who encounters the code
without the context currently available to its author**. The author already knows
how the implementation works and why its design makes sense. The reader may not.
Documentation should transfer the understanding needed to approach the code
correctly without requiring the reader to reconstruct that understanding from
the implementation first.

**Prime the reader with the correct mental model before introducing mechanics or
specific behaviors. Establish what the entity is, what responsibility it holds,
and how it relates to the surrounding design when those relationships matter**.
Once that foundation is clear, describe the important mechanics, constraints,
or behaviors needed to complete the reader's understanding. Do not begin with
implementation details that have no useful context to attach to.

**Select documentation by consequence rather than completeness**. Prioritize
information that a competent developer could reasonably misunderstand,
overlook, or otherwise have to discover manually. Ownership and lifetime
relationships, sequencing requirements, important state relationships,
invariants, architectural responsibilities, non-obvious constraints, and
behavior that differs from an intuitive assumption are all strong candidates
when they apply. Obvious declaration details and readily discoverable
implementation specifics usually are not.

**Do not make the reader assemble an important relationship from disconnected
facts when the documentation can state that relationship directly**. Explain
meaningful dependencies and consequences clearly enough that the intended model
does not depend on guesswork. **Documentation should reduce the interpretive work
required to understand the code** and, in doing so, reduce the opportunity for an
incorrect assumption to take hold.

Documentation should stop once the reader is properly primed. It is not
intended to replace inspection of the declaration, implementation, tests, or
surrounding code. Excessive documentation can obscure the information that
actually matters just as easily as insufficient documentation can omit it.
**Provide enough context for the reader to understand the code they are about to
inspect correctly, but do not attempt to document every discoverable detail**.

**Documentation is not written to demonstrate that the author understands the
code. It is written to transfer the important parts of that understanding to
the reader clearly, efficiently, and accurately**.

</br>

## Doxygen Contract Tags

**Contract-oriented Doxygen commands must be used when they communicate
information that the declaration does not already make clear**. Their presence
is not a completeness requirement. **A parameter, template parameter, return
value, or possible exception does not automatically require a corresponding
documentation command**.

### Parameters
**Use `@param` and `@tparam` when an argument carries non-obvious meaning,
constraints, ownership expectations, lifetime requirements, sequencing rules,
special values, or other contract information that matters to the caller**.
Avoid descriptions that merely restate the parameter name or obvious purpose.

```cpp
/**
 * @brief Register factory for deferred window construction.
 *
 * @param factory
 * Factory whose lifetime must extend through its registration.
 */
```

### Return
**Use `@return` when the result has semantics that are not already apparent from
the declaration**. This may include ownership, lifetime, aliasing, sentinel
values, relationships to internal state, or other behavior that affects how
the caller should interpret or use the result. Do not add a return description
solely to repeat the return type or function name.

### Throw
**Use `@throws` only when an exception is a stable and unconditional part of the
function's contract. Exceptions that depend on conditional compilation,
configuration, build mode, or similar circumstances must not be documented as
though they apply universally**. Other failure or error semantics should be
described when they are sufficiently non-obvious to affect the caller's
understanding of the contract.

### Others
The same principle applies to other Doxygen commands. **A command earns its
place when it communicates useful contract information that would otherwise be
missed or misunderstood**. Do not add documentation structure merely because a
Doxygen command is technically applicable.

</br>

## Documentation by Entity Weight

**Documentation depth should follow the conceptual weight of the entity rather
than its declaration size, visibility, or syntactic category**. Substantial
entities generally require more context, while narrow or immediately obvious
entities should remain concise.

### High-weight Entities
Classes, structs, significant enums, major public functions, source-file
documentation, and non-trivial internal types commonly warrant more than a
brief. Their documentation should establish the entity's nature, role, and any
important contract or design constraints needed to understand it correctly.
**This does not mean every large declaration requires extensive documentation.
The amount of text should still follow what the reader actually needs**.

### Medium-weight Entities
Ordinary methods, constructors, operators, helper functions, type aliases, and
similar declarations often require less. A clear `@brief` may be sufficient
when the entity's purpose and behavior are straightforward. **Additional details
should be added only when they communicate something consequential that the
declaration alone does not make clear**.

### Lightweight Entities
Lightweight entities such as simple data members, enumerators, constants, and
loose variables commonly need only compact documentation. The trailing `///<`
form is appropriate when a short description provides all of the context the
reader needs. **This does not imply that every one of these entites require
documentation, often it is not necessary except where it provides value to code
comprehension**.

```cpp
enum class ShutdownState
{
    RUNNING,   ///< Normal runtime operation.
    STOPPING,  ///< Shutdown in-progress.
    STOPPED    ///< Runtime shutdown is complete.
};
```

---

**The specific entities listed under the documentation weight classes above are
not definitive**. A method could also be classified as lightweight, or even
high-weight if complex enough. The entities are not bound to the classes they
are listed under, weight class is determined by the amount of context necessary
in the entities documentation. **The listings above are merely common cases
rather than categorical positioning**.

**Conceptual weight is the governing rule**. A private internal mechanism may
need substantial documentation when its behavior is difficult to understand,
while a public function may genuinely require only a brief. **Document the amount
of context the entity deserves rather than the amount suggested by its size or
visibility**.

</br>

## Documentation Stability & Language

Source documentation must use Chevron's established terminology for its
components, relationships, lifecycle concepts, and mechanisms. Do not invent
alternate names for concepts that already have accepted project vocabulary.
Competing terminology makes the source, project documentation, and generated
reference material harder to understand as a consistent whole.

When Chevron has no established name for a concept, use clear descriptive
language that communicates the idea without implying that new project
terminology has been established. **Prefer ordinary technical English over
unnecessary jargon, obscure vocabulary, unexplained abbreviations, dense
academic phrasing, or stylistic complexity**. Compose regular sentences as a
competent C++ developer naturally would. **A competent C++ developer should be
able to read the documentation naturally without having to decode the author's
choice of language**.

**Do not use em dashes in source documentation. Colons, semicolons, and hyphens
should be used rarely and only when they genuinely improve or are required by
the technical meaning**. Do not overload documentation with punctuation in an
attempt to make the writing sound more sophisticated.

**Write documentation so that it remains useful as implementation details
change**. Avoid coupling explanatory prose to incidental local identifiers,
temporary implementation structure, or other details that are likely to
change without changing the underlying concept. **Prefer describing the stable
responsibility, relationship, or behavior when that communicates the same
idea more durably**.

This does not prohibit direct references to established Chevron entities or
names required by Doxygen commands such as `@param` and `@tparam`. Stable
project concepts should be named when doing so improves clarity. The goal is
to avoid unnecessary dependence on identifiers that are incidental to the
explanation rather than to make documentation artificially vague.
