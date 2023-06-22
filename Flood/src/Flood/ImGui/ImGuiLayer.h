#ifndef IMGUILAYER__H
#define IMGUILAYER__H

#include "Flood/Core/Layer.h"
#include "Flood/Events/MouseEvent.h"
#include "Flood/Events/KeyEvent.h"
#include "Flood/Events/ApplicationEvent.h"

namespace Flood
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}

#endif // !IMGUILAYER__H
