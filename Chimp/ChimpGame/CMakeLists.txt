cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChimpGame")

add_library (ChimpGame STATIC "src/GameMain.cpp")

target_include_directories(ChimpGame PUBLIC "../src/") # Chimp engine include directory

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChimpGame PROPERTY CXX_STANDARD 20)
endif()