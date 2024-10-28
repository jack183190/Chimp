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
add_executable (Chimp "src/Main.cpp" "src/Chimp.h"  "src/api/scenes/Scene.h" "src/stdafx.h" "src/api/scenes/SceneManager.h" "src/impl/scenes/SceneManager.cpp" "src/impl/MainLoop.h" "src/impl/MainLoop.cpp" "src/api/Engine.h" "src/impl/time/TimeManager.cpp" "src/impl/Engine.cpp" "src/api/window/IWindow.h" "src/api/window/InputManager.h" "src/api/window/Keys.h" "src/impl/window/InputManager.cpp" "src/impl/window/WindowGLFW.h" "src/impl/window/IWindow.cpp" "src/impl/window/WindowGLFW.cpp" "src/api/utils/Maths.h"  "src/api/graphics/buffers/IBuffer.h" "src/api/graphics/IRenderingManager.h" "src/api/graphics/buffers/Usage.h" "src/preprocessor/CopyAndMove.h" "src/impl/graphics/opengl/RenderingManager.h" "src/impl/graphics/opengl/RenderingManager.cpp" "src/impl/graphics/opengl/buffers/Buffer.h" "src/impl/graphics/opengl/buffers/Buffer.cpp" "src/impl/graphics/opengl/Translations.h"  "src/api/graphics/buffers/IElementArray.h"  "src/api/utils/RawArray.h" "src/impl/utils/RawArray.cpp" "src/impl/graphics/interfaces/buffers/IElementArray.cpp" "src/impl/graphics/opengl/buffers/ElementArray.h" "src/impl/graphics/opengl/buffers/ElementArray.cpp" "src/api/graphics/buffers/IElementArrayLayout.h" "src/impl/graphics/interfaces/buffers/IElementArrayLayout.cpp" "src/api/graphics/GraphicsType.h" "src/impl/graphics/opengl/buffers/ElementArrayLayout.cpp" "src/impl/graphics/interfaces/GraphicsType.cpp" "src/api/utils/FileReader.h" "src/impl/utils/FileReader.cpp"  "src/api/graphics/shaders/IShader.h" "src/api/graphics/shaders/IShaderCompiler.h" "src/api/graphics/shaders/ShaderTypes.h" "src/impl/utils/NotNullHack.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.cpp" "src/impl/graphics/opengl/shaders/Shader.h" "src/impl/graphics/opengl/shaders/Shader.cpp"   "src/api/graphics/IRenderer.h" "src/impl/graphics/opengl/Renderer.h" "src/impl/graphics/opengl/Renderer.cpp" "src/api/graphics/PrimitiveType.h" "src/impl/graphics/interfaces/PrimitiveType.cpp" "src/api/graphics/meshes/Mesh.h" "src/impl/graphics/meshes/Mesh.cpp" "src/impl/graphics/interfaces/IRenderer.cpp" "src/api/graphics/camera/CameraMatrices.h" "src/impl/utils/Maths.cpp" "src/impl/graphics/camera/CameraMatrices.cpp" "src/api/graphics/camera/Camera.h" "src/impl/graphics/camera/Camera.cpp" "src/api/graphics/camera/ICamera.h" "src/api/graphics/shaders/IShaderBuffers.h" "src/impl/graphics/interfaces/shaders/IShaderBuffers.cpp" "src/impl/graphics/opengl/shaders/ShaderBuffers.h" "src/api/graphics/images/IImageLoader.h" "src/impl/graphics/images/interfaces/IImageLoader.cpp" "src/impl/graphics/images/stb/ImageLoader.h" "src/impl/graphics/images/stb/ImageLoader.cpp"  "src/api/graphics/textures/ITexture.h" "src/api/graphics/textures/TextureSlot.h" "src/impl/graphics/interfaces/textures/ITexture.cpp" "src/impl/graphics/opengl/textures/Texture.h" "src/impl/graphics/opengl/textures/Texture.cpp" "src/api/graphics/textures/TextureProperties.h" "src/impl/graphics/interfaces/IRenderingManager.cpp" "src/impl/graphics/images/stb/STBInit.cpp" "src/api/graphics/meshes/TexturedQuad.h" "src/impl/graphics/meshes/TexturedQuad.cpp" "src/api/graphics/camera/controllers/DebugCameraController.h" "src/impl/graphics/camera/controllers/DebugCameraController.cpp" "src/impl/graphics/interfaces/shaders/IShader.cpp" "src/impl/graphics/interfaces/buffers/IBuffer.cpp"  "src/api/assets/AssetManager.h" "src/impl/assets/AssetManager.cpp" "src/api/utils/HashCombine.h"  "src/api/ecs/ECS.h"  "src/api/utils/OptionalReference.h"  "src/api/networking/IServer.h"  "src/api/networking/EventHandler.h" "src/api/networking/events/NetworkEventType.h" "src/impl/networking/enet/Server.h" "src/impl/networking/enet/Server.cpp" "src/impl/networking/enet/InitENet.h" "src/impl/networking/enet/InitENet.cpp"  "src/impl/networking/enet/ServerFactory.h" "src/impl/networking/enet/ServerFactory.cpp" "src/impl/networking/IServer.cpp" "src/api/utils/ThreadQueue.h"  "src/api/networking/IClient.h" "src/impl/networking/IClient.cpp" "src/impl/networking/enet/Client.h" "src/impl/networking/enet/Client.cpp" "src/api/networking/PacketTypeRegistry.h"  "src/api/networking/PacketResponseFunc.h" "src/api/logging/Logger.h" "src/impl/logging/Logger.cpp" "src/Loggers.h" )

