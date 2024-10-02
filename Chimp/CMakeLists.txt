cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Platforms
set(CHIMP_PLATFORM_UNSUPPORTED 0)
set(CHIMP_PLATFORM_WINDOWS 1)

set(CHIMP_PLATFORM ${CHIMP_PLATFORM_UNSUPPORTED})

if(WIN32)
    set(CHIMP_PLATFORM ${CHIMP_PLATFORM_WINDOWS})
    add_compile_definitions(CHIMP_PLATFORM=CHIMP_PLATFORM_WINDOWS)
endif()

# Chimp
project ("Chimp")

include(FetchContent)

add_subdirectory("ChimpGame")

# Sources
add_executable (Chimp "src/main.cpp" "src/Chimp.h"  "src/api/scenes/Scene.h" "src/stdafx.h" "src/api/scenes/SceneManager.h" "src/impl/scenes/SceneManager.cpp" "src/impl/MainLoop.h" "src/impl/MainLoop.cpp" "src/api/Engine.h" "src/impl/time/TimeManager.cpp" "src/impl/Engine.cpp" "src/api/window/IWindow.h" "src/api/window/InputManager.h" "src/api/window/Keys.h" "src/impl/window/InputManager.cpp" "src/impl/window/WindowGLFW.h" "src/impl/window/IWindow.cpp" "src/impl/window/WindowGLFW.cpp")

# Include directories
target_include_directories(Chimp PRIVATE "src/")
target_include_directories(Chimp PRIVATE "src/api/")
target_include_directories(Chimp PRIVATE "src/impl/")

target_link_libraries(Chimp PRIVATE ChimpGame)

# GLFW
if (CHIMP_PLATFORM EQUAL CHIMP_PLATFORM_WINDOWS)
add_compile_definitions(CHIMP_GLFW=1)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)
target_link_libraries(Chimp PRIVATE glfw)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chimp PROPERTY CXX_STANDARD 20)
endif()