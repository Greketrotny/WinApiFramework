#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include "WindowInclude.h"
#include "ExternIncludes.h"
#include "DataTypes.h"

namespace WinApiFramework
{
	class ParentControl;

	enum ProcedureResult
	{
		Handled,
		Unhandled,
		TargetNotFound
	};

	// ~~~~~~~~ [CLASS] BaseControl ~~~~~~~~
	class BaseControl
	{
		// ~~ Basecontrol::fields ~~ //
	protected:
		HWND m_hWindow;


		// ~~ BaseControl::constructor ~~ //
	public:
		explicit BaseControl();
		virtual ~BaseControl();


		// ~~ BaseControl::methods -- //
	public:
		const HWND& GetWindowHandle() const;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ChildControl ~~~~~~~~
	class ChildControl;
	template <> struct ConStruct<ChildControl>;

	class ChildControl : virtual public BaseControl
	{
		// ~~ ChildControl::fields ~~ //
	protected:
		ParentControl* const m_pParentControl;
		DWORD m_controlStyle = WS_CHILD | WS_VISIBLE;
		Rect m_rect;
	public:
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
	protected:
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


		// ~~ ChildControl::constructor ~~ //
	public:
		ChildControl(ParentControl* const parentControl, const ConStruct<ChildControl>& conStruct);
		virtual ~ChildControl();


		// ~~ ChildControl::methods ~~ //
	protected:
		virtual ProcedureResult ControlProcedure(WPARAM wParam, LPARAM lParam) = 0;
		virtual bool CreateControlWindow() = 0;
		virtual void DestroyControlWindow() = 0;
		virtual void PushBaseEvent(ChildControl::Event event) = 0;
	public:
		void Destroy();
	public:
		void Enable();
		void Disable();

		void Move(int x, int y);
		void Move(const Point& position);
		void Resize(int width, int height);
		void Resize(const Size& size);
		void SetRect(const Rect& rect);

		const Rect& GetRect();
		Point GetMousePosition() const;
		bool IsMouseInside() const;


		// ~~ property fields ~~ //
	public:
		const Rect& Rectangle;


		// ~~ ChildControl::friends ~~ //
	public:
		friend class ParentControl;
		friend class Window;
	};


	// ~~~~~~~~ [STRUCT] ConStruct<ChildControl> ~~~~~~~~
	/*
	* ConStruct template specialization for ChildControl
	*/
	template <> struct ConStruct<ChildControl>
	{
		Rect rect;

		ConStruct(const ConStruct& conStruct)
			: rect(conStruct.rect)
		{}
		ConStruct(Rect rect = Rect())
			: rect(rect)
		{}
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] Scrollable ~~~~~~~~
	class Scrollable
	{
		// ~~ Scrollable::fields ~~ //
	protected:
		Rect m_canvasRect;


		// ~~ Scrollable::constructor ~~ //
	public:
		Scrollable();
		~Scrollable();


		// ~~ Scrollable::methods ~~ //
	public:
		Rect GetCanvasRect() const;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ControlCreator ~~~~~~~~
	class ControlCreator
	{
	protected:
		template <class T> static T* CreateControl(ParentControl* const parentControl, const ConStruct<T>& conStruct)
		{
			return new T(parentControl, conStruct);
		}
		static bool DestroyControl(ChildControl*& childControl)
		{
			if (childControl == nullptr) return false;

			delete childControl;
			childControl = nullptr;
			return true;
		}
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentControl ~~~~~~~~
	class ParentControl 
		: virtual public BaseControl
		, public Scrollable
		, public ControlCreator
	{
		// ~~ ParentControl::fields ~~ //
	private:
		std::vector<ChildControl*> m_controls;


		// ~~ ParentControl::constructor ~~ //
	protected:
		ParentControl();
		virtual ~ParentControl();


		// ~~ ParentControl::methods ~~ //
	public:
		template <class T> T* CreateControl(const ConStruct<T>& conStruct)
		{
			// create control
			T* newControl = ControlCreator::CreateControl<T>(this, conStruct);

			// add control to m_controls
			m_controls.push_back(newControl);

			return newControl;
		}
		bool DestroyControl(ChildControl* control);
		void DestroyAllChildControls();
	public:
		virtual Point GetMousePosition() const = 0;
	protected:
		ProcedureResult ProcessChildMessage(WPARAM wParam, LPARAM lParam);
		//ChildControl* const GetChildControl(size_t index);
		//size_t GetControlCount();
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !STORAGE_CONTROL_H