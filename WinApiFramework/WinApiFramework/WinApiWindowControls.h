#ifndef WIN_API_WINDOW_CONTROLS_H
#define WIN_API_WINDOW_CONTROLS_H

#include "WindowInclude.h"

#include <string>
#include <queue>


namespace WinApiFramework
{
	class Window;
	class GroupBox;


	class WindowControl
	{
		// -- fields -- //
	protected:
		HWND hControl;
		Window* parentWindow = nullptr;
		unsigned int id;

	public:
		struct EventType
		{
			static constexpr unsigned short Move = 1;
			static constexpr unsigned short Resize = 2;
			static constexpr unsigned short Enable = 3;
			static constexpr unsigned short Disable = 4;
		};
		struct Rect
		{
			int x = 50, y = 50;
			unsigned int width = 100, height = 100;
		};
		struct Config
		{
			Rect rect;
		};
	protected:
		Rect rect;


		// -- constructors -- //
	protected:
		WindowControl(const WindowControl::Config& config);
		virtual ~WindowControl();


		// -- methods -- //
	protected:
		virtual int ControlProc(WPARAM wParam, LPARAM lParam) = 0;
		virtual bool CreateControlWindow() = 0;
		void DestroyControlWindow();
	public:
		void EnableControl();
		void DisableControl();
		virtual void SetPosition(int x, int y);
		void SetDimensions(unsigned int width, unsigned int height);


		// -- property fields -- //
	public:
		const Rect& Rectangle;


		// -- friends -- //
		friend Window;
		friend GroupBox;
	};

	class Button : public WindowControl
	{
		// -- fields -- //
	private:
		std::wstring caption = L"Default";
	public:
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"Default";
		};
		struct Event
		{
			enum Type
			{
				Move,
				Resize,
				Enable,
				Disable,
				Click,
				DoubleClick,
				Focus,
				Unfocus,
				Invalid
			};
			Type type;

			Event()
			{
				this->type = Invalid;
			}
			Event(Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler
		{
			virtual void HandleEvent(Event event)
			{
				switch (event.type)
				{
				case Button::Event::Type::Click:		Click();		break;
				case Button::Event::Type::DoubleClick:	DoubleClick();	break;
				case Button::Event::Type::Enable:		Enable();		break;
				case Button::Event::Type::Disable:		Disable();		break;
				case Button::Event::Type::Focus:		Focus();		break;
				case Button::Event::Type::Unfocus:		Unfocus();		break;
				}
			}
			virtual void Click() {};
			virtual void DoubleClick() {};
			virtual void Enable() {};
			virtual void Disable() {};
			virtual void Focus() {};
			virtual void Unfocus() {};
		};
	private:
		std::queue<Event> events;
		const unsigned short buffLength = 32u;
		EventHandler *eventHandler = nullptr;


		// -- constructors -- //
	public:
		Button(const Button &otherButton) = delete;
		Button(const Button &&otherButton) = delete;
		Button(const Config &config);
		Button(const Config &config, EventHandler &eventHandler);
		~Button();


		// -- operators -- //
		Button& operator=(const Button &otherButton) = delete;
		Button& operator=(const Button &&otherButton) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
	public:
		void PushEvent(Button::Event newEvent);
		Event GetEvent();
		void ClearEventBuffer();
		void SetEventHandler(EventHandler *eh);
		void SetCaption(std::wstring newCaption);
		void SetPosition(int x, int y);
		void SetDimensions(unsigned int width, unsigned int height);


		// -- property fields -- //
		const Rect& Rect;
		const std::wstring& Caption;
	};
	class CheckBox : public WindowControl
	{
		// -- fields -- //
		// CheckBox structs & enums
	public:
		enum BoxState
		{
			UnCheck,
			Check,
			MiddleState
		};
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"Default";
			bool isTripleState = 0;
			BoxState boxState = UnCheck;
		};
		struct Event
		{
			enum Type
			{
				Move,
				Resize,
				Check,
				UnCheck,
				MiddleState,
				Enable,
				Disable,
				Invalid
			};
			Type type;

