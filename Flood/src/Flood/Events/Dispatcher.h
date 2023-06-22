#ifndef DISPATCHER__H
#define DISPATCHER__H

#include <vector>
#include <iostream>
#include "Flood/Events/Pointers/EventPointer.h"
#include "Flood/Events/Pointers/FunctionPointer.h"
#include "Flood/Events/Pointers/MethodPointer.h"

namespace Events
{
	template<typename Return, typename... Args>
	class Dispatcher
	{
		template <class C, typename... Args>
		using Method = Return(C::*)(Args...);

		using Function = Return(*)(Args...);
		using FunctionType = ActionPointer<Return, Args...>;
	private:
		std::vector<FunctionType*> m_RegisteredFct;

	public:

		Dispatcher() = default;

#pragma region Register, add method or function pointer
		///<summary> Register Method
		///<para> register a function in the vector of functions (can be registered multiple times) </para>
		///</summary>
		void Register(Function fct)
		{
			FunctionType* fctPtr = new FunctionPointer<Return, Args...>(fct);
			m_RegisteredFct.push_back(fctPtr);
		}
		///<summary> Register Method
		///<para> register a method with its object in the vector of functions (can be registered multiple times) </para>
		///</summary>
		template<class C>
		void Register(C& object, Method<C, Args...> fct)
		{
			FunctionType* fctPtr = new MethodPointer<C, Return, Args...>(object, fct);
			m_RegisteredFct.push_back(fctPtr);
		}
#pragma endregion

#pragma region Unregister, delete method or function pointer
		///<summary> Unregister Method
		///<para> remove a specific function from the vector of functions (remove the first encountered) </para>
		///</summary>
		void Unregister(Function fct)
		{
			for (int i = 0; i < m_RegisteredFct.size(); i++)
				if (fct == static_cast<FunctionPointer<Return, Args...>*>(m_RegisteredFct[i])->GetAdress())
					m_RegisteredFct.erase(m_RegisteredFct.begin() + i);
		}
		///<summary> Unregister Method
		///<para> remove a specific method registered with the associated object from the vector of functions </para>
		///</summary>
		template<class C>
		void Unregister(C& object, Method<C, Args...> fct)
		{
			for (int i = 0; i < m_RegisteredFct.size(); i++)
			{
				auto ptr = static_cast<MethodPointer<C, Return, Args...>*>(m_RegisteredFct[i]);
				if (fct == ptr->GetAdress() && &object == ptr->GetObject())
					m_RegisteredFct.erase(m_RegisteredFct.begin() + i);
			}
		}
		///<summary> Clear functions Method
		///<para> clear the vector of registered functions (remove the first encountered) </para>
		///</summary>
		void clear() { m_RegisteredFct.clear(); } // 
#pragma endregion
		///<summary> Size Method
		///<para> number of function registered </para>
		///</summary>
		inline size_t size() { return m_RegisteredFct.size(); }
#pragma region Call functions
		///<summary> Broadcast Method
		///<para> return an array of return type or nullptr if return type is void </para>
		///</summary>
		Return* BroadcastWithReturn(Args... args)
		{
			if constexpr (std::is_void<Return>())
			{
				for (auto fct : m_RegisteredFct)
				{
					(*fct)(args...);
				}
				return nullptr;
			}
			else
			{
				Return* values = new Return[size()];
				int i = 0;
				for (auto fct : m_RegisteredFct)
				{
					values[i] = (*fct)(args...);
					i++;
				}
				return values;
			}
		}

		void Broadcast(Args... args)
		{
			for (auto fct : m_RegisteredFct)
				(*fct)(args...);
		}


		///<summary> Dispose Method
		///<para> delete the array of return values created by <see cref="Dispatcher::Broadcast"/> (used only when the return type is not void) </para>
		///</summary>
		void Dispose(Return* t)
		{
			if (t != nullptr)
				delete[] t;
		}
		///<summary> Invoke Method
		///<para> return an array of return type or nullptr if return type is void </para>
		///</summary>
		Return* InvokeWithReturn(Args... args)
		{
			return BroadcastWithReturn(args...);
		}

		void Invoke(Args... args)
		{
			for (auto fct : m_RegisteredFct)
				(*fct)(args...);
		}

		///<summary> Call functions
		///<para> return an array of return type or nullptr if return type is void
		///</summary>
		void operator()(Args... args)
		{
			for (auto fct : m_RegisteredFct)
				(*fct)(args...);
		}
		///<summary> Get All functions
		///<para> return a vector of function type registered </para>
		///</summary>
		inline std::vector<FunctionType*> GetFunctions() const { return m_RegisteredFct; }
#pragma endregion
	};
}


#endif // !DISPATCHER__H
