 # ReactPP Framework - Complete Implementation Documentation

## Document Purpose

This is a comprehensive technical specification for implementing a React-like GUI framework in C/C++ with cmake build system with multi-platform support (Linux/DRM, Windows, macOS). This document serves as the single source of truth for AI-assisted implementation, providing complete architectural guidance without code examples.

---

## 1. Project Overview

### 1.1 Goals and Objectives

**Primary Goals:**
- Create a production-ready GUI framework inspired by React's component model
- Support multiple platforms with abstraction layer (Linux DRM/KMS primary, X11, Wayland, Windows, macOS)
- Implement React-like hooks system (useState, useEffect, useRef, useMemo, useCallback, useReducer)
- Build efficient Virtual DOM with Fiber-based reconciliation
- Provide event system with synthetic events and bubbling
- Support context API for prop drilling avoidance
- Ensure type-safe, memory-safe implementation in modern C++17

**Performance Targets:**
- 60 FPS rendering for typical applications
- <16ms frame time budget
- <100ms initial mount time for typical component tree
- Efficient diff algorithm (O(n) complexity)
- Minimal memory allocations during updates

---

## 2. Architecture Overview

### 2.1 System Architecture Layers

**Layer 1: Platform Abstraction**
- Window management (creation, destruction, events)
- Display information (resolution, DPI, refresh rate)
- Input device handling (keyboard, mouse, touch)
- Graphics context initialization (EGL, GLX, Win32, Cocoa)

**Layer 2: Rendering Engine**
- Abstract renderer interface
- Backend implementations (DRM/KMS, X11, Wayland, Win32, CoreGraphics)
- 2D graphics primitives (Cairo or Skia integration)
- Layout engine (Flexbox-based)
- Style resolution and cascade
- Paint operations and compositing

**Layer 3: Virtual DOM & Reconciliation**
- Virtual DOM tree structure (VNode hierarchy)
- Fiber architecture for interruptible rendering
- Reconciliation algorithm (diff/patch)
- Component instance management
- Tree traversal and updates

**Layer 4: Component System**
- Base component class with lifecycle
- Functional component support
- Props system with type erasure
- Children management
- Component registry

**Layer 5: Hooks System**
- Hook manager with call order tracking
- State hooks (useState, useReducer)
- Effect hooks (useEffect, useLayoutEffect)
- Ref hooks (useRef)
- Memoization hooks (useMemo, useCallback)
- Context hook (useContext)
- Custom hooks infrastructure

**Layer 6: Context API**
- Context creation and registration
- Provider components
- Consumer hooks
- Context propagation through tree

**Layer 7: Event System**
- Event capture and bubbling phases
- Synthetic event wrapping
- Event delegation
- Event handler attachment
- Mouse, keyboard, touch, and custom events

**Layer 8: Scheduler**
- Update queue management
- Priority-based scheduling
- Batched updates
- Time slicing for large updates
- Render queue with frame synchronization

---

## 3. Core Data Structures Documentation

### 3.1 Virtual DOM Node (VNode)

**Purpose:** Represent nodes in the virtual DOM tree

**Requirements:**
- Support four node types: Element, Text, Component, Fragment
- Store element tag/type as string
- Store props container
- Maintain ordered children list
- Optional key for reconciliation optimization
- Unique ID for tracking
- Weak parent pointer to avoid cycles
- Component instance pointer for component nodes

**Key Operations:**
- Factory methods for each node type
- Tree manipulation (appendChild, removeChild, replaceChild, insertBefore)
- Tree traversal (pre-order, post-order, level-order)
- Finding children by key
- Deep and shallow cloning
- Serialization for debugging

**Memory Management:**
- Use shared_ptr for ownership
- Use weak_ptr for parent reference
- Ensure no circular references
- Implement proper RAII cleanup

**Thread Safety:**
- ID generation must be thread-safe (atomic)
- Tree modifications not thread-safe (called from single render thread)

