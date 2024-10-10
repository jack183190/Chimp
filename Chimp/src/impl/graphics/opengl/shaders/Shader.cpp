#pragma once

#include "Shader.h"

namespace Chimp::GL {
	Shader::Shader(const ShaderFilePaths& paths,
		const ShaderCompiler& compiler)
		: m_ProgramID(0),
		m_IsValid(false)
	{
		// Compile the shaders
		const auto pathsMap = paths.AsMap();
		std::vector<GLuint> shaderIds;
		shaderIds.reserve(pathsMap.size());

		for (const auto& [type, path] : pathsMap)
		{
			const auto result = compiler.Compile(type, FileReader::Read(path));
			if (!result.Success)
			{
				assert(false);
				DeleteShaders(shaderIds);
				return;
			}
			shaderIds.push_back(result.Id);
		}

		// Combine them into a program
		m_ProgramID = glCreateProgram();
		for (const auto shaderId : shaderIds)
		{
			glAttachShader(m_ProgramID, shaderId);
		}
		glLinkProgram(m_ProgramID);

		// Check for success
		{
			GLint success;
			glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
			m_IsValid = success == GL_TRUE;
		}

		if (!m_IsValid)
		{
			std::cerr << "Failed to link shader program " << m_ProgramID << std::endl;
			assert(false);
			DeleteShaders(shaderIds);
			m_ProgramID = 0;
			return;
		}

		// Delete the shaders, the program contains all required data
		DeleteShaders(shaderIds);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	bool Shader::IsValid() const
	{
		return m_IsValid;
	}

	void Shader::DeleteShaders(std::vector<GLuint>& shaderIds)
	{
		for (const auto shaderId : shaderIds)
		{
			glDeleteShader(shaderId);
		}
		shaderIds.clear();
	}
}