#pragma once

#include "api/scenes/SceneManager.h"

namespace Chimp {
	class MainLoop {
	public:
		MainLoop(std::unique_ptr<Scene> entryScene);

	private:
		SceneManager m_SceneManager;
	};
}