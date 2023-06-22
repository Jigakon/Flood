#ifndef EXAMPLE2D__H
#define EXAMPLE2D__H

#include <Flood.h>

class Example2D : public Flood::Layer
{
public:
	Example2D();
	virtual ~Example2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Flood::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Flood::Event& event) override;

private:
	Flood::OrthographicCameraController m_Camera;

	glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.f };
	Flood::Ref<Flood::Texture2D> m_Peter;
	Flood::Ref<Flood::Texture2D> m_Lambert;
	Flood::Ref<Flood::Texture2D> m_SpriteSheet;
	Flood::Ref<Flood::SubTexture2D> m_FullDirtBlock;
};

#endif // !EXAMPLE2D__H