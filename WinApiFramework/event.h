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



	template <typename E> using event_handler_t = std::function<void(E&)>;

	struct BaseHandlerList
	{
		BaseHandlerList() = default;
		virtual ~BaseHandlerList() {}
	};
	template <typename E> struct HandlerList : public BaseHandlerList
	{
	private:
		std::vector<event_handler_t<E>> m_efl;

	public:
		HandlerList() {}
		~HandlerList() {}

	public:
		void AddEventHandler(const event_handler_t <E>& eh)
		{
			m_efl.push_back(eh);
		}
		void Invoke(E& e)
		{
			for (auto& h : m_efl)
			{
				h(e);
			}
		}
	};

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
		std::unordered_map<TypeInfoRef, BaseHandlerList*, Hasher, EqualTo> m_handlers_list;

	public:
		EventHandlerManager()
		{

		}
		~EventHandlerManager()
		{
			for (auto& h : m_handlers_list)
			{
				delete h.second;
			}
			m_handlers_list.clear();
		}

	public:
		// Find list of event handlers for specified event type
		template <typename E> HandlerList<E>* GetHandlerList()
		{
			auto search = m_handlers_list.find(typeid(E));
			if (search != m_handlers_list.end())
			{
				//return reinterpret_cast<HandlerList<E>*>(search->second);
				return dynamic_cast<HandlerList<E>*>(search->second);
			}
			else
			{
				return nullptr;
			}
		}
		template <typename E, typename ER> void AddEventHandler(ER* er, void(ER::*eh)(E&))
		{			
			AddEventHandlerToList(event_handler_t<E>(std::bind(eh, static_cast<ER*>(er), std::placeholders::_1)));
		}
		template <typename E> void AddEventHandler(void(*eh)(E&))
		{			
			AddEventHandlerToList(event_handler_t<E>(eh));
		}
	private:
		template <typename E> void AddEventHandlerToList(const event_handler_t<E>& eh)
		{
			HandlerList<E>* hlist = GetHandlerList<E>();
			if (hlist == nullptr)
			{// -> HandlerManager does not contain handler list for specified event
				HandlerList<E>* hl = new HandlerList<E>();
				hl->AddEventHandler(eh);
				m_handlers_list.insert({ typeid(E), reinterpret_cast<BaseHandlerList*>(hl) });
			}
			else
			{// -> HandlerManager has at least one event handler for specified event
				hlist->AddEventHandler(eh);
			}
		}
	};
}

#endif