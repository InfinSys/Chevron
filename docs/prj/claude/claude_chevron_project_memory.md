### Purpose & context

Jamon is the sole architect and developer of Chevron, a C++ 20+ GUI application infrastructure library built
from the ground up. Chevron is the C++ successor to two earlier Python projects called "Flux Runtime Engine"
(Fluxr) and "Python GUI Toolkit" (PyGT), and is designed as a serious, performance-first systems library with
strong emphasis on correctness, explicit naming, fail-fast behavior, and clean architecture. The library
is built on a cross-platform CMake build system based on Jamon's own "xplatform-cpp" template repository.

Core architectural concepts include: a three-tier lifetime hierarchy (`AppProcess` → `GUIEngine` →
`WindowEnvelope`), a "memory dripping" hierarchical arena allocator model, and a framework-agnostic windowing
abstraction with a concrete wxWidgets backend planned as the first integration.

Key subsystems either implemented or in active design: process-level memory pool (`ProcessMemoryPool`, `MemoryCore`,
`MemoryScope`), windowing infrastructure (`WindowDispatcher`, `WindowCoordinator`, `WindowHierarchy`), digital units
(`DigitalSize` template), utility libraries (`ScopeGuard`, `alignment.hpp`, `powers.hpp`, bit inspection, compile-time
math), and CMake platform detection.

Jamon has deep expertise in C++ systems programming — atomics, memory ordering, RAII, template metaprogramming,
lock-free data structures, and modern CMake — and engages with all design decisions at that level of depth.

---

### Current state

Memory subsystem design is in progress. `ProcessMemoryPool` has a drafted header implementation. The broader
`MemoryCore`/`MemoryScope` hierarchy design is substantially settled, including thread-local geometric growth
for contention reduction with block return path still an open question. Fixed-size memory pool education (free
lists, growth strategies, alignment, external metadata) has been completed conceptually; thread safety and
downstream interface sections remain. CMake platform detection is implemented: `cmake/platform/detective.cmake`
aggregates `arch_detection.cmake` and `os_detection.cmake`, with a `cmake/utility/detection_summary.cmake` for
configure-time output. Architecture and OS detection variables follow `CHEVRON_<DOMAIN>_<DETAIL>` screaming
case with INTERNAL cache storage.

---

### On the horizon

- **Unit test development**: Retroactively establish robust tests for existing Chevron constructs, and properly prepare GitHub Actions CI workflow for running tests.
- **Remaining memory pool education**: thread safety (section 6) and downstream interface design (section 7).
- **Unresolved design concerns identified for coordinated future decisions**: CMake dependency management for GUI frameworks (currently forces fetch regardless of local installs), `main()` entry point handling, and static vs. shared library build support (with implications for symbol visibility, ABI stability on virtual interfaces, LTO, and `AppProcess` singleton invariants).
- **Per-milestone breakdowns** within each of the current six development phases (*Structural Foundations*, *Testing Foundations*, *Resource Distribution Models*, *Windowing Model*, *wxWidgets Support Integration*, *Dynamic View Model*).
- **Library-wide exception handler** at `AppProcess` level and equivalent handlers at Chevron-spawned thread entry points.
- **GitHub Actions workflow** for automated Doxygen generation and deployment to GitHub Pages.

---

### Key learnings & principles

- **Tests validate the contract, not the implementation.** Test cases must make no assumptions about current implementation details — only about the documented behavioral contract — because implementations can change. This is a first-class design principle for Chevron's test suite.
- **Parameterized tests over single-value tests when applicable.** GTest parameterized testing with representative semantic partitions should be used when it is warranted. Don't force it.
- **Memory hierarchy design principle.** Check local free memory before going upstream; cascading upstream requests are an architectural failure mode to prevent.
- **Thread safety through self-sufficiency.** Thread-local geometric growth of allocation requests reduces contention by letting hot threads drift toward self-sufficiency, accepting some memory waste as an explicit tradeoff.
- **Single-writer property as a design goal.** The event-loop-based block return path achieves single-writer semantics on the free list push path, eliminating the need for CAS.
- **Scope guard pattern for multi-phase construction.** RAII-generalized cleanup obligations via ScopeGuard are the idiomatic solution to exception-safety gaps in multi-phase acquisition sequences.
- **Stack unwinding is implementation-defined without a matching handler.** Scope guards won't fire reliably without a top-level handler — a library-wide `AppProcess` handler is necessary infrastructure, not optional.

---

### Approach & patterns

- **Plan before implement, always.** Jamon consistently slows down to complete design and understand tradeoffs before writing any code. Claude should match this pace and not move toward implementation until design is explicitly settled.
- **Collect concerns before solving them.** When multiple unresolved issues exist, Jamon prefers to surface and compile them first to identify cross-cutting patterns, then address them in a coordinated way.
- **Iterative review with strong directional guidance.** Jamon provides clear intent and pushes back precisely when Claude misreads it, conflates distinct concepts, assumes indecision, or over-weights topics.
- **Minimal ceremony, maximal clarity.** Designs should avoid unnecessary variables, extra files, boilerplate, and overhead unless the content genuinely warrants it.
- **Whatever best validates the contract.** Test structure should not be forced into uniformity — mixing fixture types or approaches within a file is acceptable if it produces more robust contract validation.
- **Prose quality matters.** Documentation and README text should flow naturally sentence-to-sentence, avoid stacked declarative statements, never use em dashes, hyphens, colons, and semicolons only where genuinely necessary (avoid them), and calibrate length to necessity rather than completeness.
- **No polymorphism on performance-critical paths.** C++ 20 concepts preferred over templates where type encoding would leak structural position.
- **Constructor minimalism.** Keep constructor parameter counts minimal; use post-construction configuration methods for non-invariant settings.

---

### Tools & resources

- **Language/Standard:** C++ 20+
- **Build System:** CMake (modern practices; legacy patterns explicitly avoided)
- **Testing:** GoogleTest and Google Benchmark
- **Documentation:** Doxygen with /*! comment style; GitHub Pages for hosting
- **Diagramming:** draw.io (used independently by Jamon; Claude serves as design sounding board, not diagram collaborator)
- **Version control/project management:** GitHub (milestones for phase tracking, Actions for CI)
- **Key reference literature:** Bonwick 1994 slab allocator paper, Bonwick & Adams 2001 magazines/vmem paper, Berger et al. Hoard (ASPLOS 2000), Wilson et al. 1995 dynamic storage allocation survey
