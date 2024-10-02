#include "MainLoop.h"
#include "api/Engine.h"

namespace Chimp {
	MainLoop::MainLoop(std::unique_ptr<Scene> entryScene,
		Engine& engine)
		: m_SceneManager(std::move(entryScene)),
		m_Engine(engine) {
		while (!m_SceneManager.m_CurrentScene->ShouldExit()) {
			m_Engine.GetTimeManager().Update();

			m_SceneManager.Update();
			m_SceneManager.Render();
			m_SceneManager.CheckForSceneChange();
		}
	}
}