#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include "WindowInclude.h"
#include "ExternIncludes.h"
#include "DataTypes.h"

namespace WinApiFramework
{
	class ParentControl;

	// ~~~~~~~~ [CLASS] BaseControl ~~~~~~~~
	class BaseControl
	{
		// ~~ Basecontrol::fields ~~ //
	protected:
		HWND m_hWindow;
		ParentControl* const m_pParentControl;
		DWORD m_controlStyle = WS_CHILD | WS_VISIBLE; 
		Rect m_rect;
		template <class T> struct EventsManager
		{
		private:
			std::queue<T> events;
			const unsigned short buffSize = 32u;
			std::vector<std::function<void(T)>> eventHandlers;
			bool eventHandlersEnabled = true;

		public:
			// -- constructor -- //
			EventsManager() {}
			~EventsManager() {}

		public:
			// -- methods -- //
			void PushEvent(T newEvent)
			{
				// push event to buffer
				events.push(newEvent);
				if (events.size() > buffSize)
					events.pop();

				// call handler function
				if (eventHandlersEnabled)
				{
					for (unsigned int i = 0u; i < eventHandlers.size(); ++i)
					{
						eventHandlers[i](newEvent);
					}
				}
			}
			T GetEvent()
			{
				if (events.size() > 0u)
				{
					T e = events.front();
					events.pop();
					return e;
				}
				else
				{
					return T();
				}
			}
			void ClearBuffer()
			{
				events = std::queue<T>();
			}
			template <class EventReceiver> void AddEventHandler(EventReceiver* receiverObject, void(EventReceiver::*eventFunction)(T))
			{
				using std::placeholders::_1;
				std::function<void(T)> f;
				f = std::bind(eventFunction, receiverObject, _1);
				eventHandlers.push_back(f);
			}
			void RemoveAllEventHandlers()
			{
				eventHandlers.clear();
			}
			void EnableEventHandlers()
			{
				eventHandlersEnabled = true;
			}
			void DisableEventHandlers()
			{
				eventHandlersEnabled = false;
			}
		};
	public:
		struct ConStruct
		{
			Rect rect;

			ConStruct(const ConStruct& conStruct)
				: rect(conStruct.rect)
			{}
			ConStruct(Rect rect = Rect())
				: rect(rect)
			{}
		};
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4
			};
			Type type;

			Event()
			{
				type = Event::Type::Invalid;
			}
			Event(Event::Type type)
			{
				this->type = type;
			}
		};


		// ~~ BaseControl::constructor ~~ //
	public:
		explicit BaseControl(const BaseControl::ConStruct& conStruct, ParentControl* parentControl = nullptr);
		virtual ~BaseControl();


		// ~~ BaseControl::methods ~~ //
	public:
		HWND GetControlHandle() const;
		virtual int ControlProcedure(WPARAM wParam, LPARAM lParam) { return 1; };

		virtual void CreateControlWindow() = 0;
		virtual void DestroyControlWindow() = 0;

		virtual void PushBaseEvent(WindowControl::Event event) = 0;

		void Enable();
		void Disable();

		void Move(int x, int y);
		void Resize(int width, int height);
		void SetRect(Rect rect);

		int GetMouseX();
		int GetMouseY();
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentControl ~~~~~~~~
	class ParentControl
	{
		// -- ParentControl::constructor -- //
	public:
		ParentControl();
		virtual ~ParentControl();


		// -- ParentControl::methods -- //
	protected:
		virtual void AddControl(BaseControl* childControl) = 0;
		virtual void RemoveControl(BaseControl* childControl) = 0;

		
		// ~~ ParentControl::friends ~~ //
	public:
		friend class BaseControl;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !STORAGE_CONTROL_H