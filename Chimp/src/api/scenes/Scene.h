#pragma once

#include "stdafx.h"

namespace Chimp {
	class SceneManager;

	class Scene {
		friend class SceneManager;
	public:
		Scene() = default;

	public:
		virtual bool ShouldExit() const = 0;

	protected:
		// Called when the scene is activated, this may happen more than once
		// OnDeactivate must be called between calls to OnActivate
		// previousScene is the scene that was previously active, store for shop or pause scenes where the previous scenes data must be preserved.
		virtual void OnActivate(std::unique_ptr<Scene> previousScene = nullptr) = 0;

		// Called when the scene is deactivated, this may happen more than once
		virtual void OnDeactivate() = 0;

		// Called when the scene should update
		virtual void OnUpdate() = 0;

		// Called when the scene should render
		virtual void OnRender() = 0;
	};
}