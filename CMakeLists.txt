cmake_minimum_required(VERSION 3.15)
project(ReactPP VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find dependencies
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Source files
set(CORE_SOURCES
    src/core/VNode.cpp
    src/core/Props.cpp
    src/core/Component.cpp
    src/core/ComponentInstance.cpp
    src/core/FiberNode.cpp
    src/core/Reconciler.cpp
)

set(HOOKS_SOURCES
    src/hooks/HookManager.cpp
    src/hooks/useState.cpp
    src/hooks/useEffect.cpp
    src/hooks/useRef.cpp
    src/hooks/useMemo.cpp
    src/hooks/useCallback.cpp
    src/hooks/useReducer.cpp
    src/hooks/useContext.cpp
)

set(CONTEXT_SOURCES
    src/context/Context.cpp
    src/context/ContextManager.cpp
    src/context/ContextProvider.cpp
)

set(EVENTS_SOURCES
    src/events/SyntheticEvent.cpp
    src/events/EventDispatcher.cpp
    src/events/EventManager.cpp
)

set(SCHEDULER_SOURCES
    src/scheduler/UpdateScheduler.cpp
    src/scheduler/WorkLoop.cpp
)

set(RENDERER_SOURCES
    src/renderer/LayoutEngine.cpp
    src/renderer/StyleResolver.cpp
    src/renderer/RenderTree.cpp
    src/renderer/SDL2Renderer.cpp
)

set(PLATFORM_SOURCES
    src/platform/Platform.cpp
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${HOOKS_SOURCES}
    ${CONTEXT_SOURCES}
    ${EVENTS_SOURCES}
    ${SCHEDULER_SOURCES}
    ${RENDERER_SOURCES}
    ${PLATFORM_SOURCES}
)

# Create library
add_library(reactpp STATIC ${ALL_SOURCES})

target_link_libraries(reactpp
    PUBLIC
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        nlohmann_json::nlohmann_json
)

target_include_directories(reactpp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Tests
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    
    include(GoogleTest)
    
    # Core tests
    add_executable(test_vnode tests/core/test_vnode.cpp)
    target_link_libraries(test_vnode reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_vnode)
    
    add_executable(test_props tests/core/test_props.cpp)
    target_link_libraries(test_props reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_props)
    
    add_executable(test_component tests/core/test_component.cpp)
    target_link_libraries(test_component reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_component)
    
    add_executable(test_fiber tests/core/test_fiber.cpp)
    target_link_libraries(test_fiber reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_fiber)
    
    add_executable(test_reconciler tests/core/test_reconciler.cpp)
    target_link_libraries(test_reconciler reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_reconciler)
    
    # Hooks tests
    add_executable(test_hook_rules tests/hooks/test_hook_rules.cpp)
    target_link_libraries(test_hook_rules reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_hook_rules)
    
    add_executable(test_useState tests/hooks/test_useState.cpp)
    target_link_libraries(test_useState reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_useState)
    
    add_executable(test_useEffect tests/hooks/test_useEffect.cpp)
    target_link_libraries(test_useEffect reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_useEffect)
    
    add_executable(test_useRef tests/hooks/test_useRef.cpp)
    target_link_libraries(test_useRef reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_useRef)
    
    add_executable(test_useMemo tests/hooks/test_useMemo.cpp)
    target_link_libraries(test_useMemo reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_useMemo)
    
    add_executable(test_useCallback tests/hooks/test_useCallback.cpp)
    target_link_libraries(test_useCallback reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_useCallback)
    
    add_executable(test_useReducer tests/hooks/test_useReducer.cpp)
    target_link_libraries(test_useReducer reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_useReducer)
    
    # Context tests
    add_executable(test_context tests/context/test_context.cpp)
    target_link_libraries(test_context reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_context)
    
    # Events tests
    add_executable(test_synthetic_events tests/events/test_synthetic_events.cpp)
    target_link_libraries(test_synthetic_events reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_synthetic_events)
    
    add_executable(test_event_dispatcher tests/events/test_event_dispatcher.cpp)
    target_link_libraries(test_event_dispatcher reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_event_dispatcher)
    
    add_executable(test_event_manager tests/events/test_event_manager.cpp)
    target_link_libraries(test_event_manager reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_event_manager)
    
    # Scheduler tests
    add_executable(test_scheduler tests/scheduler/test_scheduler.cpp)
    target_link_libraries(test_scheduler reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_scheduler)
    
    # Renderer tests
    add_executable(test_layout tests/renderer/test_layout.cpp)
    target_link_libraries(test_layout reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_layout)
    
    add_executable(test_style_resolver tests/renderer/test_style_resolver.cpp)
    target_link_libraries(test_style_resolver reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_style_resolver)
    
    add_executable(test_render_tree tests/renderer/test_render_tree.cpp)
    target_link_libraries(test_render_tree reactpp GTest::gtest GTest::gtest_main)
    gtest_discover_tests(test_render_tree)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_executable(example_counter examples/counter/main.cpp)
    target_link_libraries(example_counter reactpp)
    
    add_executable(example_todo examples/todo/main.cpp)
    target_link_libraries(example_todo reactpp)
    
    add_executable(example_simple examples/simple/main.cpp)
    target_link_libraries(example_simple reactpp)
endif()

# Installation
install(TARGETS reactpp
    EXPORT ReactPPTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT ReactPPTargets
    FILE ReactPPTargets.cmake
    NAMESPACE ReactPP::
    DESTINATION lib/cmake/ReactPP
)

