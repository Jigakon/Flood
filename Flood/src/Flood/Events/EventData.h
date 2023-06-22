#ifndef EVENTDATA__H
#define EVENTDATA__H

#include "Flood/Events/Types/Event.h"
#include "Flood/Events/Types/ApplicationEvent.h"
#include "Flood/Events/Types/KeyEvent.h"
#include "Flood/Events/Types/MouseEvent.h"

#include "Flood/Events/Dispatcher.h"

namespace Flood
{
	class EventsStack
	{
	public:
		static EventsStack& Get()
		{
			if (s_Instance == nullptr)
				s_Instance = new EventsStack();
			return *s_Instance;
		}

		void Register(Event* e)
		{
			m_Events.push_back(e);
		}

		void DisposeAndClear()
		{
			for (auto& event : m_Events)
				delete event;
			m_Events.clear();
		}

		std::vector<Event*> m_Events;

	private:
		static EventsStack* s_Instance;
	};

	class EventsDispatchers
	{
	public:
		// Mouse
		Events::Dispatcher<bool, MouseMovedEvent*> MouseMoved;
		Events::Dispatcher<bool, MouseButtonPressedEvent*> MousePressed;
		Events::Dispatcher<bool, MouseButtonReleasedEvent*> MouseReleased;
		Events::Dispatcher<bool, MouseScrolledEvent*> MouseScrolled;

		// Keyboard
		Events::Dispatcher<bool, KeyPressedEvent*> KeyPressed;
		Events::Dispatcher<bool, KeyReleasedEvent*> KeyReleased;
		Events::Dispatcher<bool, KeyTypedEvent*> KeyTyped;

		// Window
		Events::Dispatcher<bool, WindowCloseEvent*> WindowClose;
		Events::Dispatcher<bool, WindowResizeEvent*> WindowResize;

		void PollEvent() 
		{
			auto& events = EventsStack::Get().m_Events;
			for (auto& e : events)
			{
				switch (e->GetEventType())
				{
					case EventType::WindowClose :
						for (auto& fct : WindowClose.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((WindowCloseEvent*)e);
						}
					break;
					case EventType::WindowResize :
						for (auto& fct : WindowResize.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((WindowResizeEvent*)e);
						}
						break;

					case EventType::KeyPressed:
						for (auto& fct : KeyPressed.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((KeyPressedEvent*)e);
						}
						break;
					case EventType::KeyReleased:
						for (auto& fct : KeyReleased.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((KeyReleasedEvent*)e);
						}
						break;
					case EventType::KeyTyped:
						for (auto& fct : KeyTyped.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((KeyTypedEvent*)e);
						}
						break;

					case EventType::MouseMoved:
						for (auto& fct : MouseMoved.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((MouseMovedEvent*)e);
						}
						break;
					case EventType::MouseButtonPressed:
						for (auto& fct : MousePressed.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((MouseButtonPressedEvent*)e);
						}
						break;
					case EventType::MouseButtonReleased:
						for (auto& fct : MouseReleased.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((MouseButtonReleasedEvent*)e);
						}
						break;
					case EventType::MouseScrolled:
						for (auto& fct : MouseScrolled.GetFunctions())
						{
							if (e->Handled)
								break;
							(*fct)((MouseScrolledEvent*)e);
						}
						break;
				}
			}
			
		}
	};
}

#endif // !EVENTDATA__H
