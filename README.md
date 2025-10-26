# 🚀 ImGui DirectX 11 Framework Base

A clean and modular **framework base** built around **Dear ImGui** and **DirectX 11**, designed for developers who want to quickly create tools, overlays, and applications with an efficient and extendable structure.

---

## ✨ Features

- 🎨 **Dear ImGui Integration** — Includes full support for the latest docking branch
- ⚙️ **DirectX 11 Renderer** — Optimized, minimal, and efficient rendering backend
- 🧩 **ImGui Loader** — Modular ImGui initialization and cleanup system
- 🔧 **Framework Base** — Clean architecture for rapid expansion
- 🪶 **Input Handling** — Integrated Win32 + ImGui IO management
- 💻 **Debug Console** — Optional in-app console/log viewer
- 🧠 **Memory Safe** — RAII-based resource management and clean shutdown
- 📦 **Extension Friendly** — Built with `ext/` for third-party or modular expansion
- 🔄 **Hot Reload Ready** — Designed to support runtime reload of resources or modules

---

## 🧱 Project Structure

```text
Framework/
│
├── ext/                         # External / third-party libraries
│   ├── freetype/                # Font rendering library
│   ├── ImGui/                   # Dear ImGui source + backends
│   ├── misc/                    # Additional ImGui extensions or utilities
│   └── SDK/                     # SDK or API headers used by the framework
│
├── src/
│   ├── Headers/                 # Core header files
│   │   ├── font_awesome.h
│   │   ├── fonts.h
│   │   ├── images.h
│   │   ├── main.h
│   │   ├── particles.h
│   │   └── settings.h
│   │
│   └── Sources/                 # Source files implementing framework logic
│       ├── framework.cpp
│       ├── main.cpp
│       └── ui.cpp
│
└── build/                       # Generated binaries and build output
