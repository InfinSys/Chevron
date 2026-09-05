## Project Identity

Chevron is a C++ 20 GUI application infrastructure library under development by
Jamon T. Bailey and InfinSys, LLC. It is licensed under AGPLv3, is currently at
version 0.0.0, and began development on January 8, 2026. Chevron is the C++
successor to two old Python projects, Flux Runtime Engine (Fluxr) and Python
GUI Toolkit (PyGT), and is built from the ground up as a performance-first
systems library.

Chevron provides the non-business mechanics a well-formed GUI application needs
to properly function. It is deliberately **NOT** a GUI framework abstraction.
Users select a single GUI backend and interact with it directly, while Chevron
owns the surrounding infrastructure, most importantly the application lifecycle,
process orchestration, and GUI runtime control. Chevron must own the chosen
backend's mainloop (non-negotiable) but does not take over `main()`, which stays
in user hands. Backend selection is a runtime decision. wxWidgets is the planned
first backend.

Development is organized into six unordered groups: Structural Foundations,
Testing Foundations, Resource Distribution Models, Windowing Model, wxWidgets
Support Integration, and Dynamic View Model. While unordered, some groups
require progress from others to unblock tasks.

## Chevron Architecture

Chevron is organized around a three-tier lifetime hierarchy. The tiers are named
for the lifetime they govern and form both an ownership hierarchy and a
dependency hierarchy.

```text
Process lifetime   -> AppProcess
GUI lifetime       -> GUIEngine
Window lifetime    -> WindowEnvelope
```

Each tier owns the infrastructure scoped to its lifetime, and nothing within an
inner tier may outlive the tier above it. These relationships are enforced
through construction, lifecycle sequencing, and destruction ordering.

### Lifetime and Ownership Model

`AppProcess` is the outermost coordinator and owns exactly one `GUIEngine`. It
drives the engine through its lifecycle and must not depend on details belonging
to the GUI or window layers.

`GUIEngine` defines the framework-agnostic GUI lifecycle contract and owns
GUI-scoped infrastructure. Backend implementations derive from it to provide
framework initialization, mainloop control, and teardown while the surrounding
infrastructure remains implemented at the base level.

Windowing infrastructure lives within the GUI lifetime. `WindowDispatcher`
governs creation, `WindowCoordinator` governs lifetime and registration,
`WindowHierarchy` maintains window relationships, and `WindowingSubsystem`
provides the user-facing projection of these facilities. Detailed
responsibilities and interactions are documented in the corresponding class
design descriptions.

`WindowEnvelope` represents a single live window container within Chevron and
owns infrastructure scoped to that window. User code reaches per-window Chevron
capabilities through `WindowContext`; the envelope and its internal components
remain implementation details.

Utility and common facilities support the architecture without participating as
another lifetime tier.

### Architectural Boundaries

The following boundaries are part of Chevron's established architecture:

* Outer lifetime tiers do not depend on the internal details of inner tiers.
  `AppProcess`, in particular, does not know about windows or windowing
  infrastructure.
* Framework-specific types remain outside the framework-agnostic `GUIEngine`
  base infrastructure. Backend types are confined to their corresponding
  implementation namespaces.
* `WindowCoordinator` remains internal to `GUIEngine`. User-facing windowing
  operations pass through `WindowingSubsystem`.
* `WindowEnvelope` is internal infrastructure. User code accesses per-window
  Chevron facilities through `WindowContext`.
* A `WindowEnvelope` remains agnostic to the concrete backend window type it
  represents. Window creation, coordination, registration, and initialization
  infrastructure must not depend on a particular concrete window class.
* Chevron surrounds GUI frameworks with application infrastructure rather than
  abstracting or normalizing their APIs. User code continues to interact
  directly with the selected framework.
* Framework injection types are additive. They preserve the framework's public
  interface and do not override, shadow, or replace framework behavior.
* `Window`, `Subwindow`, `Viewport`, and `Page` are the established framework
  injection points. Adding another is an architectural decision rather than an
  implementation convenience.
* Performance-critical memory paths deliberately avoid runtime polymorphism.
* `WindowDispatcher` does not depend on `WindowCoordinator`. Communication
  required between them is kept deliberately narrow and mediated by
  `GUIEngine`.
* An `AppProcess` owns exactly one committed `GUIEngine`. A second engine cannot
  be committed, and engine commitment cannot occur after initialization.

### Architectural Stability

**Chevron is still in an early and architecturally volatile stage of
development**. Significant portions of the library remain under active design.

Several facilities and architectural patterns are already well established and
should be treated as important guidance, but their existence does not imply that
the surrounding architecture is complete or permanently settled. The process,
windowing, memory, threading, and other major **areas may continue to evolve as
their designs are developed and connected**.