# Include directories
target_include_directories(Chimp PRIVATE "src/")
target_include_directories(Chimp PRIVATE "src/api/")
target_include_directories(Chimp PRIVATE "src/impl/")

target_link_libraries(Chimp PRIVATE ChimpGame)

# GLM
# TODO convert to a shell script to support other platforms, or maybe as a git submodule
add_compile_definitions(CHIMP_GLM=1)
execute_process(
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat glm https://github.com/g-truc/glm.git 1.0.1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# STB
add_compile_definitions(CHIMP_STB=1)
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat stb https://github.com/nothings/stb.git master
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# FLECS
add_compile_definitions(CHIMP_FLECS=1)
FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG master
)
FetchContent_MakeAvailable(flecs)
target_link_libraries(Chimp PRIVATE flecs::flecs_static)

# ENET
# Ended up having a lot of issues with linking ENET
# This code basically clones the repo, builds it with cmake from a batch script, then links it
# there is definitely a better way to do this, but it works
# you do need cmake and git in your env path though
# also it uses batch so thats windows only
add_compile_definitions(CHIMP_ENET=1)
set(ENET_BUILD_TYPE "Debug")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ENET_BUILD_TYPE "Release")
endif()
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/clone_lib.bat enet/enet-src https://github.com/lsalzman/enet.git master
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
execute_process(
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/enet_build.bat ${ENET_BUILD_TYPE}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/enet
)
set(ENET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/enet")
include_directories(${ENET_DIR}/enet-src/include)
set(ENET_LIB "${ENET_DIR}/enet-build/${ENET_BUILD_TYPE}/enet.lib")
target_link_libraries(Chimp PRIVATE ${ENET_LIB} winmm)

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

# OpenGL
if (CHIMP_PLATFORM EQUAL CHIMP_PLATFORM_WINDOWS)
add_compile_definitions(CHIMP_OPENGL=1)
find_package(OpenGL REQUIRED)
target_link_libraries(Chimp PRIVATE OpenGL::GL)
# GLEW (OpenGL Extension Wrangler Library)
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG glew-cmake-2.2.0
)
FetchContent_MakeAvailable(glew)
target_link_libraries(Chimp PRIVATE libglew_static)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chimp PROPERTY CXX_STANDARD 20)
endif()