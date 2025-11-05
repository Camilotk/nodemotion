<p align="center">
    <img src="./nodemotion_logo.png">
</p>

**Node Motion** is a C++23 library for visualizing data structure operations as animated 2D motion.

Its goal is to make learning data structures more intuitive — turning insertions, removals, and traversals into smooth visual stories.

---

## ✨ Purpose

Node Motion aims to:
- Show how **arrays**, **lists**, and other structures change step by step.
- Help students and educators **see** what happens during operations.
- Offer a modular, extensible C++ framework for building and rendering these visualizations.

---

## 🧩 Architecture Overview

- **Core Logic** — Abstract data structures that emit state changes.
- **Renderer** — SFML-based 2D engine for real-time animation.
- **Exporter** — FFmpeg integration to save animations as MP4.
- **Controller Layer** — Coordinates updates between data and visuals.

Everything is modular, so new structures and visualization styles can be added easily.

---

## ⚙️ Build (Development)

```bash
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
````

---

## 📘 Vision

Node Motion will grow into an open educational tool and reusable library.
It will allow developers, teachers, and students to:

* Visualize how data moves inside a program.
* Export lessons and demos as videos.
* Integrate visual feedback directly into their own C++ projects.

---

## Makefile

| Command        | Description                                     |
| -------------- | ----------------------------------------------- |
| `make`         | Configure & build everything                    |
| `make setup`   | Install dependencies and generate CMake presets |
| `make build`   | Compile the code                                |
| `make run`     | Run the `node_demo` executable                  |
| `make format`  | Run `clang-format` across all sources           |
| `make clean`   | Remove the `build/` folder                      |
| `make rebuild` | Full clean + rebuild                            |

