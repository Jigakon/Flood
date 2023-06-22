#ifndef METHODPOINTER__H
#define METHODPOINTER__H

#include "EventPointer.h"

namespace Events
{
	template<class C, typename Return, typename... Args>
	class MethodPointer : public ActionPointer<Return, Args...>
	{
		template<class C, typename Return, typename... Args>
		using Method = Return(C::*)(Args...);

	private:
		Method<C, Return, Args...> m_mthd;
		C* m_Object;

	public:
		MethodPointer(C& object, Method<C, Return, Args...> mthd) : m_mthd(mthd), m_Object(&object) {};

		Return operator()(Args... args) const override
		{
			return (m_Object->*m_mthd)(args...);
		}

		inline const Method<C, Return, Args...>& GetAdress() const { return m_mthd; }
		inline const C* GetObject() const { return m_Object; }
	};
}

#endif // !METHODPOINTER__H
