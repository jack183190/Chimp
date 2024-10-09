#include "ShaderCompiler.h"
#include "../Translations.h"

namespace Chimp::GL {
	Chimp::IShaderCompiler<GLuint>::Result ShaderCompiler::Compile(const ShaderType type, const std::string& source)
	{
		Result result = { false, 0 };
		const GLenum glShaderType = ShaderTypeTranslation(type);
		const char* src = source.c_str();

		result.Id = glCreateShader(glShaderType);
		glShaderSource(result.Id, 1, &src, nullptr);

		glCompileShader(result.Id);

		// Check if compiled successfully
		glGetShaderiv(result.Id, GL_COMPILE_STATUS, reinterpret_cast<GLint*>(&result.Success));
		if (!result.Success)
		{
			GLint length;
			glGetShaderiv(result.Id, GL_INFO_LOG_LENGTH, &length);
			std::vector<GLchar> log(length);
			glGetShaderInfoLog(result.Id, length, &length, log.data());
			std::cerr << "Shader compilation failed: " << log.data() << std::endl;
		}
		return result;
	}
}