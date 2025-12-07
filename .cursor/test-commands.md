# Test Commands Reference

## Using CMake Presets

### Configure and Build
```bash
# Configure with default preset
cmake --preset default

# Build all tests
cmake --build --preset tests

# Or build everything
cmake --build --preset default
```

### Run Tests

#### Run All Tests
```bash
cmake --test --preset all
```

#### Run Specific Test Suites
```bash
# Core tests only
cmake --test --preset core

# Hooks tests only
cmake --test --preset hooks

# Context tests only
cmake --test --preset context

# Events tests only
cmake --test --preset events

# Scheduler tests only
cmake --test --preset scheduler

# Renderer tests only
cmake --test --preset renderer

# Quick tests (core components only)
cmake --test --preset quick
```

#### Test Options
```bash
# Verbose output
cmake --test --preset verbose

# Parallel execution
cmake --test --preset parallel
```

### Using CTest Directly

After configuring and building, you can also use CTest directly:

```bash
cd build/default
ctest

# Verbose output
ctest -V

# Run specific test
ctest -R VNodeTest

# Run tests in parallel
ctest -j 4

# Show test output on failure
ctest --output-on-failure
```

### Using Helper Scripts

#### Windows (PowerShell)
```powershell
.\run-tests.ps1
.\run-tests.ps1 core    # Run only core tests
```

#### Linux/macOS (Bash)
```bash
chmod +x run-tests.sh
./run-tests.sh
./run-tests.sh core    # Run only core tests
```

## Available Test Executables

All test executables are built in `build/default/bin/` (or your configured binary directory):

- `test_vnode` - VNode tree operations
- `test_props` - Props type safety
- `test_component` - Component lifecycle
- `test_fiber` - FiberNode structure
- `test_reconciler` - Reconciliation (placeholder)
- `test_hook_rules` - HookManager rules
- `test_useState` - useState hook (placeholder)
- `test_useEffect` - useEffect hook (placeholder)
- `test_useRef` - useRef hook (placeholder)
- `test_useMemo` - useMemo hook (placeholder)
- `test_useCallback` - useCallback hook (placeholder)
- `test_useReducer` - useReducer hook (placeholder)
- `test_context` - Context API
- `test_synthetic_events` - Synthetic events
- `test_event_dispatcher` - Event dispatcher
- `test_event_manager` - Event manager
- `test_scheduler` - Update scheduler
- `test_layout` - Layout engine
- `test_style_resolver` - Style resolver
- `test_render_tree` - Render tree

## Running Individual Test Executables

You can also run test executables directly:

```bash
# Windows
build\default\bin\test_vnode.exe

# Linux/macOS
./build/default/bin/test_vnode
```

## IDE Integration

### Visual Studio Code
1. Install CMake Tools extension
2. Select a configure preset from the status bar
3. Build using the build button
4. Run tests using the test button or command palette

### CLion
1. Open CMakePresets.json
2. CLion will automatically detect presets
3. Select preset from build configuration dropdown
4. Run tests from Run menu

### Visual Studio
1. Open CMakeLists.txt
2. Visual Studio will detect CMakePresets.json
3. Select preset from configuration dropdown
4. Build and test from Solution Explorer

## Continuous Integration

Example GitHub Actions workflow:

```yaml
- name: Configure
  run: cmake --preset default

- name: Build
  run: cmake --build --preset tests

- name: Test
  run: cmake --test --preset all
```

