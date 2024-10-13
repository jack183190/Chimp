#include "api/graphics/meshes/TexturedQuad.h"

namespace Chimp {
	std::unique_ptr<Mesh> TexturedQuad::Create(
		IRenderingManager& renderingManager,
		std::shared_ptr<IShader> shader
	) {
		// Vertex buffer
		const GraphicsType VERTEX_TYPE = GraphicsType::FLOAT;
		RawArray vertexData;
		vertexData.NumberElements = (3 + 2) * 4;
		vertexData.Size = VERTEX_TYPE.Size * vertexData.NumberElements;
		vertexData.Data = new float[vertexData.NumberElements] {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				-0.5f, 0.5f, 0.0f, 1.0f, 1.0f
			};

		std::shared_ptr<IBuffer> vertexBuffer = renderingManager.CreateBuffer(
			{
				Usage::UpdateFrequency::VERY_OCCASIONAL,
				Usage::Access::CPU_WRITE
			},
			BindTarget::VERTEX_BUFFER
		);
		vertexBuffer->SetData(VERTEX_TYPE, vertexData);

		// Index buffer
		const GraphicsType INDEX_TYPE = GraphicsType::UNSIGNED_INT;
		RawArray indexData;
		indexData.NumberElements = 6;
		indexData.Size = INDEX_TYPE.Size * indexData.NumberElements;
		indexData.Data = new uint32_t[indexData.NumberElements]{
			0, 1, 2,
			0, 2, 3
		};

		auto indexBuffer = renderingManager.CreateBuffer(
			{
				Usage::UpdateFrequency::VERY_OCCASIONAL,
				Usage::Access::CPU_WRITE
			},
			BindTarget::INDEX_BUFFER
		);
		indexBuffer->SetData(INDEX_TYPE, indexData);

		// Element array
		std::unique_ptr<IElementArrayLayout> elementLayout = renderingManager.CreateElementArrayLayout(
			PrimitiveType::TRIANGLES,
			{
				{ GraphicsType::FLOAT, 3, false },
				{ GraphicsType::FLOAT, 2, false }
			}
		);

		auto elementArray = renderingManager.CreateElementArray(
			vertexBuffer,
			std::move(indexBuffer),
			std::move(elementLayout)
		);

		return Mesh::Builder()
			.AddSection(std::move(elementArray), shader)
			.Build();
	}
}