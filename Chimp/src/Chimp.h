#pragma once

#include "api/scenes/SceneManager.h"
#include "api/Engine.h"
#include "api/window/IWindow.h"
#include "api/graphics/IRenderingManager.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "api/graphics/camera/controllers/DebugCameraController.h"
#include "api/ecs/ECS.h"
#include "api/networking/PacketTypeRegistry.h"

// Utils
#include "api/utils/FileReader.h"
#include "api/utils/HashCombine.h"
#include "api/utils/Maths.h"
#include "api/utils/OptionalReference.h"
#include "api/utils/Path.h"
#include "api/utils/RawArray.h"
#include "api/utils/ThreadQueue.h"
#include "api/utils/UpdateSubscriber.h"
#include "api/utils/waves/WaveManager.h"
#include "api/threading/ThreadPool.h"