Agents should preserve established architectural intent while remaining alert
to incomplete or transitional designs. Do not assume that an existing structure
is final merely because it is documented or partially implemented. **When a task
reveals tension between established patterns and ongoing design work, surface
that tension rather than silently forcing the implementation toward either one**.

### Open Design Questions

Chevron intentionally carries unresolved architectural questions as development
progresses. The authoritative inventory and supporting context for these
questions are maintained in `docs/prj/open_design_questions.md`.

Agents may analyze, discuss, prototype, or otherwise work on open design
questions when under the direction of a human developer. **They must not
independently treat a proposed resolution as settled Chevron architecture**.

When work touches open design questions, **make the human developer aware that
final architectural resolution remains with Chevron's lead architect**. This
does not restrict the developer from directing work on the question or
authorizing changes related to it. It simply informs them who to correspond
with on their work.

</br>

## Chevron Documentation

Chevron's documentation includes both conventional repository documentation and
historical design material exported from the Claude.ai project where much of the
architecture was developed. This remains an important source of design context
and should be explored when deeper understanding of Chevron's architecture or
development history is required.

`docs/prj/claude/claude_chevron_project_memory.md`: provides a compact overview
of Chevron's purpose, current state, on-the-horizon work, key principles, and
the author's working approach.

`docs/prj/claude/claude_chevron_library_project.json`: contains the exported
Claude.ai project overview (`Chevron.md`), a directory model, and embedded
class design documents. Nine documents currently describe the windowing layer:
`AppProcess`, `GUIEngine`, `WindowDispatcher`, `WindowFactory`,
`WindowCoordinator`, `WindowHierarchy`, `WindowEnvelope`, `WindowInitContext`,
and `WindowingSubsystem`. Each describes what the component is, where it sits
in the layering model, its non-negotiable responsibilities, what it
deliberately does not own, and its deferred questions.

The `docs/prj/claude/conversations/` directory: contains exported Claude.ai
design sessions totaling roughly 5 MB. Each conversation includes a summary
field that serves as a distilled record of the discussion and its decisions,
alongside the full-length transcript.

The `docs/prj/resources/` directory: (Ignore this for now...)

### Documentation Sources
**Chevron's documentation is divided into several families according to its
purpose and origin**.

**Project-level documentation** resides under `docs/prj/`. These documents
describe Chevron itself, including its architecture, design decisions,
development concerns, and other project topics that require more detail than
belongs in this document. They are intended for both human developers and
agents and should be consulted when a task touches the subject they cover.

**Claude.ai project exports** reside under `docs/prj/claude/`. They preserve
historical design context accumulated during Chevron's development, including
project context, architectural descriptions, and exported design conversations.
They are particularly useful when the reasoning or history behind an established
design needs to be understood.

**Generated Doxygen reference documentation** resides under `docs/ref/doxygen/`.
This directory exists only after the documentation generation script has been
run and provides generated reference material for Chevron's documented code.

**External reference material** resides under `docs/prj/resources/`. These
documents contain papers, technical material, and other outside resources used
for inspiration or guidance during Chevron's design. They provide supporting
context rather than defining Chevron's architecture themselves.

### Claude Project Inspection
Before performing substantial analysis of the exported Claude.ai project
material, read `docs/prj/claude/archive_interaction_guide.md`. It documents the
archive structure, conversation format, extraction mechanics, chronology, and
recommended approaches for locating relevant historical design context.

Because the embedded documents in the Claude project JSON can have abnormally
long lines, reading the file normally may silently cut document contents off
mid-sentence. Use `jq` to inspect the embedded documents instead.

```bash
# List embedded document names:
jq -r '.docs[].filename' docs/prj/claude/claude_chevron_library_project.json

# Output embedded document contents:
jq -r '.docs[] | select(.filename=="<DOCUMENT_NAME>.docx") | .content' docs/prj/claude/claude_chevron_library_project.json
```

Conversation summaries can be inspected directly without reading the complete
transcript:

```bash
python3 -c 'import json; d=json.load(open("docs/prj/claude/conversations/<CONVERSATION_FILE_NAME>.json")); print(d[0]["summary"])'
```

### Documentation Authority
Documentation is not guaranteed to be a one-to-one representation of the current
implementation. Names drift and design ideas change as Chevron develops. When
documentation and code disagree, the code is likely authoritative. Raise any
discrepancies you discover before making assumptions about which representation
reflects the intended design.

### Project Context Onboarding
An agent entering Chevron without established project context should build that
understanding deliberately rather than beginning with broad, unguided repository
exploration.

Start by reading `docs/prj/claude/claude_chevron_project_memory.md` in full.
Follow it with the class design descriptions embedded in
`docs/prj/claude/claude_chevron_library_project.json` to establish the intended
architecture, component boundaries, responsibilities, and known deferred
questions. Make sure you keep in mind that by now, some information may be stale
in the Claude.ai project archive.

