#ifndef WINDOW_H
#define WINDOW_H

#include "window_include.h"

#include "base_window.h"
#include "event.h"
#include "data_types.h"

#include <string>
#include <algorithm>

namespace WinapiFramework
{
	class Framework;

	class Window;
	template <> struct ConStruct<Window>;

	// ~~~~~~~~ [CLASS] Window ~~~~~~~~
	class Window
		: public ScrollableWindow
		, public HasWindowProcedure<Window>
	{
	private:
		std::wstring m_caption;
		unsigned int window_id;
		bool isMainWindow = false;
		bool isEnabled = true;
		bool isActivated = true;
		bool isMinimized = false;
		bool mouseOnWindow = false;

	public:
		struct Events : public ParentWindowEvents
		{
			struct EventActivate : public BaseEvent {};
			struct EventDeactivate : public BaseEvent {};
			struct EventMaximize : public BaseEvent {};
			struct EventMinimize : public BaseEvent {};
			struct EventHScrolled : public BaseEvent {};
			struct EventVScrolled : public BaseEvent{};
			struct EventClose : public BaseEvent
			{
			private:
				Window* mp_window;
				bool closing_aborted;
			public:
				EventClose(const EventClose* event) = delete;
				EventClose(Window* window)
					: mp_window(window)
					, closing_aborted(false)
				{}
				~EventClose() {}
			public:
				void AfterHandling() override
				{
					if (!closing_aborted)
					{
						mp_window->Destroy();
					}
				}
				void AbortClosing()
				{
					closing_aborted = true;
				}
			};
			struct EventSetCaption : public BaseEvent {};
			struct EventMinSizeChange : public BaseEvent {};
			struct EventMaxSizeChange : public BaseEvent {};
			struct EventResizeEnable : public BaseEvent {};
			struct EventResizeDisable : public BaseEvent {};
			struct EventMaximizeBoxEnable : public BaseEvent {};
			struct EventMaximizeBoxDisable : public BaseEvent {};
			struct EventMinimizeBoxEnable : public BaseEvent {};
			struct EventMinimizeBoxDisable : public BaseEvent {};
		};

		struct DestroyAction : public BaseAction
		{
		private:
			Window* m_pWindow;
		public:
			DestroyAction(Window* window);
			~DestroyAction();
		public:
			void Invoke() override;
		};

	private:
		Size m_min_size, m_max_size;


	private:
		Window(const Window &wnd) = delete;
		Window(const Window &&wnd) = delete;
		Window(unsigned int id, const ConStruct<Window> &conStruct);
		~Window();


	public:
		Window& operator=(const Window &other) = delete;
		Window& operator=(const Window &&other) = delete;


	private:
		LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		//LRESULT ProcessChildMessage(WPARAM wParam, LPARAM lParam);
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void Destroy() override;
		void Close();
	public:
		template <class ET, typename... Params> void RaiseEvent(Params... params)
		{
			ET e(params...);
			InvokeEvent(e);
		}

		void SetCaption(const std::wstring& new_caption);
		void Move(int x, int y) override;
		void MoveToCenter();
		void Resize(int width, int height) override;
		void SetMinSize(unsigned int minWidth, unsigned int minHeight);
		void SetMinSize(const Size& size);
		void SetMaxSize(unsigned int maxWidth, unsigned int maxHeight);
		void SetMaxSize(const Size& size);
		void SetAsMainWindow();

		void EnableResize();
		void DisableResize();
		void EnableMaximizeBox();
		void DisableMaximizeBox();
		void EnableMinimizeBox();
		void DisableMinimizeBox();
		void Activate();
		void Maximize();
		void Minimize();

		unsigned int GetId() const;

		MessBoxButtonPressed ShowMessageBox(
			const std::wstring& caption = L"default caption",
			const std::wstring& text = L"default text",
			MessBoxButtonLayout buttons = MessBoxButtonLayout::Ok,
			MessBoxIcon icon = MessBoxIcon::Information);

		const std::wstring& GetCaption() const;
	private:
		Point GetMousePosition() const override;
	public:
		Point GetWindowMousePosition() const;
		Point GetClientMousePosition() const;
		Point GetCanvasMousePosition() const;

		
		friend Framework;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	template <> struct ConStruct<Window>
	{
	public:
		std::wstring caption;
		Rect rect;
		Size min_size;
		Size max_size;
		Size canvas_size;


	public:
		ConStruct(
			const std::wstring& caption = L"Window",
			const Rect rect = Rect(),
			const Size& min_size = Size(100, 100),
			const Size& max_size = Size(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()),
			const Size& canvasSize = Size(800, 600))
			: caption(caption)
			, rect(rect)
			, min_size(min_size)
			, max_size(max_size)
			, canvas_size(canvasSize)
		{}
		~ConStruct() {}
	};
}

#endif // !WIN_API_WINDOW