#include "ExampleLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>


ExampleLayer::ExampleLayer() : Layer("Example"),
m_CameraController(1280.f / 720.f, true)
{
	m_TriangleVA = Flood::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f,  0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f,  0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	Flood::Ref<Flood::VertexBuffer> vertexBuffer;
	vertexBuffer = Flood::VertexBuffer::Create(vertices, sizeof(vertices));

	Flood::BufferLayout layout =
	{
		{ Flood::ShaderDataType::Float3, "a_Position", false },
		{ Flood::ShaderDataType::Float4, "a_Color", false }
	};

	vertexBuffer->SetLayout(layout);
	m_TriangleVA->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	Flood::Ref<Flood::IndexBuffer> indexBuffer;
	indexBuffer = Flood::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_TriangleVA->SetIndexBuffer(indexBuffer);

	m_SquareVA = Flood::VertexArray::Create();

	float squareVertices[5 * 4] =
	{
		-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.0f,  0.0f,  1.0f
	};

	Flood::Ref<Flood::VertexBuffer> squareVB;
	squareVB = Flood::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	Flood::BufferLayout squareVBLayout =
	{
		{ Flood::ShaderDataType::Float3, "a_Position", false },
		{ Flood::ShaderDataType::Float2, "a_TexCoord", false }
	};
	squareVB->SetLayout(squareVBLayout);
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2 , 2, 3, 0 };
	Flood::Ref<Flood::IndexBuffer> squareIB;
	squareIB = Flood::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	std::string path = "Assets/Shaders/";

	m_TriangleShader = Flood::Shader::Create("BaseShader", path + "vertex.vert", path + "fragment.frag");

	m_FlatColorShader = Flood::Shader::Create("FlatColor", path + "flatColorVertex.vert", path + "flatColorFragment.frag");

	auto textureShader = m_ShaderLib.Load("Assets/Shaders/texture.glsl");

	m_Texture = Flood::Texture2D::Create("Assets/Textures/peter.png");
	m_CreajeuxLogoTexture = Flood::Texture2D::Create("Assets/Textures/creajeux.png");

	textureShader->Bind();
	textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnUpdate(Flood::Timestep ts)
{
	//FLOOD_APP_TRACE("dt : {0} s ({1} ms)", ts.GetSeconds(), ts.GetMilliseconds());

	m_CameraController.OnUpdate(ts);

	Flood::RenderCommand::SetClearColor({ 0.f, 0.5f, 0.7f, 1.f });
	Flood::RenderCommand::Clear();

	Flood::Renderer::BeginScene(m_CameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

	for (int y = 0; y < 20; y++)
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

			Flood::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}

	auto textureShader = m_ShaderLib.Get("texture");
	m_Texture->Bind();
	Flood::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Triangle
	// Flood::Renderer::Submit(m_TriangleShader, m_TriangleVA);

	Flood::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void ExampleLayer::OnEvent(Flood::Event& event)
{
	m_CameraController.OnEvent(event);
}