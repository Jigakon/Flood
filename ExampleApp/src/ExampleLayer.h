#ifndef EXAMPLELAYER__H
#define EXAMPLELAYER__H

#include <Flood.h>

class ExampleLayer : public Flood::Layer
{
public:
	ExampleLayer();

	void OnUpdate(Flood::Timestep ts) override;

	virtual void OnImGuiRender() override;

	void OnEvent(Flood::Event& event) override;

private:
	Flood::ShaderLibrary m_ShaderLib;
	Flood::Ref<Flood::Shader> m_TriangleShader;
	Flood::Ref<Flood::VertexArray> m_TriangleVA;

	Flood::Ref<Flood::Shader> m_FlatColorShader;
	Flood::Ref<Flood::VertexArray> m_SquareVA;

	Flood::Ref<Flood::Texture2D> m_Texture, m_CreajeuxLogoTexture;

	Flood::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.f };
};

#endif // !EXAMPLELAYER__H
