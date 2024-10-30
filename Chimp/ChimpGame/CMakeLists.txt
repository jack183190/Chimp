cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChimpGame")

add_library (ChimpGame STATIC "src/GameMain.cpp" "src/GameRenderer.h" "src/GameRenderer.cpp" "src/menu/MenuScene.h" "src/menu/MenuScene.cpp" "src/networking/GameClient.h" "src/networking/GameClient.cpp"  "src/stdafx.h"  "src/networking/GameServer.h" "src/networking/GameServer.cpp" "src/Logger.h" "src/game/GameScene.h" "src/game/GameScene.cpp" "src/networking/Networking.h" "src/networking/packets/ClientMatchStartPacket.h" "src/networking/server/MatchHandler.h" "src/networking/server/MatchHandler.cpp" "src/networking/server/ServerMatch.h" "src/networking/matches/MatchResult.h" "src/networking/server/ServerMatch.cpp" "src/networking/server/MatchSet.h" "src/networking/server/MatchSet.cpp" "src/networking/packets/Packets.h" "src/networking/client/ClientCurrentMatchHandler.h" "src/networking/client/ClientCurrentMatchHandler.cpp")

# Add src as include dir
target_include_directories(ChimpGame PUBLIC "src/")

# Include chimp
add_compile_definitions(CHIMP_FLECS=1)
target_include_directories(ChimpGame PUBLIC "../src/")
target_link_libraries(ChimpGame PRIVATE imgui)
target_link_libraries(ChimpGame PRIVATE flecs::flecs_static)

add_compile_definitions(GAME_SRC="${CMAKE_CURRENT_SOURCE_DIR}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChimpGame PROPERTY CXX_STANDARD 20)
endif()