#include "MainLoop.h"

namespace Chimp {
	MainLoop::MainLoop(std::unique_ptr<Scene> entryScene)
		: m_SceneManager(std::move(entryScene)) {
		while (!m_SceneManager.m_CurrentScene->ShouldExit()) {
			m_SceneManager.Update();
			m_SceneManager.Render();
			m_SceneManager.CheckForSceneChange();
		}
	}
}