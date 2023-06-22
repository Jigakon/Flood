#ifndef EVENTSPOINTER__H
#define EVENTSPOINTER__H

namespace Events
{
	template<typename Return, typename... Args>
	class ActionPointer
	{
	public:
		ActionPointer() = default;
		virtual ~ActionPointer() = default;
		virtual Return operator() (Args...) const = 0;
	};
}

#endif // !EVENTSPOINTER__H
