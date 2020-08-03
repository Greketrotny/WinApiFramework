#ifndef PANEL_H
#define PANEL_H

#include "base_window.h"

namespace WinapiFramework
{
	class Panel;
	template <> struct ConStruct<Panel>;

	class Panel 
		: public ParentWindow
		, public HasWindowProcedure<Panel>
	{
	public:
		struct Events : public ParentWindowEvents
		{

		};


	private:
		Panel(const Panel& other) = delete;
		Panel(Panel&& other) = delete;
		Panel(ParentWindow* parent, const ConStruct<Panel>& conStruct);
		~Panel();


	private:
		Panel& operator=(const Panel &other) = delete;
		Panel& operator=(Panel &&other) = delete;


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
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;


	public:
		friend class ObjectCreator;
	};

	template <> struct ConStruct<Panel>
	{
		Rect rect;

		ConStruct(const Rect& rect)
			: rect(rect)
		{}
	};
}

#endif