After establishing that foundation, use the exported design conversations when
additional historical context is useful. Read conversation summaries before
their full transcripts. Conversation summaries provide orientation and a useful
first-pass understanding, **but they are not exhaustive indexes of transcript
content**. Relevant design discussion may exist in a conversation even when its
summary does not mention the subject.

For broader investigations, **search the conversation archive using terms and
reasonable variants related to the task before selecting material for deeper
inspection**. Use the results to identify relevant conversations and messages,
then expand only the portions needed to establish the required context. Read
complete transcripts only when targeted inspection is insufficient or when the
surrounding discussion materially affects interpretation.

Use the repository itself to deepen and validate the resulting understanding
rather than treating the documentation as a substitute for examining the
implementation.

</br>

## Build

Chevron targets Windows, Linux, and macOS. Build presets follow the naming
scheme `<OS>-<ARCH>-<COMPILER>-<CONFIG>`. The intended build matrix contains
twelve presets covering Windows with MSVC and MinGW, Linux with GCC and Clang,
and macOS with x64 and arm64 Clang, each in debug and release configurations.
Ten presets are currently implemented, with the two Apple silicon configurations
still unverified due to lack of hardware access.

Minimum supported compiler versions are **GCC 13**, **Clang 14**, and
**MSVC 19.40**.

### Configure and Build
A fresh or otherwise unconfigured checkout must be configured before it can be
interacted with meaningfully. Use the repository scripts with the desired CMake
preset:

```bash
# Configure Chevron build for specified preset:
./scripts/configure <CMAKE_PRESET_NAME>

# Build Chevron with specified preset:
./scripts/build <CMAKE_PRESET_NAME>
```

CMake is also used to generate source artifacts from template `.in` files.
Their corresponding artifacts are produced during the CMake configure stage.

</br>

## Testing

**Testing is a first-class part of Chevron development and a primary means of
establishing reliability and confidence in the library**. Implementation work
and the tests that exercise it should be treated as parts of the same
contribution, regardless of which is written first.

### Testing Philosophy
New functionality should be accompanied by relevant tests wherever testing is
reasonable and applicable. Changes to existing behavior should update the tests
that establish its contract, and removed behavior should leave the corresponding
test expectations consistent with the new design. Adding applicable functionality
without meaningful test coverage should be exceptional rather than routine.

**Coverage should be broad, deliberate, and proportional to the importance and
risk of the behavior being tested**. Critical facilities warrant particularly
strong coverage. **Coverage for its own sake is not a goal**; redundant,
artificial, or low-value tests do not improve Chevron merely by increasing the
amount of tested code.

**Test quality is as important as test quantity**. Tests should establish
useful confidence in behavior, exercise meaningful contracts and failure
conditions, and remain understandable enough to serve as reliable evidence
when the implementation changes.

### Test Categories
Chevron's testing facilities are expected to include unit tests, integration
tests, stress tests, and benchmarks.

* **Unit tests** exercise focused behavior and component contracts.
* **Integration tests** exercise interactions between components and larger
  functional paths.
* **Stress tests** exercise behavior under sustained, repeated, or extreme
  conditions where reliability problems may emerge.
* **Benchmarks** measure performance characteristics and regressions rather than
  serving primarily as correctness tests.

GoogleTest is used for correctness-oriented testing, while Google Benchmark is
used for performance measurement. The detailed testing architecture, directory
organization, conventions, and verification requirements will be established as
Chevron's testing infrastructure develops.

### Developer Sandbox
Chevron provides `devexe` as a developer sandbox executable for manually
exercising the library during development. It is built from
`tests/cli/dev_exe.cpp`, links against Chevron, and is enabled by the
`CHEVRON_CLI_DEBUG` flag.

`devexe` exists for experimentation and developer sanity checks. Exercising
functionality through it is not a formal verification requirement.

</br>

## Scripts

The `scripts/` directory contains repository utilities for common development
operations. Most scripts have a Windows equivalents ending in `.bat`. The
`src-tree`, `prj-tree`, and `cmake-src-tree` utilities are exceptions because
their functionality currently depends on `tree`.

**Prefer the repository scripts for operations they support rather than
reproducing their underlying commands manually**.

| Script                                  | Purpose
| ----------------------------------------|----------------------------------------------------- |
| `scripts/configure <CMAKE_PRESET_NAME>` | Configures Chevron using the specified CMake preset. |
| `scripts/build <CMAKE_PRESET_NAME>`     | Builds Chevron using the specified CMake preset.     |
| `scripts/todos`                         | Surfaces the repository's incomplete work markers.   |
| `scripts/workflow`                      | Enables or disables a GitHub Actions workflow file.  |