**AI Implementation Task:**
- Create header file `include/reactpp/core/VNode.hpp`
- Implement all methods in `src/core/VNode.cpp`
- Write exhaustive unit tests in `tests/core/test_vnode.cpp`
- Test coverage target: >95%
- Tests must verify: all factory methods, tree operations, traversal correctness, clone deep/shallow, memory leak detection, thread-safe ID generation, edge cases

---

### 3.2 Properties System (Props)

**Purpose:** Type-safe, type-erased property container for components

**Requirements:**
- Store arbitrary types using std::any
- Track type information with std::type_index
- Support get/set/has/remove operations
- Provide type-safe get with exceptions on mismatch
- Provide tryGet returning optional
- Support event handler storage (std::function)
- Support merging props objects
- Implement equality comparison (shallow)
- Iterator support for key enumeration

**Type Safety:**
- Throw std::runtime_error on type mismatch with descriptive message
- Store original type information for debugging
- Provide type query methods

**Performance Considerations:**
- Use std::unordered_map for O(1) access
- Avoid unnecessary copying of std::any
- Efficient comparison for reconciliation

**AI Implementation Task:**
- Create header file `include/reactpp/core/Props.hpp`
- Implement in `src/core/Props.cpp`
- Write comprehensive tests in `tests/core/test_props.cpp`
- Test coverage target: >98%
- Tests must verify: type safety with multiple types, get/set/remove operations, merge behavior, equality operator, iterator usage, exception handling, memory management with std::any, performance with large prop sets

---

### 3.3 Component Base Class

**Purpose:** Abstract base for all components (class-based and functional)

**Requirements:**
- Pure virtual render() method returning VNode::Ptr
- Virtual lifecycle methods:
  - onCreate() - initialization
  - onMount() - after first render to DOM
  - onUpdate(oldProps, newProps) - before re-render
  - onUnmount() - cleanup before removal
- Props storage and access
- Reference to HookManager for hooks support
- Component ID for tracking
- Display name for debugging

**Lifecycle Guarantees:**
- onCreate called once during instantiation
- onMount called after initial render committed
- onUpdate called before each re-render (except first)
- onUnmount called before destruction
- Hooks only callable during render phase

**AI Implementation Task:**
- Create header `include/reactpp/core/Component.hpp`
- Implement base functionality in `src/core/Component.cpp`
- Write tests in `tests/core/test_component.cpp`
- Test coverage target: >90%
- Tests must verify: lifecycle method call order, props access, hook manager integration, proper cleanup, exception safety

---

### 3.4 Component Instance Manager

**Purpose:** Manage component lifecycle and instances

**Requirements:**
- Create and destroy component instances
- Track instance-to-VNode mapping
- Call lifecycle methods at appropriate times
- Manage hook state per instance
- Handle component updates (props changes)
- Cleanup on unmount

**State Management:**
- Map ComponentId to Component instances
- Map ComponentId to HookManager instances
- Track mount status
- Queue cleanup operations

**Thread Safety:**
- All operations called from single render thread
- No locking required if single-threaded guarantee maintained

**AI Implementation Task:**
- Create header `include/reactpp/core/ComponentInstance.hpp`
- Implement in `src/core/ComponentInstance.cpp`
- Write tests in `tests/core/test_component.cpp`
- Test coverage target: >90%
- Tests must verify: creation/destruction, lifecycle ordering, props updates, hook state persistence, cleanup completeness, memory leak prevention

---

### 3.5 Fiber Node Architecture

**Purpose:** Support interruptible rendering and time slicing

**Requirements:**
- Mirror VNode structure with additional fields
- Store work-in-progress state
- Track pending updates
- Support priority levels
- Link to alternate fiber (current vs work-in-progress)
- Store effect lists for commit phase

