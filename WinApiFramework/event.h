#ifndef EVENT_H_
#define EVENT_H_

#include <unordered_map>
#include <vector>

namespace WinApiFramework
{
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



	// ~~~~~~~~ [STRUCT] BaseEvent ~~~~~~~~
	struct BaseEvent
	{
	public:
		BaseEvent() = default;
		virtual ~BaseEvent() = default;
	public:
		virtual void BeforeHandling() {}
		virtual void AfterHandling() {}
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ event functions ~~~~~~~~
	template <typename E, class C> using event_member_function_t = void(C::*)(E&);
	template <typename E> using event_function_t = void(*)(E&);



	// ~~~~~~~~ event functor ~~~~~~~~
	template <typename E> struct BaseEventFunctor
	{
		BaseEventFunctor() {}
		virtual ~BaseEventFunctor() {}

		virtual void Call(E& event) const = 0;
		virtual const event_function_t<E> GetFunction() const = 0;
	};
	template <typename E, class C> struct EventFunctor : public BaseEventFunctor<E>
	{
	private:
		event_member_function_t<E, C> m_function;
		C* m_pObject;

	public:
		EventFunctor(event_member_function_t<E, C> function, C* object)
			: m_function(function)
			, m_pObject(object)
		{}
		~EventFunctor() {}

	public:
		void Call(E& event) const override
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
		void Call(E& event) const override
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
		std::vector<const BaseEventFunctor<E>*> m_ehl;

	public:
		FunctorList() {}
		~FunctorList() 
		{
			for (auto& eh : m_ehl) 
				delete eh;
		}

	public:
		void AddEventHandler(const BaseEventFunctor<E>* eh)
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
		void CallHandlers(E& e) const
		{
			for (auto& h : m_ehl)
			{
				h->Call(e);
			}
		}
	};



	// ~~~~~~~~ [CLASS] EventHandler ~~~~~~~~
	class EventHandler
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

	protected:
		EventHandler() {}
		~EventHandler()
		{
			for (auto& h : m_functor_lists)
			{
				delete h.second;
			}
			m_functor_lists.clear();
		}

	protected:
		template <typename E> void InvokeEvent(E& event)
		{
			event.BeforeHandling();

			const FunctorList<E>* flist = GetFunctorList<E>();
			if (flist) flist->CallHandlers(event);

			event.AfterHandling();
		}
	protected:
		template <typename E, typename C> void BindEventFunc(void(C::*function)(E&), C* object)
		{
			AddEventFunctorToList(new EventFunctor<E, C>(function, object));
		}
		template <typename E> void BindEventFunc(void(*function)(E&))
		{			
			AddEventFunctorToList(new EventFunctor<E, void>(function));
		}

		template <typename E, typename C> bool UnbindEventFunc(void(C::*function)(E&))
		{
			BaseEventFunctor<E>* eh = new EventFunctor<E, C>(function, nullptr);
			bool result = RemoveEventFunctorFromList<E>(eh);
			delete eh;
			return result;
		}
		template <typename E> bool UnbindEventFunc(void(*function)(E&))
		{
			BaseEventFunctor<E>* eh = new EventFunctor<E, void>(function);
			bool result = RemoveEventFunctorFromList<E>(eh);
			delete eh;
			return result;
		}
	private:
		// Find list of event functors for specified event type
		template <typename E> FunctorList<E>* GetFunctorList()
		{
			auto search = m_functor_lists.find(typeid(E));
			if (search == m_functor_lists.end()) 
				return nullptr;

			return dynamic_cast<FunctorList<E>*>(search->second);
		}
		template <typename E> void AddEventFunctorToList(BaseEventFunctor<E>* eh)
		{
			FunctorList<E>* hlist = GetFunctorList<E>();
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
		template <typename E> bool RemoveEventFunctorFromList(const BaseEventFunctor<E>* eh)
		{
			FunctorList<E>* hlist = GetFunctorList<E>();
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