			Event()
			{
				this->type = Invalid;
			}
			Event(Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler
		{
			virtual void HandleEvent(CheckBox::Event event) = 0;
		};
	private:
		std::wstring caption = L"Default";
		bool isTripleState = 0;
		BoxState boxState;

		std::queue<Event> events;
		const unsigned short buffLength = 32u;
		EventHandler *eventHandler = nullptr;


		// -- constructors -- //
	public:
		CheckBox(const CheckBox& checkBox) = delete;
		CheckBox(const CheckBox&& checkBox) = delete;
		CheckBox(const Config& config);
		CheckBox(const Config& config, EventHandler &eventHandler);
		~CheckBox();


		// -- operators -- //
	public:
		CheckBox& operator=(const CheckBox& checkBox) = delete;
		CheckBox& operator=(const CheckBox&& checkBox) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushEvent(Event newEvent);
	public:
		Event GetEvent();
		void ClearEventBuffer();
		void EnableControl();
		void DisableControl();
		void SetEventHandler(EventHandler &eh);
		void SetCaption(std::wstring newCaption);
		void SetBoxState(BoxState newState);
		void SetBoxState(unsigned int newState);
		void SetPosition(int x, int y);
		void SetDimensions(unsigned int width, unsigned int height);


		// -- property fields -- //
		const BoxState& State;
		const std::wstring& Caption;
		const bool& IsTripleState;
	};
	class Label : public WindowControl
	{
		// -- fields -- //
	public:
		enum TextAlignment
		{
			Left,
			Center,
			Right
		};
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"default";
			TextAlignment textAlignment = TextAlignment::Left;
		};
	private:
		std::wstring caption;
		TextAlignment textAlignment;


		// -- constructors -- //
	public:
		Label(const Label& label) = delete;
		Label(const Label&& label) = delete;
		Label(const Config& config);
		~Label();


		// -- operators -- //
	public:
		Label operator=(const Label& label) = delete;
		Label operator=(const Label&& label) = delete;

		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
	public:
		void SetCaption(std::wstring newCaption);
		void SetDimensions(unsigned int width, unsigned int height);
		void SetTextAligment(TextAlignment textAlignment);


		// -- property fields -- //
	public:
		const std::wstring& Caption;
		const TextAlignment& Alignment;
	};
	class RadioButton : public WindowControl
	{
		// -- fields -- //
	private:
	public:
		struct Config : public WindowControl::Config
		{

		};
	private:

		// -- constructors -- //
	public:
		RadioButton(const RadioButton& radioButton) = delete;
		RadioButton(const RadioButton&& radioButton) = delete;
		RadioButton(const Config& config);
		~RadioButton();


		// -- operators -- //
	public:
		RadioButton& operator=(const RadioButton& radioButton) = delete;
		RadioButton& operator=(const RadioButton&& radioButton) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
	};
	class GroupBox : public WindowControl
	{
		// -- fields -- //
	private:
		std::vector<WindowControl*> controls;
		std::wstring caption = L"Group";
	public:
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"Group";
		};

		// -- constructor -- //
	public:
		GroupBox(const GroupBox& groupBox) = delete;
		GroupBox(const GroupBox&& groupBox) = delete;
		GroupBox(const Config& config);
		~GroupBox();

		// -- operators -- //
	public:
		GroupBox& operator=(const GroupBox &groupBox) = delete;
		GroupBox& operator=(const GroupBox &&groupBox) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
	public:
		void SetCaption(std::wstring newCaption);
		void SetPosition(int x, int y);
		void AddControl(WindowControl *control);
		void RemoveControl(WindowControl *control);


		// -- property fields -- //
	public:
		const std::wstring& Caption;
	};
	class ProgressBar : public WindowControl
	{
		// -- fields -- //
	private:
		unsigned int minValue, maxValue, position, step;

	public:
		enum BarState
		{
			Normal,
			Pause,
			Error
		};
		struct Config : WindowControl::Config
		{
			unsigned int minValue = 0;
			unsigned int maxValue = 100;
			unsigned int position = 0;
			unsigned int step = 1;
		};
	private:
		BarState barState;

		// -- constrctor -- //
	public:
		ProgressBar(const ProgressBar &progressBar) = delete;
		ProgressBar(const ProgressBar &&progressBar) = delete;
		ProgressBar(const Config& config);
		~ProgressBar();


		// -- operators -- //
	public:
		ProgressBar& operator=(const ProgressBar &progressBar) = delete;
		ProgressBar& operator=(const ProgressBar &&progressBar) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
	public:
		void SetMinValue(unsigned int value);
		void SetMaxValue(unsigned int value);
		void SetRange(unsigned int min, unsigned int max);
		void SetStep(unsigned int step);
		void SetPosition(unsigned int position);
		void SetState(BarState state);


		// -- property fields -- //
	public:
		const unsigned int& MinValue;
		const unsigned int& MaxValue;
		const unsigned int& Position;
		const unsigned int& Step;
	};
	class GraphicsBox : public WindowControl
	{
		// -- fields -- //
	private:



		// -- constructors -- //
	public:
		GraphicsBox(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox(const GraphicsBox&& graphicsBox) = delete;


		// -- operators -- //
	public:
		GraphicsBox& operator=(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox& operator=(const GraphicsBox&& graphicsBox) = delete;
	};
}

#endif // !WIN_API_WINDOW_CONTROLS_H