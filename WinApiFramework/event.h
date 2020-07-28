#ifndef EVENT_H_
#define EVENT_H_

#include <unordered_map>
#include <vector>

namespace WinApiFramework
{
	// ~~~~~~~~ [STRUCT] BaseEvent ~~~~~~~~
	struct BaseEvent
	{
	public:
		BaseEvent() = default;
		virtual ~BaseEvent() = default;
	public:
		virtual void InvokeHandlers() = 0;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [STRUCT] BaseAction ~~~~~~~~
	struct BaseAction
	{
	public:
		BaseAction() = default;
		virtual ~BaseAction() = default;
	public:
		virtual void Invoke() = 0;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




	// ~~~~~~~~ event functions ~~~~~~~~
	template <typename E, class C> using event_member_function_t = void(C::*)(E&);
	template <typename E> using event_function_t = void(*)(E&);


	// ~~~~~~~~ event functor ~~~~~~~~
	template <typename E> struct BaseEventFunctor
	{
		BaseEventFunctor() {}
		virtual ~BaseEventFunctor() {}

		virtual void Call(E& event) = 0;
		virtual const event_function_t<E> GetFunction() const = 0;
	};
	template <typename E, class C> struct EventFunctor : public BaseEventFunctor<E>
	{
	private:
		C* m_pObject;
		event_member_function_t<E, C> m_function;

	public:
		EventFunctor(C* object, event_member_function_t<E, C> function)
			: m_pObject(object)
			, m_function(function)
		{}
		~EventFunctor() {}

	public:
		void Call(E& event) override
		{
			(m_pObject->*m_function)(event);
		}
		const event_function_t<E> GetFunction() const override
		{
			const event_member_function_t<E, C>* fn_ptr = &m_function;
			return *(reinterpret_cast<const event_function_t<E>*>(fn_ptr));
		}
	};
	template <typename E> struct EventFunctor<E, void> : public BaseEventFunctor<E>
	{
	private:
		event_function_t<E> m_function;

	public:
		EventFunctor(event_function_t<E> function)
			: m_function(function)
		{}
		~EventFunctor() {}

	public:
		void Call(E& event) override
		{
			m_function(event);
		}
		const event_function_t<E> GetFunction() const override
		{
			return m_function;
		}
	};


	// ~~~~~~~~ event functor list ~~~~~~~~
	struct BaseFunctorList
	{
		BaseFunctorList() = default;
		virtual ~BaseFunctorList() {}
	};
	template <typename E> struct FunctorList : public BaseFunctorList
	{
	private:
		std::vector<BaseEventFunctor<E>*> m_ehl;

	public:
		FunctorList() {}
		~FunctorList() 
		{
			for (auto& eh : m_ehl) 
				delete eh;
		}

	public:
		void AddEventHandler(BaseEventFunctor<E>* eh)
		{
			m_ehl.push_back(eh);
		}
		bool RemoveEventHandler(const BaseEventFunctor<E>* eh)
		{
			for (size_t i = 0; i < m_ehl.size(); i++)
			{
				if ((m_ehl[i]->GetFunction()) == (eh->GetFunction()))
				{
					delete m_ehl[i];
					m_ehl.erase(m_ehl.begin() + i);
					return true;
				}
			}
			return false;
		}
		void CallHandlers(E& e)
		{
			for (auto& h : m_ehl)
			{
				h->Call(e);
			}
		}
	};


	// ~~~~~~~~ [STRUCT] EventFunctorManager ~~~~~~~~
	struct EventFunctorManager
	{
	private:
		using TypeInfoRef = std::reference_wrapper<const std::type_info>;
		struct Hasher
		{
			std::size_t operator()(TypeInfoRef code) const
			{
				return code.get().hash_code();
			}
		};
		struct EqualTo
		{
			bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const
			{
				return lhs.get() == rhs.get();
			}
		};
		std::unordered_map<TypeInfoRef, BaseFunctorList*, Hasher, EqualTo> m_functor_lists;

	public:
		EventFunctorManager()
		{

		}
		~EventFunctorManager()
		{
			for (auto& h : m_functor_lists)
			{
				delete h.second;
			}
			m_functor_lists.clear();
		}

	public:
		// Find list of event functors for specified event type
		template <typename E> FunctorList<E>* GetHandlerList()
		{
			auto search = m_functor_lists.find(typeid(E));
			if (search != m_functor_lists.end())
			{
				return dynamic_cast<FunctorList<E>*>(search->second);
			}
			else
			{
				return nullptr;
			}
		}

		template <typename E, typename C> void AddEventHandler(C* object, void(C::*function)(E&))
		{
			AddEventHandlerToList(new EventFunctor<E, C>(object, function));
		}
		template <typename E> void AddEventHandler(void(*function)(E&))
		{			
			AddEventHandlerToList(new EventFunctor<E, void>(function));
		}

		template <typename E, typename C> bool RemoveEventHandler(void(C::*function)(E&))
		{
			BaseEventFunctor<E>* eh = new EventFunctor<E, C>(nullptr, function);
			bool result = RemoveEventHandlerFromList<E>(eh);
			delete eh;
			return result;
		}
		template <typename E> bool RemoveEventHandler(void(*function)(E&))
		{
			BaseEventFunctor<E>* eh = new EventFunctor<E, void>(function);
			bool result = RemoveEventHandlerFromList<E>(eh);
			delete eh;
			return result;
		}
	private:
		template <typename E> void AddEventHandlerToList(BaseEventFunctor<E>* eh)
		{
			FunctorList<E>* hlist = GetHandlerList<E>();
			if (hlist == nullptr)
			{// -> HandlerManager does not contain handler list for specified event
				FunctorList<E>* hl = new FunctorList<E>();
				hl->AddEventHandler(eh);
				m_functor_lists.insert({ typeid(E), reinterpret_cast<BaseFunctorList*>(hl) });
			}
			else
			{// -> HandlerManager has at least one event handler for specified event
				hlist->AddEventHandler(eh);
			}
		}
		template <typename E> bool RemoveEventHandlerFromList(const BaseEventFunctor<E>* eh)
		{
			FunctorList<E>* hlist = GetHandlerList<E>();
			if (hlist == nullptr)
			{// -> there is no handler list for specified event
				return false;
			}
			else
			{// -> try to find and remove event handler
				return hlist->RemoveEventHandler(eh);
			}
		}
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif