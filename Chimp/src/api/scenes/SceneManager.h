#pragma once

#include "Scene.h"

namespace Chimp {
	class MainLoop;
	class SceneManager {
		friend class MainLoop;
	private:
		SceneManager(std::unique_ptr<Scene> entryScene);

	public:
		// Switch to the new scene after the current frame
		// newScene cannot be nullptr
		void QueueSceneChange(std::unique_ptr<Scene> newScene);

		// Switch to the new scene immediately
		// newScene cannot be nullptr
		void ChangeScene(std::unique_ptr<Scene> newScene);

	private:
		// Called when the scene should update
		void Update();

		// Called when the scene should render
		void Render();

		// If m_NextScene is not nullptr, switch to it
		void CheckForSceneChange();

	private:
		std::unique_ptr<Scene> m_CurrentScene;
		std::unique_ptr<Scene> m_NextScene;
	};
}