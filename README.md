
<!--
    (January 2026 - Jamon T. Bailey)
-->

<div>
    <h1 align="center">Chevron Library</h1>
    <h5 align="center">InfinSys, LLC. 2026</h5>
</div></br>

[![Static Badge](https://img.shields.io/badge/version-0.0.0-blue)](https://github.com/InfinSys/Chevron/releases)

</br>

[![Windows Build Status](https://github.com/InfinSys/Chevron/actions/workflows/windows-build.yml/badge.svg?branch=v0.x)](https://github.com/InfinSys/Chevron/actions/workflows/windows-build.yml)
[![Linux Build Status](https://github.com/InfinSys/Chevron/actions/workflows/linux-build.yml/badge.svg?branch=v0.x)](https://github.com/InfinSys/Chevron/actions/workflows/linux-build.yml)
[![macOS Build Status](https://github.com/InfinSys/Chevron/actions/workflows/macos-build.yml/badge.svg?branch=v0.x)](https://github.com/InfinSys/Chevron/actions/workflows/macos-build.yml)

<br/>

<div align="center">
    <img width="5334" height="2667" alt="Chevron social preview graphic" src="https://github.com/user-attachments/assets/ae03bd22-b48a-4cb0-83e7-afae662bd171" />
</div></br>

<div>
    <h1><em>What is this?</em></h1>
    <p>
        Every C++ GUI application has to settle the same foundational details before it can really begin: how memory is structured, how concurrent threads of execution are managed,
        how windows are handled, how UI views change at runtime, and more. None of this is the application itself, yet it is crucial to the applications functionality.
    </p>
    <p>
        <strong>
        Chevron is a C++20 GUI application infrastructure library that owns the mechanics of a modern GUI application (<em>process lifetime, window scaffolding, dynamic view handling,
        thread infrastructure, etc.</em>) so user code can focus on the application itself, written directly against the GUI framework the user chose.
        </strong>
    </p></br>
    <img width="3334" height="2500" alt="Diagram visually conveying components Chevron provides downstream applications" src="https://github.com/user-attachments/assets/517181f7-a682-4dee-876f-ebf96ee59403" />
    <p align="center"><em>( diagram showcasing components Chevron provides downstream applications )</em></p>
</div></br>

> [!WARNING] <!--| GitHub Notice: Early Development |-->
> <h3>
>    Chevron is in active early development. The API is unstable and breaking changes should be expected. This library is not yet suitable for production code!
> </h3>

</br>

<div>
    <h1>What Chevron is <em><strong>NOT</strong></em></h1>
    <h3>
        <strong>Chevron is GUI application <em>infrastructure</em>,</strong>
    </h3>
    <h3>
        <strong>not a GUI framework abstraction</strong>
    </h3>
    <p>
        It <strong>does NOT unify GUI frameworks behind a portable interface</strong> and it <strong>does NOT introduce an explicit widget layer of its own</strong>. A Chevron user
        picks exactly one GUI framework, includes it directly, and writes GUI code similar to how they typically would. What changes is the structural environment that code lives
        inside (<em>the parts of the application that aren't about the GUI but are around it</em>).
    </p>
</div></br>

<hr></br>

<div>
    <h1>Architecture Overview</h1>
    <h2><em>Three-Tier-Lifetime Hierarchy</em></h2>
    <p>
        Chevron is organized as three nested lifetime tiers, each owned and bounded by the one above it. <code>AppProcess</code> opens the process lifetime and lives for the full
        duration of the application. Inside it, <code>GUIEngine</code> opens the GUI lifetime, governing framework initialization, the mainloop, and teardown. Inside that, each
        live window is represented by a <code>WindowEnvelope</code>, created when a window is dispatched and destroyed when that window closes. Nothing in a tier may exist before
        its enclosing tier has opened, and nothing may outlive its close. Every other piece of infrastructure in the library (<em>memory, threading, windowing, dynamic views</em>)
        lives somewhere inside this nesting.
    </p>
    <img width="2917" height="1250" alt="Chevron three-tier-lifetime hierarchy diagram" src="https://github.com/user-attachments/assets/2febb3cf-b431-48dd-bd0e-d848cf322d89" />
    <p align="center"><em>( diagram showcasing major system component lifetime tiers )</em></p>
</div></br>

<h1>Conceptual Chevron Usage</h1>

```cpp
#include <chevron/entry.hpp>      // `ENTRY_POINT_METHOD_SIGNATURE` and `ENTRY_POINT_ARG_VARS` macros
#include <chevron/process.hpp>    // Application process-level constructs
#include <chevron/wx/runtime.hpp> // wxWidgets GUI framework engine
#include <chevron/units.hpp>      // Digital size units

using chevron::AppProcess;
using WxEngine = chevron::wx::Engine;

using chevron::ProcessMemoryConfig;
using chevron::ProcessThreadConfig;
using chevron::ProcessExitReport;

ENTRY_POINT_METHOD_SIGNATURE
{
    ProcessMemoryConfig memoryConfig = /*Configure process memory*/;
    ProcessThreadConfig threadConfig = /*Configure process threads*/;
    
    AppProcess proc{memoryConfig, threadConfig};
    
    WxEngine::Configuration runtimeConfig;
    runtimeConfig.forwardCmdlArgs(ENTRY_POINT_ARG_VARS);
    
    auto guiEngine = std::make_unique<WxEngine>(runtimeConfig);
    guiEngine->windowing().registerFactory( /*Callable that returns wxFrame pointer*/ );
    
    proc.commitGUIEngine(std::move(guiEngine));
    proc.initializeGUIEngine();
    proc.mainloopEntry();
    
    ProcessExitReport report = proc.shutdown();
    
    return report.exitCode;
}
```
<!--
<p>
    <strong>The user constructs an <code>AppProcess</code>, builds the concrete <code>GUIEngine</code> for their chosen framework, registers window factories through the engine's
    windowing surface, commits the engine to the process, and runs. Everything else (<em>subsidiary windows, runtime dispatches, infrastructure access</em>) happens from inside
    the application after the mainloop is running.</strong>
</p>
-->

</br>

<div>
    <h2>Planned GUI Framework Support</h2>
    <h3><a href="https://wxwidgets.org/">wxWidgets</a> | <a href="https://www.qt.io/development/qt-framework">Qt Framework</a></h3>
</div></br>

<hr></br>

<div align="center">
    <h3>( <em><strong>...This README is still in progress...</strong></em> )</h3>
</div>
