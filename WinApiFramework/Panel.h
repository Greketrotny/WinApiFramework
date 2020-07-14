#ifndef PANEL_H
#define PANEL_H

#include "BaseControl.h"

namespace WinApiFramework
{
	class Panel;
	template <> struct ConStruct<Panel>;

	class Panel 
		: public ChildControl
		, public ParentControl
		, public HasWindowProcedure<Panel>
	{
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

			Event(Type type = Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		ChildControl::EventsManager<Panel::Event> m_events;

		std::wstring window_class_name = L"panelClassName";	// TODO: change to create differents


	private:
		Panel(const Panel& panel) = delete;
		Panel(Panel&& panel) = delete;
		Panel(ParentControl* parentControl, const ConStruct<Panel>& conStruct);
		~Panel();


	private:
		Panel& operator=(const Panel &otherButton) = delete;
		Panel& operator=(Panel &&otherButton) = delete;


	private:
		/*static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Panel* panel = (Panel*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (panel)
			{
				if (!panel->WindowProcedure(msg, wParam, lParam))	return 0;
			}
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}*/
		LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(Panel::Event((Panel::Event::Type)event.type));
		}
	public:
		Point GetMousePosition() const override;

	public:
		ChildControl::EventsManager<Panel::Event>& Events;

	public:
		friend class ControlCreator;
	};

	template <> struct ConStruct<Panel> : ConStruct<ChildControl>
	{

		ConStruct(ConStruct<ChildControl> conStruct = ConStruct<ChildControl>())
			: ConStruct<ChildControl>::ConStruct(conStruct)
		{}
	};
}

#endif