#pragma once

#include "api/graphics/shaders/IShader.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "impl/graphics/opengl/shaders/ShaderCompiler.h"

namespace Chimp::GL {
	class Shader : public IShader {
		DISABLE_COPY(Shader);
	public:
		Shader(const ShaderFilePaths& paths, const ShaderCompiler& compiler);
		~Shader() override;

		void Bind() const override;

		bool IsValid() const override;

	private:
		// Delete shaders by id and clear the vector
		void DeleteShaders(std::vector<GLuint>& shaderIds);

	private:
		GLuint m_ProgramID;
		bool m_IsValid;
	};
}