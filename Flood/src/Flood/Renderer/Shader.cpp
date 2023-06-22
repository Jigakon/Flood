#include "flpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>

namespace Flood
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: FLOOD_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		FLOOD_ENGINE_ASSERT(false, "UnKnown RendererAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: FLOOD_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(path);
		}

		FLOOD_ENGINE_ASSERT(false, "UnKnown RendererAPI");
		return nullptr;
	}

	std::string Shader::Parse(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::string line;
		std::stringstream ss;
		while (getline(stream, line))
		{
			ss << line << '\n';
		}
		return ss.str();
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		FLOOD_ENGINE_ASSERT(!Exists(name), "Shader already registered");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string & name)
	{
		FLOOD_ENGINE_ASSERT(Exists(name), "Shader not found");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