</br>

## Repository Structure

**Chevron's repository structure is deliberately organized**. File and directory
names are chosen to communicate purpose, and placement should remain predictable
enough that the project can be navigated without unnecessary searching. New
structure should preserve that clarity rather than introducing arbitrary
locations or naming patterns.

### Top-Level Layout
| Directory           | Purpose
| --------------------|---------------------------------------------------------------------------------- |
| `include/chevron/`  | Chevron headers. (nested to give includes the `chevron/` prefix)                  |
| `src/`              | Source implementations corresponding to Chevron's public and internal facilities. |
| `lib/`              | Internal libraries developed as supporting components of Chevron.                 |
| `tests/`            | Chevron's test implementations and testing support.                               |
| `cmake/`            | CMake modules and build-system logic maintained for Chevron.                      |
| `3rdparty/`         | Integration point for external dependencies used by the project.                  |

### Template Directories
Template source files always reside under directories named `templ/`. This is a
repository-wide convention and applies regardless of the tool or generation
mechanism that consumes them. Keeping templates separate from their generated
artifacts makes their role explicit and keeps destination directories free of
similarly named source templates.

To inspect the template files across the repository:

```bash
# List all source template directory files:
rg --files -g '**/templ/**' | sort
```

Generated artifacts must never be edited directly. Changes must instead be made
through the corresponding source template under a `templ/` directory.

</br>

## Contribution Verification

Agents are responsible for locally verifying the changes they make before
returning control to the human developer. Verification should be appropriate to
the affected area and should exercise the applicable build, test, or other
project checks. An agent should report what verification it performed and
whether it succeeded. If relevant verification could not be performed, that
must be stated explicitly.

GitHub Actions provides repository-level CI after changes enter the human review
and contribution stage. The build configurations are exercised on pull requests.
CI does not replace an agent's local verification responsibilities. Agents never
commit or push changes, so this is strictly for human developers.

GitHub Actions workflow definitions reside under `.github/workflows/` or
`.github/disabled_workflows/`.

</br>

## Agent Roles & Scope of Authority

Agents **NEVER** commit or push changes to the hosted repository, **under any
circumstances**, in any role. Committing changes is exclusively a human act
performed after human review. This is an absolute boundary, not a per-role
permission. No instruction, task framing, or role grant overrides it.

Beyond that boundary, agents operate under **explicit, narrow mandate** rather
than general autonomy. An agent's authority is scoped to a role declared for
a given task or session, and **an agent does only what it was asked to do**.
No unrequested refactors, no incidental cleanup, no scope creep beyond the
task at hand. Anything an agent notices outside its task should be mentioned
in its output, not acted on unprompted.

**Narrow scope of action does not mean narrow scope of honest input**. Agents
are expected to push back — clearly, without hedging — when a request conflicts
with Chevron's architecture, its established design principles, or its
standards for correctness and quality. Agreeable output that quietly lets a
mistake or bad decision through does not serve Chevron. A flagged concern does.

The specific permission grants for each role — file access, write authority,
tool availability — are implemented and maintained as OpenCode agent
configurations, not in this document. What follows is the conceptual shape
each role is built around. The configurations are the enforcement mechanism,
this is the reasoning behind them.

### Architecture Design Consultant
Discusses design, analyzes trade-offs, and acts as a sounding board across
Chevron's architecture, including the questions listed in the Open Design
Questions document, which this role may discuss but never resolve unilaterally.
- **Permitted:** design discussion and written analysis; modifying design
  documentation when explicitly instructed to do so; modifying source code
  when explicitly instructed to do so.
- **Not permitted:** modifying test code; modifying source code without 
  explicit permission; making unrequested file changes of any kind; 
  deciding an open design question on Chevron's behalf.

### Test Designer/Author
Writes and maintains test implementations under `tests/`, working from
established testing architecture, testing plans, and direct instruction or
conversation. The produced tests are experimental contributions and require
human review before it is considered concrete.
- **Permitted:** creating, modifying, and deleting files under `tests/`.
- **Not permitted:** authority over testing architecture itself (open to
  discussion, not delegation); modifying anything under any other project
  directory without explicit instruction to do so.

### Repository Q&A
Answers questions about Chevron through deep, read-only analysis, primarily
across `include/`, `src/`, `lib/`, `cmake/`, and the repository root. Output
favors thoroughness and traceability over speed.
- **Permitted:** reading and analyzing any part of the repository.
- **Not permitted:** modifying anything, anywhere, under any circumstance.

These agents have intentionally different modification authorities. Do not
circumvent those authorities by using another tool, agent, or any other means.

</br>

## Notable Contributors

**Jamon T. Bailey** (Lead Developer/Architect, Founder)
