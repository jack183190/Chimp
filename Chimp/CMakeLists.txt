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
add_executable (Chimp "src/main.cpp" "src/Chimp.h"  "src/api/scenes/Scene.h" "src/stdafx.h" "src/api/scenes/SceneManager.h" "src/impl/scenes/SceneManager.cpp" "src/impl/MainLoop.h" "src/impl/MainLoop.cpp" "src/api/Engine.h" "src/impl/time/TimeManager.cpp" "src/impl/Engine.cpp" "src/api/window/IWindow.h" "src/api/window/InputManager.h" "src/api/window/Keys.h" "src/impl/window/InputManager.cpp" "src/impl/window/WindowGLFW.h" "src/impl/window/IWindow.cpp" "src/impl/window/WindowGLFW.cpp" "src/api/Maths.h"  "src/api/graphics/buffers/IBuffer.h" "src/api/graphics/IRenderingManager.h" "src/api/graphics/buffers/Usage.h" "src/preprocessor/CopyAndMove.h" "src/impl/graphics/opengl/RenderingManager.h" "src/impl/graphics/opengl/RenderingManager.cpp" "src/impl/graphics/opengl/buffers/Buffer.h" "src/impl/graphics/opengl/buffers/Buffer.cpp" "src/impl/graphics/opengl/Translations.h"  "src/api/graphics/buffers/IElementArray.h"  "src/api/utils/RawArray.h" "src/impl/utils/RawArray.cpp" "src/impl/graphics/interfaces/buffers/IElementArray.cpp" "src/impl/graphics/opengl/buffers/ElementArray.h" "src/impl/graphics/opengl/buffers/ElementArray.cpp" "src/api/graphics/buffers/IElementArrayLayout.h" "src/impl/graphics/interfaces/buffers/IElementArrayLayout.cpp" "src/api/graphics/GraphicsType.h" "src/impl/graphics/opengl/buffers/ElementArrayLayout.cpp" "src/impl/graphics/interfaces/GraphicsType.cpp" "src/api/utils/FileReader.h" "src/impl/utils/FileReader.cpp"  "src/api/graphics/shaders/IShader.h" "src/api/graphics/shaders/IShaderCompiler.h" "src/api/graphics/shaders/ShaderTypes.h" "src/impl/utils/NotNullHack.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.h" "src/impl/graphics/opengl/shaders/ShaderCompiler.cpp" "src/impl/graphics/opengl/shaders/Shader.h" "src/impl/graphics/opengl/shaders/Shader.cpp"   "src/api/graphics/IRenderer.h" "src/impl/graphics/opengl/Renderer.h" "src/impl/graphics/opengl/Renderer.cpp" "src/api/graphics/PrimitiveType.h" "src/impl/graphics/interfaces/PrimitiveType.cpp")

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