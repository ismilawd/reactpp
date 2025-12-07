# ReactCPP Framework - Project Memory

## Project Overview
ReactCPP is a React-like GUI framework implemented in C++17, following the specifications in `docs/Impl.MD` and `docs/README.MD`. The project uses CMake for building and targets multiple platforms (Linux/DRM, Windows, macOS).

## Current Implementation Status

### ✅ Fully Implemented Components

#### Core Data Structures
- **VNode** (`include/reactcpp/core/VNode.hpp`): Complete implementation
  - Factory methods: createElement, createText, createComponent, createFragment
  - Tree operations: appendChild, removeChild, replaceChild, insertBefore
  - Traversal: traversePreOrder, traversePostOrder, traverseLevelOrder
  - Finding: findChildByKey, findById
  - Cloning: cloneShallow, cloneDeep
  - Serialization: serialize() method
  - Thread-safe ID generation using atomic
  - Inherits from std::enable_shared_from_this for proper shared_ptr management

- **Props** (`include/reactcpp/core/Props.hpp`): Complete implementation
  - Type-safe property storage using std::any
  - Type tracking with std::type_index
  - Methods: set, get, tryGet, has, remove, merge
  - Equality comparison (shallow)
  - Iterator support
  - Exception handling for type mismatches

- **Component** (`include/reactcpp/core/Component.hpp`): Complete implementation
  - Pure virtual render() method
  - Lifecycle methods: onCreate, onMount, onUpdate, onUnmount
  - Props storage and access
  - HookManager integration
  - Component ID generation (thread-safe)
  - Display name for debugging

- **ComponentInstance** (`include/reactcpp/core/ComponentInstance.hpp`): Complete implementation
  - Instance creation and destruction
  - Lifecycle management (mount/unmount)
  - Hook manager association
  - Props updates

- **FiberNode** (`include/reactcpp/core/FiberNode.hpp`): Complete structure
  - Fiber tree structure (return_, child, sibling, alternate)
  - Effect tags (Placement, Update, Deletion)
  - Priority levels
  - Props and state storage
  - Inherits from std::enable_shared_from_this

#### Hooks Infrastructure
- **HookManager** (`include/reactcpp/hooks/HookManager.hpp`): Complete implementation
  - Hook state storage per component
  - Index tracking for hook call order
  - Validation for hook rules
  - Dirty flag for re-render triggering

#### Rendering
- **SDL2Renderer** (`include/reactcpp/renderer/SDL2Renderer.hpp`): Complete implementation
  - Window creation and management
  - Drawing primitives: drawRect, fillRect
  - Color utilities: rgba, rgb
  - Event polling
  - Proper cleanup and RAII

#### UI Elements
- **View, Text, Button, Input** (`include/reactcpp/elements/`): Factory functions implemented
  - Convenience functions for common use cases
  - Props integration

### 🚧 Stubbed/Placeholder Components (Headers exist, implementation pending)

#### Hooks (Headers with template signatures, implementations pending)
- useState, useEffect, useRef, useMemo, useCallback, useReducer, useContext
- All have header files with proper signatures but empty .cpp files

#### Context API
- Context template class (basic structure implemented)
- ContextManager (basic structure, needs full implementation)
- ContextProvider component (template structure)

#### Events
- SyntheticEvent (basic structure, needs full implementation)
- EventDispatcher (structure defined, needs capture/bubble implementation)
- EventManager (structure defined, needs full handler management)

#### Scheduler
- UpdateScheduler (priority queue structure, needs work processing)
- WorkLoop (structure defined, needs main loop implementation)

#### Renderer (Partial)
- LayoutEngine (structure defined, needs flexbox algorithm)
- StyleResolver (structure defined, needs style computation)
- RenderTree (structure defined, needs tree update logic)

#### Reconciliation
- Reconciler (header exists, needs diff algorithm implementation)

### 📁 Project Structure

