cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChimpGame")

add_library (ChimpGame STATIC "src/GameMain.cpp" "src/TestScene.h" "src/TestScene.cpp" "src/GameRenderer.h" "src/GameRenderer.cpp")

# FLECS
add_compile_definitions(CHIMP_FLECS=1)
FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG master
)
FetchContent_MakeAvailable(flecs)
target_link_libraries(ChimpGame PRIVATE flecs::flecs_static)

target_include_directories(ChimpGame PUBLIC "../src/") # Chimp engine include directoryv

add_compile_definitions(GAME_SRC="${CMAKE_CURRENT_SOURCE_DIR}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChimpGame PROPERTY CXX_STANDARD 20)
endif()