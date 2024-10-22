#pragma once

// Maths
#include "api/utils/Maths.h"

// STD Data Structures

#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <deque>
#include <stack>
#include <string>
#include <queue>
#include <tuple>
#include <array>

// STD Multithreading

#include <mutex>
#include <thread>

// STD Useful things

#include <algorithm>
#include <any>
#include <iostream>
#include <memory>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <functional>
#include <typeinfo>
#include <climits>
#include <typeindex>
#include <iterator>
#include <filesystem>
#include <random>
#include <optional>
#include <utility>

// OpenGL
#ifdef CHIMP_OPENGL
#include <GL/glew.h>
#include <GL/gl.h>
#endif

// GLFW
#ifdef CHIMP_GLFW
#include <GLFW/glfw3.h>
#endif

// STB
#ifdef CHIMP_STB
#include <../vendor/stb/stb_image.h>
#endif

// Preprocessor
#include "preprocessor/CopyAndMove.h"