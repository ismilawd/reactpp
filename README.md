# ReactPP - React-like GUI Framework in C++

A modern GUI framework in C++ that mimics React's declarative paradigm, component model, and state management patterns.

## Features

- **Virtual DOM System**: Efficient tree structure with diffing and reconciliation
- **Component System**: Class-based and functional components with lifecycle management
- **Hooks System**: useState, useEffect, useRef, useMemo, useCallback, useReducer, useContext
- **Context API**: Prop drilling avoidance
- **Event System**: Synthetic events with bubbling/capturing
- **Rendering**: SDL2 backend with flexbox layout engine
- **Framebuffer Support**: Direct rendering to Linux framebuffer (`/dev/fb0`) without X11/Wayland
- **Type Safety**: Modern C++17 with compile-time type checking

## Building

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler
- SDL2 development libraries
- nlohmann/json (header-only, will be fetched if not found)
- Google Test (for tests)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

```bash
cd build
ctest
```

### Running Examples

```bash
cd build
./bin/example_simple
./bin/example_counter
```

## Framebuffer Mode (Headless Linux)

ReactPP supports rendering directly to the Linux framebuffer device (`/dev/fb0`) without requiring X11 or Wayland. This is useful for embedded systems, kiosks, or systems without a window manager.

### Prerequisites

- Linux system with framebuffer device (`/dev/fb0`)
- Root access or membership in the `video` group (to access `/dev/fb0`)
- Build the project as normal (framebuffer renderer is included)

### Setup

1. **Verify framebuffer device exists:**
   ```bash
   ls -l /dev/fb0
   ```

2. **Check framebuffer info:**
   ```bash
   fbset -i
   ```
   This shows the current framebuffer resolution and color depth.

3. **Ensure console is in graphics mode:**
   ```bash
   # Switch to a virtual terminal (Ctrl+Alt+F1-F6)
   # Ensure framebuffer is enabled in kernel
   cat /proc/cmdline | grep vga
   ```

4. **Set permissions (if needed):**
   ```bash
   # Option 1: Run as root
   sudo ./bin/example_counter --fb
   
   # Option 2: Add user to video group
   sudo usermod -a -G video $USER
   # Then log out and log back in
   ```

### Running in Framebuffer Mode

Use the `--fb` or `--framebuffer` flag with any example:

```bash
# Use default framebuffer device (/dev/fb0)
./bin/example_counter --fb

# Specify a different framebuffer device
./bin/example_counter --fb /dev/fb1

# Show help
./bin/example_counter --help
```

### Framebuffer Mode Details

- **Resolution**: Automatically detects framebuffer resolution (no manual width/height needed)
- **Color Depth**: Supports 16, 24, and 32-bit color depths
- **Input Handling**: Basic input support is available but may need additional implementation for your use case
- **Performance**: Direct memory mapping provides excellent performance
- **No Dependencies**: Works without X11, Wayland, or any window manager

### Troubleshooting

**Problem: Permission denied**
```bash
# Solution: Run with sudo or add user to video group
sudo ./bin/example_counter --fb
```

**Problem: Framebuffer not found**
```bash
# Check if framebuffer device exists
ls -l /dev/fb*

# Enable framebuffer in kernel (add to kernel command line)
# vga=0x0318 or nomodeset
```

**Problem: Screen is blank or distorted**
- Verify framebuffer resolution matches your display: `fbset -i`
- Check color depth compatibility: `fbset -depth 32`
- Ensure console is not in text mode

**Problem: Build errors related to framebuffer**
- Ensure you're building on Linux (framebuffer API is Linux-specific)
- Check that `<linux/fb.h>` header is available

### Example: Running on Embedded Linux

For embedded systems or single-board computers (Raspberry Pi, BeagleBone, etc.):

```bash
# 1. Build on target system or cross-compile
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

# 2. Disable desktop environment (optional)
sudo systemctl stop lightdm  # or gdm, xdm, etc.

# 3. Switch to console
sudo systemctl set-default multi-user.target

# 4. Run application
sudo ./bin/example_counter --fb

# 5. To restore desktop, reboot or:
sudo systemctl set-default graphical.target
```

### Architecture Notes

The framebuffer renderer (`FramebufferRenderer`) implements the same interface as `SDL2Renderer`, making it a drop-in replacement. Key differences:

- Direct memory mapping of framebuffer device
- No window management overhead
- Bitmap font rendering (no TTF dependencies)
- Simplified event handling (extendable via `/dev/input` devices)

See `include/reactpp/renderer/FramebufferRenderer.hpp` for the full API.

## Project Structure

```
.
├── include/reactpp/     # Public headers
│   ├── core/            # Core data structures
│   ├── hooks/           # Hooks system
│   ├── elements/        # Primitive UI elements
│   ├── renderer/        # Rendering engine
│   └── ...
├── src/                 # Implementation files
├── tests/               # Unit tests
├── examples/            # Example applications
└── docs/               # Documentation
```

## Usage Example

```cpp
#include "reactpp/ReactPP.hpp"

using namespace reactpp;
using namespace reactpp::elements;

class Counter : public Component {
    VNode::Ptr render() override {
        // TODO: Use hooks when implemented
        return View({}, {
            Text("Count: 0"),
            Button({}, {Text("Increment")})
        });
    }
};
```

## Status

This project is in active development. Core infrastructure is being built according to the roadmap in `docs/Impl.MD`.

## License

[Add your license here]

