#ifndef WIN_API_WINDOW_CONTROLS_H
#define WIN_API_WINDOW_CONTROLS_H

#include "WindowInclude.h"

#include <string>
#include <queue>

#include <d2d1.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")


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
		struct Rect
		{
			int x = 50, y = 50;
			unsigned int width = 100, height = 100;
		};
		struct Config
		{
			Rect rect;
		};
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move	= 1,
				Resize	= 2,
				Enable	= 3,
				Disable	= 4
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
		struct EventHandler
		{
		protected:
			void HandleBaseEvent(WindowControl::Event::Type eventType)
			{
				switch (eventType)
				{
				case WindowControl::Event::Move:	Move();		break;
				case WindowControl::Event::Resize:	Resize();	break;
				case WindowControl::Event::Enable:	Enable();	break;
				case WindowControl::Event::Disable:	Disable();	break;
				}
			}
		public:
			virtual void Move() {};
			virtual void Resize() {};
			virtual void Enable() {};
			virtual void Disable() {};
		};
	protected:
		Rect rect;
		const unsigned short eventBuffSize = 32u;


		// -- constructors -- //
	protected:
		WindowControl(const WindowControl::Config& config);
		virtual ~WindowControl();


		// -- methods -- //
	protected:
		virtual int ControlProc(WPARAM wParam, LPARAM lParam) = 0;
		virtual bool CreateControlWindow() = 0;
		void DestroyControlWindow();
		virtual void PushBaseEvent(WindowControl::Event event) = 0;
	public:
		void EnableControl();
		void DisableControl();
		virtual void SetPosition(int x, int y);
		void SetDimensions(unsigned int width, unsigned int height);
		int GetMouseX();
		int GetMouseY();


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
		std::wstring caption;
	public:
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"Default";
		};
		struct Event

		{
			enum Type
			{
				Invalid	= 0,
				Move	= 1,
				Resize	= 2,
				Enable	= 3,
				Disable	= 4,
				Click,
				DoubleClick,
				Focus,
				Unfocus
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
		struct EventHandler : public WindowControl::EventHandler
		{
			virtual void HandleEvent(Button::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case Button::Event::Type::Click:		Click();		break;
				case Button::Event::Type::DoubleClick:	DoubleClick();	break;
				case Button::Event::Type::Focus:		Focus();		break;
				case Button::Event::Type::Unfocus:		Unfocus();		break;
				}
			}
			virtual void Click() {};
			virtual void DoubleClick() {};
			virtual void Focus() {};
			virtual void Unfocus() {};
		};
	private:
		std::queue<Event> events;
		EventHandler *eventHandler = nullptr;


		// -- constructors -- //
	public:
		Button(const Button &otherButton) = delete;
		Button(const Button &&otherButton) = delete;
		Button(const Config &config);
		Button(const Config &config, EventHandler *eventHandler);
		~Button();


		// -- operators -- //
		Button& operator=(const Button &otherButton) = delete;
		Button& operator=(const Button &&otherButton) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{ 
			PushEvent(Button::Event((Button::Event::Type)event.type));
		}
	public:
		void PushEvent(Button::Event newEvent);
		Button::Event GetEvent();
		void ClearEventBuffer();
		void SetEventHandler(EventHandler *eventHandler);
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
				Invalid = 0,
				Move	= 1,
				Resize	= 2,
				Enable	= 3,
				Disable = 4,
				Check,
				UnCheck,
				MiddleState
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
		struct EventHandler : public WindowControl::EventHandler
		{
			virtual void HandleEvent(CheckBox::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case CheckBox::Event::Type::Check:			Check();		break;
				case CheckBox::Event::Type::UnCheck:		UnCheck();		break;
				case CheckBox::Event::Type::MiddleState:	MiddleState();	break;
				}
			}
			virtual void Check() {};
			virtual void UnCheck() {};
			virtual void MiddleState() {};
		};
	private:
		std::wstring caption = L"Default";
		bool isTripleState = 0;
		BoxState boxState;

		std::queue<Event> events;
		EventHandler *eventHandler = nullptr;


		// -- constructors -- //
	public:
		CheckBox(const CheckBox& checkBox) = delete;
		CheckBox(const CheckBox&& checkBox) = delete;
		CheckBox(const Config& config);
		CheckBox(const Config& config, EventHandler *eventHandler);
		~CheckBox();


		// -- operators -- //
	public:
		CheckBox& operator=(const CheckBox& checkBox) = delete;
		CheckBox& operator=(const CheckBox&& checkBox) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			PushEvent(CheckBox::Event((CheckBox::Event::Type)event.type));
		}
	public:
		void PushEvent(CheckBox::Event newEvent);
		Event GetEvent();
		void ClearEventBuffer();
		void EnableControl();
		void DisableControl();
		void SetEventHandler(EventHandler *eventHandler);
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

	// class Slider
	/*//class Slider : public WindowControl
	//{
	//	// -- fields -- //
	//private:
	//	
	//public:
	//	struct Config : public WindowControl
	//	{

	//	};
	//	struct Event
	//	{
	//		enum Type
	//		{
	//			Invalid = 0,
	//			Move = 1,
	//			Resize = 2,
	//			Enable = 3,
	//			Disable = 4,
	//		};
	//		Type type;

	//		Event()
	//		{
	//			this->type = Invalid;
	//		}
	//		Event(Type type)
	//		{
	//			this->type = type;
	//		}
	//	};
	//	struct EventHandler : public WindowControl::EventHandler
	//	{
	//		virtual void HandleEvent(Slider::Event event)
	//		{
	//			HandleBaseEvent((WindowControl::Event::Type)event.type);

	//			switch (event.type)
	//			{
	//				// cases
	//			}
	//		}
	//		// virtual functions
	//	};

	//	// -- constructor -- //
	//public:
	//	Slider(const Slider &slider) = delete;
	//	Slider(const Slider &&slider) = delete;
	//	Slider(const Config &config);
	//	Slider(const Config &config, EventHandler *eventHandler);


	//	// -- operators -- //
	//public:
	//	Slider& operator=(const Slider& slider) = delete;
	//	Slider& operator-(const Slider &&slider) = delete;


	//	// -- methods -- //
	//private:
	//	int ControlProc(WPARAM wParam, LPARAM lParam) override;
	//	bool CreateControlWindow() override;
	//	void PushBaseEvent(WindowControl::Event event) override
	//	{
	//		PushEvent(Slider::Event((Slider::Event::Type)event.type));
	//	}
	//public:
	//	void PushEvent(Slider::Event event);

	//};*/

	class Label : public WindowControl
	{
		// -- fields -- //
	private:
		std::wstring caption;
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
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4,
				CaptionChange,
				TextAlignmentChange
			};
			Type type;

			Event()
			{
				type = Invalid;
			}
			Event(Label::Event::Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler
		{
			virtual void HandleEvent(Label::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case Label::Event::Type::CaptionChange:			CaptionChange();		break;
				case Label::Event::Type::TextAlignmentChange:	TextAlignmentChange();	break;
				}
			}
			virtual void CaptionChange() {};
			virtual void TextAlignmentChange() {};
		};
	private:
		std::queue<Label::Event> events;
		EventHandler *eventHandler = nullptr;
		TextAlignment textAlignment;


		// -- constructors -- //
	public:
		Label(const Label& label) = delete;
		Label(const Label&& label) = delete;
		Label(const Config &config);
		Label(const Config &config, EventHandler *eventHandler);
		~Label();


		// -- operators -- //
	public:
		Label operator=(const Label& label) = delete;
		Label operator=(const Label&& label) = delete;

		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			PushEvent(Label::Event((Label::Event::Type)event.type));
		}
	public:
		void PushEvent(Label::Event newEvent);
		Label::Event GetEvent();
		void ClearEventBuffer();
		void SetEventHandler(EventHandler *eventHandler);
		void SetCaption(std::wstring newCaption);
		void SetDimensions(unsigned int width, unsigned int height);
		void SetTextAligment(TextAlignment textAlignment);


		// -- property fields -- //
	public:
		const std::wstring& Caption;
		const TextAlignment& Alignment;
	};
	/*In progress*/class RadioButton : public WindowControl
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
	/*In progress*/class GroupBox : public WindowControl
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
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4,
				MinValueChange,
				MaxValueChange,
				PositionChange,
				StepChange,
				BarStateChange
			};
			Type type;

			Event()
			{
				type = Invalid;
			}
			Event(ProgressBar::Event::Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler
		{
			virtual void HandleEvent(ProgressBar::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case ProgressBar::Event::Type::MinValueChange:	MinValueChange();	break;
				case ProgressBar::Event::Type::MaxValueChange:	MaxValueChange();	break;
				case ProgressBar::Event::Type::PositionChange:	PositionChange();	break;
				case ProgressBar::Event::Type::StepChange:		StepChange();		break;
				case ProgressBar::Event::Type::BarStateChange:	BarStateChange();	break;
				}
			}
			virtual void MinValueChange() {};
			virtual void MaxValueChange() {};
			virtual void PositionChange() {};
			virtual void StepChange() {};
			virtual void BarStateChange() {};
		};
	private:
		BarState barState;
		std::queue<ProgressBar::Event> events;
		EventHandler *eventHandler = nullptr;

		// -- constrctor -- //
	public:
		ProgressBar(const ProgressBar &progressBar) = delete;
		ProgressBar(const ProgressBar &&progressBar) = delete;
		ProgressBar(const Config& config);
		ProgressBar(const Config& config, EventHandler *eventHandler);
		~ProgressBar();


		// -- operators -- //
	public:
		ProgressBar& operator=(const ProgressBar &progressBar) = delete;
		ProgressBar& operator=(const ProgressBar &&progressBar) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			PushEvent(ProgressBar::Event((ProgressBar::Event::Type)event.type));
		}
	public:
		void PushEvent(ProgressBar::Event newEvent);
		ProgressBar::Event GetEvent();
		void ClearEventBuffer();
		void SetEventHandler(EventHandler *eventHandler);
		void SetMinValue(unsigned int value);
		void SetMaxValue(unsigned int value);
		void SetRange(unsigned int min, unsigned int max);
		void SetStep(unsigned int step);
		void SetPosition(unsigned int position);
		void SetState(BarState state);
		void StepIt();


		// -- property fields -- //
	public:
		const unsigned int& MinValue;
		const unsigned int& MaxValue;
		const unsigned int& Position;
		const unsigned int& Step;
		const BarState& State;
	};
	/*In progress*/class GraphicsBox : public WindowControl
	{
		// -- fields -- //
	private:
		ID2D1Factory* D2DFactory = NULL;
		IWICImagingFactory *ImageFactory = NULL;	
		ID2D1HwndRenderTarget* RT = NULL;
		ID2D1Bitmap *D2DBitmap;
		IWICBitmap *ImageBitmap;

		RECT rc;
		bool initialized = false;

	public:
		struct Config : WindowControl::Config
		{

		};
		
		// -- constructors -- //
	public:
		GraphicsBox(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox(const GraphicsBox&& graphicsBox) = delete;
		GraphicsBox(const Config &config);
		~GraphicsBox();


		// -- operators -- //
	public:
		GraphicsBox& operator=(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox& operator=(const GraphicsBox&& graphicsBox) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{

		}
	public:
		void InitGraphics2D();
		void DrawLine(int x, int y);
	};
}

#endif // !WIN_API_WINDOW_CONTROLS_H