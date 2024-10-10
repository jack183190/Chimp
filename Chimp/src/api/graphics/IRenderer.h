#pragma once

#include "buffers/IElementArray.h"
#include "shaders/IShader.h"
#include "Mesh.h"
#include "camera/Camera.h"

namespace Chimp
{
	class IRenderer
	{
	protected:
		IRenderer() = default;
	public:
		virtual ~IRenderer() = default;

		// Get the default camera
		[[nodiscard]] const Camera& GetDefaultCamera();

		// Set the camera
		void SetCamera(std::shared_ptr<ICamera> camera);

		// Draw a mesh
		void Draw(const Mesh& mesh) const;

		// Draw a mesh section
		virtual void Draw(const Mesh::Section& meshSection) const = 0;

	protected:
		// Get the camera matrices
		[[nodiscard]] const CameraMatrices& GetCameraMatrices() const;

	private:
		std::shared_ptr<Camera> m_DefaultCamera = std::make_shared<Camera>();
		std::shared_ptr<ICamera> m_Camera = m_DefaultCamera;
	};
}