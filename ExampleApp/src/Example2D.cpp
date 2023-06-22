#include "Example2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

#include "Flood/Debug/Instrumentor.h"

Example2D::Example2D() : Layer("Example2D"), m_Camera(1280.f / 720.f) {}

void Example2D::OnAttach()
{
	FLOOD_PROFILE_FUNCTION();

	m_Peter = Flood::Texture2D::Create("Assets/Textures/peter.png");
	m_Lambert = Flood::Texture2D::Create("Assets/Textures/lamboubou.png");
	m_SpriteSheet = Flood::Texture2D::Create("Assets/Textures/dirtTexture.png");

	m_FullDirtBlock = Flood::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 6 }, { 16.f, 16.f }, { 1.f, 2.f });
}

void Example2D::OnDetach()
{
	FLOOD_PROFILE_FUNCTION();

	Flood::Renderer2D::Shutdown();
}

void Example2D::OnUpdate(Flood::Timestep ts)
{
	FLOOD_PROFILE_FUNCTION();

	m_Camera.OnUpdate(ts);

	Flood::Renderer2D::ResetStats();
	{
		FLOOD_PROFILE_SCOPE("Example2D::Render prep");
		Flood::RenderCommand::SetClearColor({ 0.f, 0.5f, 0.7f, .5f });
		Flood::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.f;

		FLOOD_PROFILE_SCOPE("Example2D::Render Draw");
		Flood::Renderer2D::BeginScene(m_Camera.GetCamera());
		Flood::Renderer2D::DrawQuad({ 0.f, 0.f, -0.1f }, { 10.f, 10.f }, m_Peter, { 1.f,1.f,1.f,1.f }, 1.f);
		Flood::Renderer2D::DrawQuad({ -.5f, 0.f }, { .8f, .8f }, m_Color);
		Flood::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Flood::Renderer2D::DrawQuad({ 3.f, 0.f, 0.1f }, { 1.f, 1.f }, rotation, m_Lambert);
		Flood::Renderer2D::DrawQuad({ 1.f, 0.f, 0.1f }, { 1.f, 1.f }, -rotation, m_Lambert);
		Flood::Renderer2D::DrawQuad({ 2.f, 1.f, 0.1f }, { 1.f, 1.f }, -rotation, m_Lambert);
		Flood::Renderer2D::DrawQuad({ 2.f, -1.f, 0.1f }, { 1.f, 1.f }, rotation, m_Lambert);
		Flood::Renderer2D::DrawQuad({ -6.f, 0.f, 0.1f }, { 5.f, 5.f }, m_SpriteSheet);
		Flood::Renderer2D::DrawQuad({ -6.f, -5.f, 0.1f }, { 1.f, 2.f }, m_FullDirtBlock);
		Flood::Renderer2D::EndScene();
		/*
		Flood::Renderer2D::BeginScene(m_Camera.GetCamera());
		for (float y = -5.f; y < 5.f; y += 0.1f)
			for (float x = -5.f; x < 5.f; x += 0.1f)
			{
				Flood::Renderer2D::DrawQuad({ x, y , 0.1f}, { .095f, .095f }, {(x+5.f)/10.f, (y+5.f)/10.f, 0.f, .5f});
			}

		Flood::Renderer2D::EndScene();
		*/
	}
}

void Example2D::OnImGuiRender()
{
	FLOOD_PROFILE_FUNCTION();

	ImGui::Begin("Game");

	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_Color));

	auto renderer2DStats = Flood::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats");
	ImGui::Text("DrawCalls : %d", renderer2DStats.DrawCalls);
	ImGui::Text("Quads : %d", renderer2DStats.QuadCount);
	ImGui::Text("Vertices : %d", renderer2DStats.GetTotalVertexCount());
	ImGui::Text("Indices : %d", renderer2DStats.GetTotalIndexCount());

	ImGui::End(); // Settings End
}

void Example2D::OnEvent(Flood::Event& event)
{
	m_Camera.OnEvent(event);
}