**Fiber Fields:**
- type: Element, Component, Text, Fragment
- stateNode: Reference to actual DOM or component instance
- return: Parent fiber
- child: First child fiber
- sibling: Next sibling fiber
- alternate: Alternate fiber for double-buffering
- effectTag: Type of work (Placement, Update, Deletion)
- updateQueue: Pending updates
- memoizedProps: Props from last render
- memoizedState: State from last render
- pendingProps: New props for this render
- priority: Update priority level

**Reconciliation Process:**
1. Begin work phase (process fiber tree)
2. Complete work phase (create effects list)
3. Commit phase (apply effects to real DOM)

**AI Implementation Task:**
- Create header `include/reactpp/core/FiberNode.hpp`
- Implement in `src/core/FiberNode.cpp`
- Write tests in `tests/core/test_fiber.cpp`
- Test coverage target: >85%
- Tests must verify: fiber tree construction, double buffering, effect list generation, priority handling, memory management

---

## 4. Hooks System Documentation

### 4.1 Hook Manager

**Purpose:** Track and manage hook state per component instance

**Requirements:**
- Store hook state array per component
- Track current hook index during render
- Validate hook call order (React's rules of hooks)
- Reset index between renders
- Provide hook registration API
- Support custom hooks

**Hook Rules Enforcement:**
- Hooks only called during render phase
- Same number of hooks each render
- Same order of hooks each render
- No conditional hooks
- Throw exceptions on rule violations with clear messages

**State Structure:**
- Array of hook states (std::vector<std::any>)
- Current index (size_t)
- Component ID association
- Dirty flag for re-render triggering

**AI Implementation Task:**
- Create header `include/reactpp/hooks/HookManager.hpp`
- Implement in `src/hooks/HookManager.cpp`
- Write tests in `tests/hooks/test_hook_rules.cpp`
- Test coverage target: >95%
- Tests must verify: rule enforcement, index tracking, state persistence, exception messages, memory cleanup, edge cases

---

### 4.2 useState Hook

**Purpose:** Add local state to functional components

**Interface:**
- Input: Initial state value or initializer function
- Output: Pair of [currentState, setStateFunction]

**Behavior:**
- On first call: Initialize with initial value/function result
- On subsequent calls: Return current state
- setState: Schedule update and re-render
- setState with function: Receive previous state as argument
- Batched updates: Multiple setState in same event batched

**Implementation Details:**
- Store state value in hook manager
- setState generates update in fiber
- State updates enqueued in scheduler
- Functional updates applied during commit

**AI Implementation Task:**
- Create header `include/reactpp/hooks/useState.hpp`
- Implement in `src/hooks/useState.cpp`
- Write tests in `tests/hooks/test_useState.cpp`
- Test coverage target: >95%
- Tests must verify: initialization, updates, functional updates, batching, re-render triggering, multiple states in component, state persistence across renders

---

### 4.3 useEffect Hook

**Purpose:** Perform side effects after render

**Interface:**
- Input: Effect function, optional dependency array
- Effect function returns optional cleanup function

**Behavior:**
- Effect runs after render committed to DOM
- Cleanup runs before next effect or unmount
- Dependencies: Run only when dependencies change
- Empty array: Run once on mount
- No array: Run after every render

**Cleanup Semantics:**
- Cleanup runs before next effect execution
- Cleanup runs before unmount
- Cleanup runs synchronously during commit

**Implementation Details:**
- Store effect function and dependencies in hook state
- Compare dependencies on each render (shallow equality)
- Queue effects during render phase
- Execute effects in commit phase after DOM updates
- Store cleanup function for next render

**AI Implementation Task:**
- Create header `include/reactpp/hooks/useEffect.hpp`
- Implement in `src/hooks/useEffect.cpp`
- Write tests in `tests/hooks/test_useEffect.cpp`
- Test coverage target: >95%
- Tests must verify: execution timing, cleanup execution, dependency comparison, mount/unmount behavior, multiple effects, effect ordering, exception handling

---

### 4.4 useRef Hook

**Purpose:** Persist mutable value across renders without triggering re-render

**Interface:**
- Input: Initial value
- Output: Ref object with 'current' property

**Behavior:**
- Returns same ref object on every render
- Mutating 'current' doesn't trigger re-render
- Persists across component lifetime
- Common use: Storing DOM references or mutable values

**Implementation Details:**
- Store ref object in hook state
- Return same object reference each render
- No comparison or update logic needed

**AI Implementation Task:**
- Create header `include/reactpp/hooks/useRef.hpp`
- Implement in `src/hooks/useRef.cpp`
- Write tests in `tests/hooks/test_useRef.cpp`
- Test coverage target: >90%
- Tests must verify: initialization, persistence, mutation without re-render, multiple refs, object identity stability

---

### 4.5 useMemo Hook

**Purpose:** Memoize expensive computations

**Interface:**
- Input: Computation function, dependency array
- Output: Memoized value

**Behavior:**
- Compute value on first render
- Recompute only when dependencies change
- Return cached value if dependencies unchanged
- Dependencies: Required array

**Implementation Details:**
- Store [value, dependencies] in hook state
- Compare dependencies each render
- Only call computation function when needed
- Cache result for subsequent renders

**Performance Considerations:**
- Comparison cost vs computation cost tradeoff
- Shallow equality for dependencies
- Document when to use vs when overhead not worth it

**AI Implementation Task:**
- Create header `include/reactpp/hooks/useMemo.hpp`
- Implement in `src/hooks/useMemo.cpp`
- Write tests in `tests/hooks/test_useMemo.cpp`
- Test coverage target: >90%
- Tests must verify: initial computation, caching, recomputation on dep change, dependency comparison, multiple memoized values, exception safety

---

### 4.6 useCallback Hook

**Purpose:** Memoize function references

**Interface:**
- Input: Callback function, dependency array
- Output: Memoized callback

**Behavior:**
- Return same function instance when dependencies unchanged
- Create new function when dependencies change
- Useful for preventing child re-renders

**Implementation Details:**
- Similar to useMemo but for functions
- Store [function, dependencies] in hook state
- Return cached function reference

**AI Implementation Task:**
- Create header `include/reactpp/hooks/useCallback.hpp`
- Implement in `src/hooks/useCallback.cpp`
- Write tests in `tests/hooks/test_useCallback.cpp`
- Test coverage target: >90%
- Tests must verify: function identity preservation, recomputation on dep change, closure capture, multiple callbacks, integration with child props

---

### 4.7 useReducer Hook

**Purpose:** Manage complex state with reducer pattern

**Interface:**
- Input: Reducer function, initial state, optional init function
- Output: Pair of [state, dispatch]

**Reducer Function:**
- Signature: (currentState, action) => newState
- Must be pure function
- Action is arbitrary type (usually struct with type field)

**Behavior:**
- Initialize state with initial value or init(initialArg)
- dispatch(action) calls reducer and updates state
- dispatch is stable (same reference across renders)
- Multiple dispatches in same render are batched

**Implementation Details:**
- Store current state in hook state
- dispatch function queues action in update queue
- Actions processed during next render
- Reducer called in render phase with queued actions

**AI Implementation Task:**
- Create header `include/reactpp/hooks/useReducer.hpp`
- Implement in `src/hooks/useReducer.cpp`
- Write tests in `tests/hooks/test_useReducer.cpp`
- Test coverage target: >95%
- Tests must verify: initialization, dispatching, reducer calls, batching, dispatch stability, complex state updates, action types

---

### 4.8 useContext Hook

**Purpose:** Access context values without prop drilling

**Interface:**
- Input: Context object reference
- Output: Current context value

**Behavior:**
- Read value from nearest Provider up the tree
- Subscribe to context changes
- Re-render when context value changes
- Throw if no Provider found (optional default value)

**Implementation Details:**
- Look up component tree for context Provider
- Register component as context consumer
- Provider tracks consumers for update notifications
- Context change triggers re-render of consumers

**AI Implementation Task:**
- Create header `include/reactpp/hooks/useContext.hpp`
- Implement in `src/hooks/useContext.cpp`
- Write tests in `tests/context/test_context.cpp`
- Test coverage target: >90%
- Tests must verify: value reading, subscription, updates, nesting, multiple contexts, no provider error

---

## 5. Context API Documentation

### 5.1 Context Creation and Structure

**Purpose:** Share values through component tree without explicit props

**Requirements:**
- createContext function to create context objects
- Context object contains:
  - Unique ID
  - Default value
  - Display name for debugging
- Provider component type
- Consumer list for update propagation

**Context Object Interface:**
- Should be immutable after creation
- Reference equality for identity
- Type-safe value storage

**AI Implementation Task:**
- Create header `include/reactpp/context/Context.hpp`
- Implement in `src/context/ContextManager.cpp`
- Write tests in `tests/context/test_context.cpp`
- Test coverage target: >90%
- Tests must verify: context creation, uniqueness, default values, type safety

---

### 5.2 Context Provider Component

**Purpose:** Provide context value to descendant components

**Requirements:**
- Special component type recognized by reconciler
- Takes 'value' prop containing context data
- Takes 'children' prop for descendant tree
- Updates all consumers when value changes
- Uses shallow equality for change detection

**Behavior:**
- On mount: Register as provider in context manager
- On update: Check if value changed, notify consumers
- On unmount: Unregister from context manager

**Optimization:**
- Only re-render consumers if value actually changed
- Support nested providers (closer provider wins)

**AI Implementation Task:**
- Create header `include/reactpp/context/ContextProvider.hpp`
- Implement in `src/context/ContextProvider.cpp`
- Write tests in `tests/context/test_context.cpp`
- Test coverage target: >90%
- Tests must verify: value provision, consumer notifications, nesting, value change detection, cleanup

---

### 5.3 Context Manager

**Purpose:** Global registry for contexts and providers

**Requirements:**
- Maintain mapping of context ID to active providers
- Track provider hierarchy in component tree
- Find nearest provider for consumer lookup
- Notify consumers on provider value changes

**Data Structures:**
- Map: ContextId → Stack of Provider references
- Map: ProviderId → Set of Consumer components
- Component tree traversal capability

**Thread Safety:**
- Called from single render thread
- No locking required

**AI Implementation Task:**
- Create header `include/reactpp/context/ContextManager.hpp`
- Implement in `src/context/ContextManager.cpp`
- Write tests in `tests/context/test_context.cpp`
- Test coverage target: >85%
- Tests must verify: registration/unregistration, lookups, hierarchy, notifications, memory management

---

## 6. Reconciliation Algorithm

### 6.1 Reconciliation Overview

**Purpose:** Efficiently update real DOM to match Virtual DOM

**Algorithm:** Fiber-based reconciliation with two-phase commit

**Phase 1: Render Phase (Interruptible)**
- Traverse new Virtual DOM tree
- Compare with current Fiber tree
- Generate minimal set of changes (effect list)
- Can be paused and resumed

**Phase 2: Commit Phase (Synchronous)**
- Apply all changes to real DOM atomically
- Execute effects
- Call lifecycle methods
- Cannot be interrupted

**Key Optimizations:**
- Keys for list reconciliation
- Type-based heuristics (different types = full replace)
- Depth-first traversal
- Sibling reordering detection

---

### 6.2 Diffing Algorithm

**Purpose:** Determine minimal changes between old and new tree

**Rules:**
1. **Different Types:** Replace entire subtree
2. **Same Type Element:** Update props, recurse children
3. **Same Type Component:** Update props, re-render
4. **Keys:** Match elements by key for list reconciliation

**List Reconciliation:**
- Build maps of keyed children
- Match old and new children by key
- Detect moves, insertions, deletions
- Minimize DOM operations

**Output:** Effect list with operations:
- Placement: Insert new node
- Update: Update existing node
- Deletion: Remove node
- Ref updates
- Effect hooks to run

**AI Implementation Task:**
- Create header `include/reactpp/core/Reconciler.hpp`
- Implement in `src/core/Reconciler.cpp`
- Write tests in `tests/core/test_reconciler.cpp`
- Test coverage target: >85%
- Tests must verify: element updates, type changes, list reordering, key matching, effect generation, complex tree updates

---

### 6.3 Commit Phase

**Purpose:** Apply all changes to real DOM and execute effects

**Steps:**
1. **Before Mutation:**
   - Call getSnapshotBeforeUpdate (class components)
   
2. **Mutation:**
   - Apply DOM changes (insertions, deletions, updates)
   - Update refs
   - Process all Placement, Update, Deletion effects

3. **Layout:**
   - Execute useLayoutEffect hooks (synchronous)
   - Call componentDidMount/componentDidUpdate

4. **After Layout:**
   - Schedule useEffect hooks (asynchronous, after paint)

**Guarantees:**
- All DOM changes applied atomically (from user perspective)
- Lifecycle methods see consistent DOM state
- Effects run in declaration order
- Cleanup functions run before next effects

**AI Implementation Task:**
- Implement in `src/core/Reconciler.cpp` (commit methods)
- Write tests in `tests/core/test_reconciler.cpp`
- Test coverage target: >80%
- Tests must verify: operation application, lifecycle timing, effect execution, cleanup execution, error boundaries

---

## 7. Event System Documentation

### 7.1 Event Architecture

**Purpose:** Normalize browser events and provide React-like event handling

**Event Types:**
- Mouse events: click, mousedown, mouseup, mousemove, mouseenter, mouseleave
- Keyboard events: keydown, keyup, keypress
- Focus events: focus, blur, focusin, focusout
- Touch events: touchstart, touchmove, touchend (if platform supports)
- Custom events

**Event Properties:**
- Normalized across platforms
- Pooled for performance (reused objects)
- Persistent() method to keep event data
- preventDefault() and stopPropagation()

---

### 7.2 Synthetic Event Wrapper

**Purpose:** Platform-agnostic event representation

**Requirements:**
- Wrap platform-specific event objects
- Expose common properties: type, target, currentTarget, timestamp
- Mouse-specific: clientX/Y, screenX/Y, button, buttons
- Keyboard-specific: key, keyCode, altKey, ctrlKey, shiftKey, metaKey
- Pooling support: reset() method, isPersistent flag

**Event Lifecycle:**
1. Platform event received
2. Wrapped in SyntheticEvent
3. Dispatched through capture/bubble phases
4. Returned to pool (unless persist() called)

**AI Implementation Task:**
- Create header `include/reactpp/events/SyntheticEvent.hpp`
- Implement in `src/events/SyntheticEvent.cpp`
- Write tests in `tests/events/test_synthetic_events.cpp`
- Test coverage target: >90%
- Tests must verify: property access, pooling, persistence, platform wrapping, modification methods

---

### 7.3 Event Propagation

**Purpose:** Implement capture and bubble phases like DOM

**Propagation Phases:**
1. **Capture Phase:** Root to target, top-down
2. **Target Phase:** Event handlers on target element
3. **Bubble Phase:** Target to root, bottom-up

**Behavior:**
- stopPropagation() stops propagation to next element
- stopImmediatePropagation() stops propagation and subsequent handlers on same element
- preventDefault() prevents default action
- Event handlers receive SyntheticEvent reference

**Event Delegation:**
- Attach single native listener at root
- Dispatch to React handlers via propagation
- Efficient for large component trees

**AI Implementation Task:**
- Create header `include/reactpp/events/EventDispatcher.hpp`
- Implement in `src/events/EventDispatcher.cpp`
- Write tests in `tests/events/test_event_dispatcher.cpp`
- Test coverage target: >85%
- Tests must verify: capture phase, bubble phase, stopping propagation, handler ordering, delegation efficiency

---

### 7.4 Event Manager

**Purpose:** Register and manage event handlers for components

**Requirements:**
- Map VNode to event handlers
- Support multiple handlers per event type
- Handle handler addition/removal during updates
- Integrate with reconciliation for updates
- Clean up handlers on unmount

**Handler Storage:**
- Map: (NodeId, EventType) → Handler function
- Efficient lookup during event dispatch
- Handle handler updates (old removed, new added)

**AI Implementation Task:**
- Create header `include/reactpp/events/EventManager.hpp`
- Implement in `src/events/EventManager.cpp`
- Write tests in `tests/events/test_event_manager.cpp`
- Test coverage target: >85%
- Tests must verify: registration, removal, updates, cleanup, multiple handlers, memory management

---

## 8. Scheduler Documentation

### 8.1 Update Scheduler

**Purpose:** Manage update queue and prioritize work

**Priority Levels:**
- Immediate: Synchronous, must complete now (discrete user input)
- User-blocking: Should complete quickly (<250ms) (hover, scrolling)
- Normal: Normal priority (~5s)
- Low: Defer to idle time
- Idle: Can be delayed indefinitely

**Scheduling Algorithm:**
- Maintain priority queue of updates
- Process highest priority first
- Time-slice long-running work
- Yield to browser/event loop
- Batch updates from same event

**Update Batching:**
- Multiple setState calls in event handler batched
- Flush batched updates at end of event
- Controlled flushing for testing

**AI Implementation Task:**
- Create header `include/reactpp/scheduler/UpdateScheduler.hpp`
- Implement in `src/scheduler/UpdateScheduler.cpp`
- Write tests in `tests/scheduler/test_scheduler.cpp`
- Test coverage target: >80%
- Tests must verify: priority ordering, batching, time slicing, yielding, immediate vs deferred updates

---

### 8.2 Work Loop

**Purpose:** Main rendering loop processing updates

**Loop Structure:**
1. Check if work available
2. Select next unit of work (fiber)
3. Perform work (render phase)
4. Check if time expired
5. If expired and not urgent: Yield
6. If work complete: Commit phase
7. Repeat

**Time Budgeting:**
- Frame budget: ~16ms for 60fps
- Reserved time for browser work
- Configurable time slice duration
- Preemption points between fibers

**AI Implementation Task:**
- Implement in `src/scheduler/WorkLoop.cpp`
- Write tests in `tests/scheduler/test_scheduler.cpp`
- Test coverage target: >75%
- Tests must verify: loop execution, yielding, time budgeting, completion detection, error handling

---

## 9. Layout Engine Documentation

### 9.1 Flexbox Layout Algorithm

**Purpose:** Position and size components using Flexbox model

**Supported Properties:**
- **Container:** display, flexDirection, flexWrap, justifyContent, alignItems, alignContent, gap
- **Item:** flexGrow, flexShrink, flexBasis, alignSelf, order
- **Box Model:** width, height, minWidth, minHeight, maxWidth, maxHeight, margin, padding, border

**Layout Process:**
1. **Determine main/cross axis** based on flexDirection
2. **Resolve flex-basis** for each item
3. **Calculate flexible lengths** (grow/shrink)
4. **Align items** on cross axis
5. **Justify content** on main axis
6. **Apply gaps** between items
7. **Calculate final bounds** for each element

**Constraints:**
- Respect min/max width/height
- Handle aspect ratios if specified
- Text wrapping and measurement
- Nested flex containers

**AI Implementation Task:**
- Create header `include/reactpp/renderer/LayoutEngine.hpp`
- Implement in `src/renderer/LayoutEngine.cpp`
- Write tests in `tests/renderer/test_layout.cpp`
- Test coverage target: >85%
- Tests must verify: all flex properties, constraints, nesting, text layout, edge cases, performance benchmarks

---

### 9.2 Style Resolution

**Purpose:** Compute final styles with cascade and inheritance

**Style Sources:**
1. Default (user agent) styles
2. Component props (inline styles)
3. Style inheritance from parent

**Computed Values:**
- Resolve relative units (%, em, vh/vw) to absolute
- Apply inheritance (color, font, etc.)
- Compute shorthand properties (margin, padding)
- Handle 'auto' values
- Cache computed styles per node

**AI Implementation Task:**
- Create header `include/reactpp/renderer/StyleResolver.hpp`
- Implement in `src/renderer/StyleResolver.cpp`
- Write tests in `tests/renderer/test_style_resolver.cpp`
- Test coverage target: >80%
- Tests must verify: inheritance, cascade, unit resolution, computed values, caching

---

### 9.3 Render Tree

**Purpose:** Bridge between Virtual DOM and actual rendering

**Structure:**
- Parallel tree to Virtual DOM
- Contains computed layout (x, y, width, height)
- Contains computed styles
- Contains render state (needs repaint)
- Links to VNode and Component

**Update Flow:**
1. Reconciliation produces updated VNode tree
2. Update render tree structure
3. Mark dirty nodes for layout
4. Compute layout for dirty subtrees
5. Mark for repaint
6. Paint during next frame

**AI Implementation Task:**
- Create header `include/reactpp/renderer/RenderTree.hpp`
- Implement in `src/renderer/RenderTree.cpp`
- Write tests in `tests/renderer/test_render_tree.cpp`
- Test coverage target: >80%
- Tests must verify: tree construction, updates, dirty tracking, layout integration

---

## 10. Platform Abstraction

### 10.1 Linux DRM/KMS Backend (Primary)

**Purpose:** Direct framebuffer rendering without X11/Wayland overhead

**Requirements:**
- DRM device enumeration and opening
- Mode setting (resolution, refresh rate)
- Plane configuration (primary, overlay)
- Buffer management (GBM, dumb buffers)
- Atomic mode setting for tear-free flipping
- VSync synchronization
- Input device handling via libinput

**Initialization Flow:**
1. Open DRM device (/dev/dri/card0)
2. Enumerate connectors and encoders
3. Find connected display
4. Select video mode
5. Create GBM surface
6. Initialize EGL context
7. Create framebuffers
8. Set up page flipping

**Input Handling:**
- Open input devices via libinput
- Integrate with udev for hotplug
- Process input events (keyboard, mouse, touch)
- Translate to synthetic events

**AI Implementation Task:**
- Create headers in `include/reactpp/platform/linux/`
- Implement in `src/platform/linux/DRMBackend.cpp` and related files
- Write tests in `tests/platform/test_drm.cpp`
- Test coverage target: >70% (hardware-dependent)
- Create example in `examples/drm_simple/`
- Document setup in `docs/DRM_Setup.md`

---

### 10.2 X11 Backend

**Purpose:** Traditional X11 window system support

**Requirements:**
- Window creation and management
- Event handling (Expose, KeyPress, etc.)
- Graphics context (Xlib/XCB rendering or OpenGL via GLX)
- Clipboard integration
- DPI awareness

**AI Implementation Task:**
- Create header `include/reactpp/platform/linux/X11Backend.hpp`
- Implement in `src/platform/linux/X11Backend.cpp`
- Write tests focusing on window management
- Create example application

---

### 10.3 Wayland Backend

**Purpose:** Modern Linux display server protocol support

**Requirements:**
- Wayland connection and registry
- XDG shell protocol for windows
- Shared memory buffers or EGL
- Input handling via wl_seat
- HiDPI support

**AI Implementation Task:**
- Create header `include/reactpp/platform/linux/WaylandBackend.hpp`
- Implement in `src/platform/linux/WaylandBackend.cpp`