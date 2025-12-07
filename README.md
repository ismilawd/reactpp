# ReactPP - React-like GUI Framework in C++

A modern GUI framework in C++ that mimics React's declarative paradigm, component model, and state management patterns.

## Features

- **Virtual DOM System**: Efficient tree structure with diffing and reconciliation
- **Component System**: Class-based and functional components with lifecycle management
- **Hooks System**: useState, useEffect, useRef, useMemo, useCallback, useReducer, useContext
- **Context API**: Prop drilling avoidance
- **Event System**: Synthetic events with bubbling/capturing
- **Rendering**: SDL2 backend with flexbox layout engine
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

