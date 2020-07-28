#ifndef EVENT_H_
#define EVENT_H_

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



	// ~~~~~~~~ event handler functions ~~~~~~~~
	template <typename E, class C> using event_member_function_t = void(C::*)(E&);
	template <typename E> using event_function_t = void(*)(E&);


	// ~~~~~~~~ event handlers ~~~~~~~~
	template <typename E> struct BaseEventHandler
	{
		BaseEventHandler() {}
		virtual ~BaseEventHandler() {}

		virtual void Call(E& event) = 0;
		virtual const event_function_t<E> GetFunction() const = 0;
	};
	template <typename E, class C> struct EventHandler : public BaseEventHandler<E>
	{
	private:
		C* m_pObject;
		event_member_function_t<E, C> m_function;

	public:
		EventHandler(C* object, event_member_function_t<E, C> function)
			: m_pObject(object)
			, m_function(function)
		{}
		~EventHandler() {}

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
	template <typename E> struct EventHandler<E, void> : public BaseEventHandler<E>
	{
	private:
		event_function_t<E> m_function;

	public:
		EventHandler(event_function_t<E> function)
			: m_function(function)
		{}
		~EventHandler() {}

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


	// ~~~~~~~~ event handler list ~~~~~~~~
	struct BaseHandlerList
	{
		BaseHandlerList() = default;
		virtual ~BaseHandlerList() {}
	};
	template <typename E> struct HandlerList : public BaseHandlerList
	{
	private:
		std::vector<BaseEventHandler<E>*> m_ehl;

	public:
		HandlerList() {}
		~HandlerList() 
		{
			for (auto& eh : m_ehl) 
				delete eh;
		}

	public:
		void AddEventHandler(BaseEventHandler<E>* eh)
		{
			m_ehl.push_back(eh);
		}
		bool RemoveEventHandler(const BaseEventHandler<E>* eh)
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


	// ~~~~~~~~ [STRUCT] EventHandlerManager ~~~~~~~~
	struct EventHandlerManager
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
		std::unordered_map<TypeInfoRef, BaseHandlerList*, Hasher, EqualTo> m_handlers_lists;

	public:
		EventHandlerManager()
		{

		}
		~EventHandlerManager()
		{
			for (auto& h : m_handlers_lists)
			{
				delete h.second;
			}
			m_handlers_lists.clear();
		}

	public:
		// Find list of event handlers for specified event type
		template <typename E> HandlerList<E>* GetHandlerList()
		{
			auto search = m_handlers_lists.find(typeid(E));
			if (search != m_handlers_lists.end())
			{
				return dynamic_cast<HandlerList<E>*>(search->second);
			}
			else
			{
				return nullptr;
			}
		}

		template <typename E, typename C> void AddEventHandler(C* object, void(C::*function)(E&))
		{
			AddEventHandlerToList(new EventHandler<E, C>(object, function));
		}
		template <typename E> void AddEventHandler(void(*function)(E&))
		{			
			AddEventHandlerToList(new EventHandler<E, void>(function));
		}

		template <typename E, typename C> bool RemoveEventHandler(void(C::*function)(E&))
		{
			BaseEventHandler<E>* eh = new EventHandler<E, C>(nullptr, function);
			bool result = RemoveEventHandlerFromList<E>(eh);
			delete eh;
			return result;
		}
		template <typename E> bool RemoveEventHandler(void(*function)(E&))
		{
			BaseEventHandler<E>* eh = new EventHandler<E, void>(function);
			bool result = RemoveEventHandlerFromList<E>(eh);
			delete eh;
			return result;
		}
	private:
		template <typename E> void AddEventHandlerToList(BaseEventHandler<E>* eh)
		{
			HandlerList<E>* hlist = GetHandlerList<E>();
			if (hlist == nullptr)
			{// -> HandlerManager does not contain handler list for specified event
				HandlerList<E>* hl = new HandlerList<E>();
				hl->AddEventHandler(eh);
				m_handlers_lists.insert({ typeid(E), reinterpret_cast<BaseHandlerList*>(hl) });
			}
			else
			{// -> HandlerManager has at least one event handler for specified event
				hlist->AddEventHandler(eh);
			}
		}
		template <typename E> bool RemoveEventHandlerFromList(const BaseEventHandler<E>* eh)
		{
			HandlerList<E>* hlist = GetHandlerList<E>();
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