```
.
├── CMakeLists.txt              # Main CMake configuration
├── include/reactcpp/           # Public headers
│   ├── ReactCPP.hpp           # Main include file
│   ├── core/                  # Core data structures
│   │   ├── VNode.hpp          ✅ Complete
│   │   ├── Props.hpp          ✅ Complete
│   │   ├── Component.hpp      ✅ Complete
│   │   ├── ComponentInstance.hpp ✅ Complete
│   │   ├── FiberNode.hpp      ✅ Structure complete
│   │   └── Reconciler.hpp     🚧 Stub
│   ├── hooks/                 # Hooks system
│   │   ├── HookManager.hpp    ✅ Complete
│   │   ├── useState.hpp       🚧 Stub
│   │   ├── useEffect.hpp      🚧 Stub
│   │   ├── useRef.hpp         🚧 Stub
│   │   ├── useMemo.hpp        🚧 Stub
│   │   ├── useCallback.hpp    🚧 Stub
│   │   ├── useReducer.hpp     🚧 Stub
│   │   └── useContext.hpp     🚧 Stub
│   ├── elements/              # UI primitives
│   │   ├── View.hpp           ✅ Complete
│   │   ├── Text.hpp          ✅ Complete
│   │   ├── Button.hpp        ✅ Complete
│   │   ├── Input.hpp         ✅ Complete
│   │   └── Elements.hpp      ✅ Complete
│   ├── renderer/             # Rendering engine
│   │   ├── SDL2Renderer.hpp  ✅ Complete
│   │   ├── LayoutEngine.hpp   🚧 Stub
│   │   ├── StyleResolver.hpp  🚧 Stub
│   │   └── RenderTree.hpp    🚧 Stub
│   ├── context/              # Context API
│   │   ├── Context.hpp       🚧 Partial
│   │   ├── ContextManager.hpp 🚧 Stub
│   │   └── ContextProvider.hpp 🚧 Stub
│   ├── events/               # Event system
│   │   ├── SyntheticEvent.hpp 🚧 Stub
│   │   ├── EventDispatcher.hpp 🚧 Stub
│   │   └── EventManager.hpp  🚧 Stub
│   ├── scheduler/            # Update scheduling
│   │   ├── UpdateScheduler.hpp 🚧 Stub
│   │   └── WorkLoop.hpp      🚧 Stub
│   └── platform/             # Platform abstraction
│       └── Platform.hpp      🚧 Stub
├── src/                      # Implementation files
│   └── [mirrors include structure]
├── tests/                     # Unit tests
│   ├── core/                 ✅ Tests for VNode, Props, Component, FiberNode
│   ├── hooks/                🚧 Placeholder tests
│   ├── context/              🚧 Placeholder tests
│   ├── events/               🚧 Placeholder tests
│   ├── scheduler/            🚧 Placeholder tests
│   └── renderer/            🚧 Placeholder tests
└── examples/                  # Example applications
    ├── simple/               ✅ Basic component example
    ├── counter/              ✅ Counter with state (manual)
    └── todo/                  ✅ Todo app structure

```

## Key Architectural Decisions

1. **Memory Management**: Uses smart pointers (shared_ptr, unique_ptr) throughout. VNode and FiberNode inherit from enable_shared_from_this for proper shared ownership.

2. **Type Safety**: Props system uses std::any with type_index tracking for runtime type checking. Template-based APIs for compile-time type safety.

3. **Thread Safety**: ID generation uses atomic counters. Main rendering assumed single-threaded (no locking in most places).

4. **Component Model**: Both class-based (inheriting Component) and functional components (via function pointers) are supported.

5. **Build System**: CMake 3.15+ with support for:
   - SDL2 (rendering)
   - nlohmann/json (serialization)
   - Google Test (testing)
   - Cross-platform support

## Testing Status

- ✅ Core tests: VNode, Props, Component have comprehensive tests
- ✅ Basic tests: FiberNode, HookManager, Context creation
- 🚧 Placeholder tests: All other modules have stub test files

## Dependencies

- **Required**: C++17, SDL2, nlohmann/json, Google Test
- **Optional**: (Future) Skia, FreeType, stb_image

## Build Configuration

- CMake options: `BUILD_TESTS`, `BUILD_EXAMPLES`
- Output directories: `bin/`, `lib/`
- Test discovery: Uses GoogleTest's gtest_discover_tests

## Next Implementation Priorities

Based on `docs/Impl.MD` roadmap:

1. **Phase 1 Complete**: Foundation (VNode, Props, Component, Primitives) ✅
2. **Phase 2 Next**: Rendering Engine
   - Complete LayoutEngine (flexbox algorithm)
   - Complete StyleResolver
   - Complete RenderTree
   - Initial render pipeline
3. **Phase 3**: Component System (mostly done, needs functional component support)
4. **Phase 4**: State Management (useState implementation)
5. **Phase 5**: Hooks System (complete all hooks)
6. **Phase 6+**: Context, Events, Advanced Features

## Known Issues / TODOs

1. VNode::shared_from_this() requires VNode to be managed by shared_ptr (this is correct usage)
2. Hook implementations are stubs - need full useState, useEffect, etc.
3. Reconciliation algorithm not implemented
4. Event system needs capture/bubble phase implementation
5. Layout engine needs flexbox algorithm
6. Style resolution needs cascade and inheritance logic
7. Render tree update logic needs implementation
8. Work loop needs time-slicing and yielding
9. Examples use manual state management - need hooks for proper React-like API

## Code Style Notes

- Namespace: `reactcpp` for main namespace, nested namespaces for modules
- Naming: PascalCase for classes, camelCase for methods, UPPER_CASE for constants
- Smart pointers: Prefer shared_ptr for shared ownership, unique_ptr for exclusive
- RAII: All resources properly managed with constructors/destructors

## File Organization

- Headers in `include/reactcpp/` mirror namespace structure
- Implementation in `src/` mirrors header structure
- Tests in `tests/` mirror source structure
- Examples in `examples/` with descriptive names

## Important Implementation Details

1. **VNode IDs**: Thread-safe atomic counter starting at 1
2. **Component IDs**: Thread-safe atomic counter starting at 1
3. **Props Type Safety**: Throws std::runtime_error on type mismatch with descriptive messages
4. **Hook Rules**: HookManager validates call order (enforcement in place, needs integration)
5. **SDL2 Integration**: Proper initialization and cleanup, window management complete

This memory should be updated as implementation progresses.

