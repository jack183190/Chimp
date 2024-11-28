#include "api/scenes/SceneManager.h"

namespace Chimp {
	SceneManager::SceneManager(std::unique_ptr<Scene> entryScene) 
		: m_CurrentScene(std::move(entryScene)) {
		
	}

	void SceneManager::QueueSceneChange(std::unique_ptr<Scene> newScene)
	{
			m_NextScene = std::move(newScene);
	}

	void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene)
	{
		QueueSceneChange(std::move(newScene));	
		CheckForSceneChange();
	}

	void SceneManager::Update()
	{
		m_CurrentScene->OnUpdate();
	}

	void SceneManager::Render()
	{
		m_CurrentScene->OnRender();
	}

	void SceneManager::RenderUI()
	{
		m_CurrentScene->OnRenderUI();
	}

	bool SceneManager::CheckForSceneChange()
	{
		if (m_NextScene != nullptr) [[unlikely]] {
			m_CurrentScene->OnDeactivate();
			std::unique_ptr<Scene> previousScene = std::move(m_CurrentScene);
			m_CurrentScene = std::move(m_NextScene);
			m_CurrentScene->OnActivate(std::move(previousScene));
			return true;
		}
		return false;
	}


}