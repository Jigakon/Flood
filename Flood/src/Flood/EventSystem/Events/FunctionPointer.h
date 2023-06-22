#ifndef FUNCTIONPOINTER__H
#define FUNCTIONPOINTER__H

#include "EventPointer.h"

namespace Events
{
	template<typename Return, typename... Args>
	class FunctionPointer : public ActionPointer<Return, Args...>
	{
		using Function = Return(*)(Args...);

	private:
		Function m_fct;
	public:
		FunctionPointer(Function fct) : m_fct(fct) {};

		Return operator()(Args... args) const override
		{
			return m_fct(args...);
		}

		inline const Function& GetAdress() const { return m_fct; }
	};
}

#endif // !FUNCTIONPOINTER__H