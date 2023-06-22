#ifndef LAYER__H
#define LAYER__H

#include "Flood/Core/Core.h"
#include "Flood/Events/Event.h"
#include "Flood/Core/Timestep.h"

namespace Flood
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName()const { return m_DebugName; } ;

	protected:
		std::string m_DebugName;
	};
}

#endif //!LAYER__H
