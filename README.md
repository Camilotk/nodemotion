<p align="center">
  <img src="./nodemotion_logo.png" alt="Node Motion" width="420">
</p>

# Node Motion

**Node Motion** is a C++23 framework for deterministic visualization of data-structure operations.  
It provides an explicit API for describing structural modifications (insertions, removals, pointer updates) and renders them as synchronized 2D animations.  
The library integrates **SFML** for rendering and **FFmpeg** for frame export.

---

## Overview

Node Motion is designed for developers, educators, and researchers who need reproducible, frame-accurate visualization of data structures.  
Each operation is captured, replayed, and rendered deterministically, producing visual explanations of how structures evolve in memory.

---

## Components

| Component | Description |
|------------|-------------|
| **NodeMotion** | Core controller managing node states, operations, layout, and animation sequence. |
| **Node** | Visual entity representing a structure element (value + pointer). |
| **Operation** | Encapsulates a structural change (`addFirst`, `addLast`, `remove`, `get`, etc.). |
| **SfmlRenderer** | Handles frame drawing and composition using SFML. |
| **FfmpegExporter** | Streams raw RGBA frames to FFmpeg for MP4 export. |
| **Layout Engine** | Automatically arranges and wraps nodes based on available width. |

---

## Build

**Requirements**
- GCC ≥ 13 or Clang ≥ 17  
- CMake ≥ 3.26  
- Conan 2.x  
- FFmpeg installed system-wide  
- SFML 2.6.x  

**Build Instructions**
```bash
apt update
apt install -y git build-essential cmake python3 python3-pip ffmpeg pkg-config \
    libx11-dev libgl1-mesa-dev libudev-dev libfreetype6-dev libjpeg-dev \
    libopenal-dev libsndfile1-dev libvorbis-dev libflac-dev
pip install conan
conan profile detect --force
conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install
make
````

This will build the NodeMotion static library and demonstration executables.

---

## Example

```cpp
#include "NodeMotion/NodeMotion.hpp"
using namespace nodemotion;

int main() {
    NodeMotion motion(1280, 720, 1);
    motion.setBackground(0x151515FF);

    motion.addCode({
        "List<int> list;",
        "list.addLast(10);",
        "list.addLast(20);",
        "list.addFirst(5);",
        "list.removeLast();"
    });

    Node* n1 = motion.createNode("10");
    Node* n2 = motion.createNode("20");
    Node* n3 = motion.createNode("5");

    motion.addLast(n1);
    motion.addLast(n2);
    motion.addFirst(n3);
    motion.removeLast();

    motion.render("linked_list_demo.mp4");
}
```

This example produces a deterministic MP4 animation showing the logical and visual state of a linked list evolving step by step.

---

## Makefile Commands

| Command        | Description                                     |
| -------------- | ----------------------------------------------- |
| `make`         | Configure and build the entire project          |
| `make setup`   | Install dependencies and generate CMake presets |
| `make build`   | Compile all sources                             |
| `make run`     | Execute the default demo                        |
| `make format`  | Apply clang-format to source files              |
| `make clean`   | Remove build artifacts                          |
| `make rebuild` | Clean and rebuild from scratch                  |

---

## Design Notes

* Deterministic frame sequencing (1 operation → 1 visual step).
* Clean separation between data model and rendering pipeline.
* Adaptive layout and multi-line wrapping for long structures.
* Automatic NULL detection and head tracking.
* No dynamic allocation during frame generation.
* Platform-independent MP4 output via FFmpeg.

---

## Development Roadmap

### Current Phase: Linked Structures

* [x] Singly Linked List visualization
* [x] Dynamic node insertion/removal
* [x] Automatic head and NULL detection
* [x] Multi-line adaptive layout
* [ ] Circular list visualization
* [ ] Indexed access (`get(index)`) animation
* [ ] Traversal highlighting

### Next Phase: Array-Based Structures

* [ ] Animated insertion and removal in static arrays
* [ ] Indexed access and search visualization
* [ ] Dynamic ArrayList insertion/removal with reallocation
* [ ] Visualization of element shifting

---

## License

Node Motion is distributed under the **GNU General Public License v3.0 (GPL-3.0)**.

See the [LICENSE](./LICENSE) file for full details.
