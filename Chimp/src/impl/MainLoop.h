#pragma once

#include "api/scenes/SceneManager.h"

namespace Chimp {
	class Engine;
	class MainLoop {
	public:
		MainLoop(std::unique_ptr<Scene> entryScene, Engine& engine);

	private:
		SceneManager m_SceneManager;
		Engine& m_Engine;
	};
}