
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
    <img width="3334" height="2500" alt="Diagram visually conveying components Chevron provides downstream applications" src="https://github.com/user-attachments/assets/d9de4367-a9ad-4a0b-ba03-d89f9c4af8a9" />
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

<div>
    <h1>License</h1>
    <h3>
        Chevron is released under the GNU Affero GPLv3 license. See <a href="https://github.com/InfinSys/Chevron/blob/v0.x/LICENSE">LICENSE</a> for the full text.
    </h3>
</div></br>

<hr></br>

<div>
    <h1>Development Updates</h1>
    <h2><em>Subsystem Development Progress</em></h2>
    <table>
        <thead>
            <tr>
                <th><strong>Subsystem</strong></th>
                <th><strong>Status</strong></th>
                <th><strong>Notes</strong></th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td><strong>Process Lifecycle</strong></td>
                <td><em>Design</em></td>
                <td><code>AppProcess</code> design still pending further library development</td>
            </tr>
            <tr>
                <td><strong>GUI Runtime</strong></td>
                <td><em>Design</em></td>
                <td><code>GUIEngine</code> design still pending further library development</td>
            </tr>
            <tr>
                <td><strong>Memory Infrastructure</strong></td>
                <td><em>Design</em></td>
                <td>
                    Process-level memory construct designs/implementations complete; Broader memory hierarchy still in-progress
                </td>
            </tr>
            <tr>
                <td><strong>Thread Infrastructure</strong></td>
                <td><em>Design</em></td>
                <td><code>ThreadCore</code> and related entity designs still incomplete</td>
            </tr>
            <tr>
                <td><strong>Host System Services</strong></td>
                <td><em>Not Started</em></td>
                <td>N/a</td>
            </tr>
            <tr>
                <td><strong>Logging System</strong></td>
                <td><em>Not Started</em></td>
                <td>N/a</td>
            </tr>
            <tr>
                <td><strong>Windowing System</strong></td>
                <td><em>Design</em></td>
                <td>
                    <code>WindowCoordinator</code> and <code>WindowDispatcher</code> designs nearly complete; <code>WindowEnvelope</code> and <code>WindowingSubsystem</code> still
                    pending further design
                </td>
            </tr>
            <tr>
                <td><strong>Window Component System</strong></td>
                <td><em>Design</em></td>
                <td>Incomplete design; Pending further window facilities design and development</td>
            </tr>
            <tr>
                <td><strong>Dynamic View System</strong></td>
                <td><em>Design</em></td>
                <td>Incomplete design; Pending further window facilities design and development</td>
            </tr>
            <tr>
                <td><strong>Event Routing System</strong></td>
                <td><em>Not Started</em></td>
                <td>N/a</td>
            </tr>
            <tr>
                <td><strong>Messaging Infrastructure</strong></td>
                <td><em>Not Started</em></td>
                <td>N/a</td>
            </tr>
            <tr>
                <td><strong>Telemetry Infrastructure</strong></td>
                <td><em>Not Started</em></td>
                <td>N/a</td>
            </tr>
        </tbody>
    </table>
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
    <p align="center"><em>( diagram showcasing major system component lifetime tiers )</em></p></br>
    <h2><em>Chevron Application Structure</em></h2>
    <p>
        Every Chevron application is structured around a single <code>AppProcess</code> at its root. <code>AppProcess</code> owns the <code>GUIEngine</code>, an abstraction whose
        concrete implementation matches the user's chosen GUI framework. The GUI engine hosts Chevron's windowing infrastructure: a dispatcher that creates windows, a coordinator
        that manages them once they exist, and a windowing subsystem that surfaces these components together as a coherent user-facing API. Live windows are represented by
        <code>WindowEnvelope</code> instances held by the coordinator, each carrying its own per-window infrastructure (<strong><em>PWI</em></strong>). The application's behavior
        lives inside those windows, where the user writes code against the chosen GUI framework directly.
    </p>
</div></br>

<h1>Conceptual Chevron Usage</h1>
<h2><em>Bootstrapping / Application Launch</em></h2>
<p>
    Chevron applications are bootstrapped from the users own <code>main()</code> entry point. During this stage, the process is tailor configured to the applications profile, a GUI
    engine is prepared for the users chosen framework, and window factories are registered to declare window intent. Once setup work is complete on the established GUI engine, it
    can then be handed off to <code>AppProcess</code> to enter application runtime under Chevron's direction. The bootstrapping phase is also the ideal opportunity to conduct any
    necessary pre-launch work.
</p>

> [!NOTE] <!--| GitHub Notice: Early Development |-->
> <p>
>     <strong>The main method is always tasked with handling process-wide setup, it is not application code. The application itself lives downstream of this stage, in the windows
>     and infrastructure that come online during runtime.</strong>
> </p>

```cpp
/*!
 * @file main.cpp
 *
 * @brief
 * Conceptual portable GUI application launch sequence.
 */

#include <chevron/entry.hpp>      // Entry point helpers and macros
#include <chevron/process.hpp>    // Application process-level constructs
#include <chevron/wx/runtime.hpp> // wxWidgets GUI framework engine

using chevron::AppProcess;
using WxEngine = chevron::wx::Engine;

using chevron::WindowFactory;
using chevron::ProcessMemoryConfig;
using chevron::ProcessThreadConfig;
using chevron::ProcessExitReport;

// Note:
// `ENTRY_POINT_METHOD_SIGNATURE` and `ENTRY_POINT_ARG_VARS` are
// Chevron defined macros from <chevron/entry.hpp>.

/*!
 * @brief
 * Application entry point (main method)
 */
ENTRY_POINT_METHOD_SIGNATURE {
    ProcessMemoryConfig memoryConfig{ /*Configure process memory*/ };
    ProcessThreadConfig threadConfig{ /*Configure process threads*/ };
    
    AppProcess proc{memoryConfig, threadConfig};
    
    WxEngine::Configuration runtimeConfig;
    runtimeConfig.forwardCmdlArgs(ENTRY_POINT_ARG_VARS);
    
    auto guiEngine = std::make_unique<WxEngine>(runtimeConfig);

    guiEngine->windowing().registerFactory(
        WindowFactory{
            /*Callable that returns new wxFrame pointer*/,
            /*Window dispatch descriptor*/
        }
    );
    
    proc.commitGUIEngine(std::move(guiEngine));
    proc.initializeGUIEngine();
    
    proc.mainloopEntry();
    ProcessExitReport report = proc.shutdown();
    return report.exitCode;
}
```

</br>

<div>
    <h2>Planned GUI Framework Support</h2>
    <h3>
        <a href="https://wxwidgets.org/">wxWidgets</a> |
        <a href="https://www.qt.io/development/qt-framework">Qt Framework</a>
    </h3>
</div></br>

<div align="center">
    <h3>( <em><strong>...This README is still in progress...</strong></em> )</h3>
</div>
