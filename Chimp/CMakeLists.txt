cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Chimp")

add_subdirectory("ChimpGame")

add_executable (Chimp "src/main.cpp" "src/Chimp.h"  "src/api/scenes/Scene.h" "src/stdafx.h" "src/api/scenes/SceneManager.h" "src/impl/scenes/SceneManager.cpp" "src/impl/MainLoop.h" "src/impl/MainLoop.cpp" "src/api/Engine.h")

target_include_directories(Chimp PRIVATE "src/")
target_include_directories(Chimp PRIVATE "src/api/")
target_include_directories(Chimp PRIVATE "src/impl/")

target_link_libraries(Chimp PRIVATE ChimpGame)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chimp PROPERTY CXX_STANDARD 20)
endif()