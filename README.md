
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
    <img width="5334" height="2667" alt="Chevron_GitHub_Social_Preview_ren002" src="https://github.com/user-attachments/assets/ae03bd22-b48a-4cb0-83e7-afae662bd171" />
</div></br>

<div>
    <h1><em>What is this?</em></h1>
    <p>
        Every C++ GUI application has to settle the same foundational details before it can really begin: how memory is structured, how concurrent threads of execution are managed,
        how windows are managed, how UI views change at runtime, etc. None of this is the application itself, yet it is crucial to the applications functionality.
    </p>
    <p>
        <strong>
        Chevron is a C++20 GUI application infrastructure library that owns the mechanics of a modern GUI application (<em>process lifetime, window scaffolding, dynamic view handling,
        memory/thread infrastructure, etc.</em>) so user code can focus on the application itself, written directly against the GUI framework the user chose.
        </strong>
    </p>
</div></br>

> [!WARNING] <!--| GitHub Notice: Early Development |-->
> <h3>
>    Chevron is in active early development. The API is unstable and breaking changes should be expected. This library is not yet suitable for production code!
> </h3>

</br>

<div>
    <h1>What Chevron is <em><strong>NOT</strong></em></h1>
    <h3>
        <strong>Chevron is GUI application <em>infrastructure</em>, NOT a GUI framework abstraction</strong>
    </h3>
    <p>
        It does not unify GUI frameworks behind a portable interface and it does not introduce an explicit widget layer of its own. A Chevron user picks exactly one GUI framework, includes
        it directly, and writes GUI code as they typically would. What changes is the structural environment that code lives inside (<em>the parts of the application that aren't about the
        GUI but are around it</em>).
    </p>
    <p> [ ...graph visually conveying Chevron's relationship to a GUI framework... ] </p>
</div></br>

<hr></br>

<div>
    <h1>Planned GUI Framework Support</h1>
    <h3><a href="https://wxwidgets.org/">wxWidgets</a> | <a href="https://www.qt.io/development/qt-framework">Qt Framework</a></h3>
</div></br>

<hr></br>

<div align="center">
    <h3>( <em><strong>...This README is still in progress...</strong></em> )</h3>
</div>
