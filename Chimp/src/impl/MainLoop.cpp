#include "MainLoop.h"
#include "api/Engine.h"

namespace Chimp {
	MainLoop::MainLoop(std::unique_ptr<Scene> entryScene,
		Engine& engine)
		: m_SceneManager(std::move(entryScene)),
		m_Engine(engine) {
		while (!m_SceneManager.m_CurrentScene->ShouldExit()) {
			m_Engine.GetTimeManager().Update();
			m_Engine.GetWindow().Update();

			m_SceneManager.Update();
			m_Engine.GetWindow().UpdateEnd();

			m_Engine.GetRenderingManager().GetRenderer().StartDrawing();
			m_SceneManager.Render();

			m_Engine.GetWindow().SwapBuffers();

			m_SceneManager.CheckForSceneChange();
		}
	}
}