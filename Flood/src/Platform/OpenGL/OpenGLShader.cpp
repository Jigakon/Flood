#include "flpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <fstream>
#include <vector>

namespace Flood
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		FLOOD_ENGINE_ASSERT(false, "unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		FLOOD_PROFILE_FUNCTION();

		m_Name = name;

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = Shader::Parse(vertexPath);
		sources[GL_FRAGMENT_SHADER] = Shader::Parse(fragmentPath);
		Compile(sources);

	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		FLOOD_PROFILE_FUNCTION();

		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind(".");
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		FLOOD_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		FLOOD_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
			}
			else
			{
				FLOOD_ENGINE_ERROR("Could not read from file '{0}'", path);
			}
			in.close();
		}
		else
			FLOOD_ENGINE_ERROR("Could not open file '{0}'", path);

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		FLOOD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLenght = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			FLOOD_ENGINE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLenght + 1;
			std::string type = source.substr(begin, eol - begin);
			FLOOD_ENGINE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specifier");
		
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			FLOOD_ENGINE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		FLOOD_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		
		FLOOD_ENGINE_ASSERT(shaderSources.size() <= 2, "Flood only support 2 shaders in a program (vert, frag)");
		std::array<GLenum, 2> glShaderIds;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);

				FLOOD_ENGINE_ERROR("shader compilation failure!\n{0}", infoLog.data());
				FLOOD_ASSERT(false, "shader compilation failure!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIds[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIds)
				glDeleteShader(id);

			FLOOD_ENGINE_ERROR("{0}", infoLog.data());
			FLOOD_ENGINE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIds)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		FLOOD_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		FLOOD_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		FLOOD_PROFILE_FUNCTION();

		UploadUniformMat3(name, mat);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		FLOOD_PROFILE_FUNCTION();

		UploadUniformMat4(name, mat);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		FLOOD_PROFILE_FUNCTION();

		UploadUniformFloat4(name, values);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		FLOOD_PROFILE_FUNCTION();

		UploadUniformFloat3(name, values);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		FLOOD_PROFILE_FUNCTION();

		UploadUniformFloat2(name, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		FLOOD_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		FLOOD_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}
}
