## Open Design Questions

* The error handling boundary. The line between returned status codes and thrown
  exceptions is undecided library-wide.
* `AppProcess` one-instance-per-process enforcement.
* Static versus shared library support and its implications for symbol visibility,
  ABI stability, and `AppProcess` singleton invariants.
* `main()` entry point handling conventions.
* The GUI dependency strategy. CMake currently force-fetches the selected backend
  regardless of local installs.
* The `WindowDispatcher` to `WindowCoordinator` handoff protocol, the
  single-instance query interface, and the lifecycle event notification
  mechanism.
* The per-window infrastructure base component set and the meaning of the minimum
  PWI flag.
