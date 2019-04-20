#ifndef WIN_API_WINDOW_CONTROLS_H
#define WIN_API_WINDOW_CONTROLS_H

#include "WindowInclude.h"

#include <string>
#include <queue>

#include <d2d1helper.h>
#include <d2d1effects.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")


namespace WinApiFramework
{
	class Window;

	class WindowControl
	{
		// -- fields -- //
	protected:
		HWND hControl;
		Window* parentWindow = nullptr;
		DWORD controlStyle = WS_CHILD | WS_VISIBLE;
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
		template <class T> struct EventHandler
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
			virtual void HandleEvent(T event) = 0;
			virtual void Move() {};
			virtual void Resize() {};
			virtual void Enable() {};
			virtual void Disable() {};
		};
		template <class T> struct Events
		{
		private:
			std::queue<T> events;
			const unsigned short buffSize = 32u;
			WindowControl::EventHandler<T> *eventHandler = nullptr;

		public:
			// -- constructor -- //
			Events()
			{

			}
			~Events()
			{
				if (eventHandler)
				{
					delete eventHandler;
					eventHandler = nullptr;
				}
			}

		public:
			// -- methods -- //
			void PushEvent(T newEvent)
			{
				// push event to buffer
				events.push(newEvent);
				if (events.size() > buffSize)
					events.pop();

				// call handler function
				if (eventHandler) eventHandler->HandleEvent(newEvent);
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
			void SetEventHandler(WindowControl::EventHandler<T> *eventHandler)
			{
				this->eventHandler = eventHandler;
			}
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
		virtual void PushBaseEvent(WindowControl::Event event) = 0;
	public:
		void EnableControl();
		void DisableControl();
		virtual void Move(int x, int y);
		virtual void Resize(unsigned int width, unsigned int height);
		int GetMouseX();
		int GetMouseY();


		// -- property fields -- //
	public:
		const int& X;
		const int& Y;
		const unsigned int& Width;
		const unsigned int& Height;
		const Rect& Rectangle;


		// -- friends -- //
		friend Window;
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
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4,
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
		struct EventHandler : public WindowControl::EventHandler<Button::Event>
		{
			virtual void HandleEvent(Button::Event event) override
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
		WindowControl::Events<Button::Event> events;


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
			events.PushEvent(Button::Event((Button::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring newCaption);


		// -- property fields -- //
		const Rect& Rect;
		const std::wstring& Caption;
		WindowControl::Events<Button::Event>& Events;
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
				Move = 1,
				Resize = 2,
				Enable = 3,
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
		struct EventHandler : public WindowControl::EventHandler<CheckBox::Event>
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
		WindowControl::Events<CheckBox::Event> events;


		// -- constructors -- //
	public:
		CheckBox(const CheckBox& checkBox) = delete;
		CheckBox(const CheckBox&& checkBox) = delete;
		CheckBox(const Config& config);
		CheckBox(const Config& config, CheckBox::EventHandler *eventHandler);
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
			events.PushEvent(CheckBox::Event((CheckBox::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring newCaption);
		void SetBoxState(BoxState newState);
		void SetBoxState(unsigned int newState);
		void EnableTripleState();
		void DisableTripleState();


		// -- property fields -- //
		const BoxState& State;
		const std::wstring& Caption;
		const bool& IsTripleState;
	};
	class TrackBar : public WindowControl
	{
		// -- fields -- //
	private:
		bool selectRangeEnabled = false;
		unsigned int smallStep = 1u, largeStep = 10u;
		int thumbPosition;
		HWND hLabel1, hLabel2;

	public:
		struct Range
		{
			int min = 0, max = 100;
		};
		struct Labels
		{
			std::wstring label1 = L"";
			std::wstring label2 = L"";
		};
		enum Orientation
		{
			Horizontal,
			Vertical
		};
		struct Config : public WindowControl::Config
		{
			Range trackRange;
			Range selectRange;
			Labels labels;
			Orientation orientation;
			int startPosition;
			unsigned int smallStep, largeStep;
			bool EnableSelectRange = false;
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
				TrackPosChange
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
		struct EventHandler : public WindowControl::EventHandler<TrackBar::Event>
		{
			virtual void HandleEvent(TrackBar::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case TrackBar::Event::Type::MaxValueChange: MaxValueChange(); break;
				case TrackBar::Event::Type::MinValueChange: MinValueChange(); break;
				case TrackBar::Event::Type::TrackPosChange: TrackPosChange(); break;
				}
			}
			virtual void MinValueChange() {};
			virtual void MaxValueChange() {};
			virtual void TrackPosChange() {};
		};
	private:
		WindowControl::Events<TrackBar::Event> events;
		Range trackRange;
		Range selectRange;
		Labels labels;
		Orientation orientation;


		// -- constructors -- //
	public:
		TrackBar(const TrackBar &TrackBar) = delete;
		TrackBar(const TrackBar &&TrackBar) = delete;
		TrackBar(const Config &config);
		TrackBar(const Config &config, EventHandler *eventHandler);
		~TrackBar();


		// -- operators -- //
	public:
		TrackBar& operator=(const TrackBar& TrackBar) = delete;
		TrackBar& operator-(const TrackBar &&TrackBar) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(TrackBar::Event((TrackBar::Event::Type)event.type));
		}
	public:
		void SetPosition(int x, int y);
		void SetMinTrackValue(int value);
		void SetMaxTrackValue(int value);
		void SetMinSelectValue(int value);
		void SetMaxSelectValue(int value);
		void SetTrackRange(Range newRAnge);
		void SetTrackRange(int minValue, int maxValue);
		void SetSelectRange(Range newSelectRange);
		void SetSelectRange(int minValue, int maxValue);
		void SetThumbPosition(int newPosition);
		void SetSmallStep(unsigned int smallStep);
		void SetLargeStep(unsigned int largeStep);
		void EnableSelectRange();
		void DisableSelectRange();
		void SetLabel1(const std::wstring& label1);
		void SetLabel2(const std::wstring& label2);
		void SetLabels(const std::wstring& label1, const std::wstring& label2);
		void SetLabels(const Labels& newLabels);

		// -- property fields -- //
	public:
		const int& Position;
		const int& TrackMin;
		const int& TrackMax;
		const Orientation& Orientation;
		const unsigned int& SmallStep;
		const unsigned int& LargeStep;
	};
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
		struct EventHandler : public WindowControl::EventHandler<Label::Event>
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
		WindowControl::Events<Label::Event> events;
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
			events.PushEvent(Label::Event((Label::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring newCaption);
		void SetTextAligment(TextAlignment textAlignment);


		// -- property fields -- //
	public:
		const std::wstring& Caption;
		const TextAlignment& Alignment;
		WindowControl::Events<Label::Event>& Events;
	};
	class Edit : public WindowControl
	{
		// -- fields -- //
	private:
		std::wstring text = L"";
		bool passwordMode = false;
		bool numberOnly = false;
		unsigned int textLengthLimit = 0xFFFF;

	public:
		enum TextAlignment
		{
			Left,
			Center,
			Right
		};
		enum LettersMode
		{
			UpperCase,
			LowerCase,
			Either
		};
		struct Config : public WindowControl::Config
		{
			TextAlignment textAlignment = TextAlignment::Left;
			LettersMode lettersMode = LettersMode::Either;
			std::wstring text = L"";
			bool passwordMode = false;
			bool numberOnly = false;
			unsigned int textLengthLimit = 0xFFFF;
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
				Text,
				TextAlignment,
				LettersMode,
				PasswordMode,
				NumberMode,
				TextLimitReach
			};
			Type type;

			Event()
			{
				type = Invalid;
			}
			Event(Edit::Event::Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler<Edit::Event>
		{
			virtual void HandleEvent(Edit::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case Edit::Event::Type::Text:			TextChange();		break;
				case Edit::Event::Type::TextAlignment:	TextAlignment();	break;
				case Edit::Event::Type::LettersMode:	LettersMode();		break;
				case Edit::Event::Type::PasswordMode:	PasswordMode();		break;
				case Edit::Event::Type::NumberMode:		NumberMode();		break;
				case Edit::Event::Type::TextLimitReach:	TextLimitReach();	break;
				}
			}
			virtual void TextChange() {};
			virtual void TextAlignment() {};
			virtual void LettersMode() {};
			virtual void PasswordMode() {};
			virtual void NumberMode() {};
			virtual void TextLimitReach() {};
		};
	private:
		WindowControl::Events<Edit::Event> events;
		TextAlignment textAlignment = TextAlignment::Left;
		LettersMode lettersMode = LettersMode::Either;


		// -- contstructors -- //
	public:
		Edit(const Edit& edit) = delete;
		Edit(const Edit&& edit) = delete;
		Edit(const Config& config);
		Edit(const Config& config, EventHandler *eventHandler);
		~Edit();


		// -- operators -- //
	public:
		Edit operator=(const Edit& edit) = delete;
		Edit operator=(const Edit&& edit) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(Edit::Event((Edit::Event::Type)event.type));
		}
	public:
		void SetText(std::wstring newText);
		void SetTextAlignment(Edit::TextAlignment newAlignment);
		void SetLettersMode(Edit::LettersMode newLettersMode);
		void SetPasswordMode(bool passwordMode);
		void SetNumberOnlyMode(bool numberOnlyMode);
		void SetTextLengthLimit(unsigned int newLimit);
		std::wstring GetText();

		// -- property fields -- //
	public:
		const bool& PasswordMode;
		const bool& NumberOnlyMode;
		const TextAlignment& Alignment;
		const LettersMode& LetterMode;
	};
	/*In progress*//*class RadioButton : public WindowControl
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
	};*/
	/*In progress*//*class GroupBox : public WindowControl
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
	};*/
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
		struct EventHandler : public WindowControl::EventHandler<ProgressBar::Event>
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
		WindowControl::Events<ProgressBar::Event> events;


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
			events.PushEvent(ProgressBar::Event((ProgressBar::Event::Type)event.type));
		}
	public:
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
		WindowControl::Events<ProgressBar::Event>& Events;
	};
	/*In progress*/class GraphicsBox : public WindowControl
	{
		// -- fields -- //
	private:
		struct Graphics
		{
			// -- fields -- //
		private:
			GraphicsBox *control;
			unsigned int width = 0u, height = 0u;

			struct PixelMap
			{
			private:
				unsigned int width, height;
				unsigned int *pixels = nullptr;

			public:
				PixelMap(unsigned int width, unsigned int height)
				{
					this->width = width;
					this->height = height;

					pixels = new unsigned int[width * height];

					for (unsigned int i = 0; i < width * height; i++)
						pixels[i] = 0xFF000000;

				}
				~PixelMap()
				{
					if (pixels)
					{
						delete[] pixels;
					}
				}

				void Resize(unsigned int newWidth, unsigned int newHeight)
				{
					this->width = newWidth;
					this->height = newHeight;

					if (pixels) delete[] pixels;
					pixels = new unsigned int[width * height];

					unsigned int range = width * height;
					for (unsigned int i = 0; i < range; i++)
						pixels[i] = 0xFF000000;
				}
				unsigned int* GetFirstAddress()
				{
					return pixels;
				}
				void SetPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
				{
					unsigned int *pixel = (pixels + y * width + x);
					*pixel = 0xFF000000 | (r << 16) | (g << 8) | b;
				}
			};
			PixelMap *pixelMap;

			ID2D1Factory *D2DFactory = nullptr;
			ID2D1HwndRenderTarget *RT = nullptr;
			ID2D1BitmapRenderTarget *BRT = nullptr;
			ID2D1Bitmap *bitmap = nullptr, *compBitmap = nullptr;
			ID2D1SolidColorBrush *brush = nullptr;
			ID2D1BitmapBrush *bitmapBrush = nullptr;

			// -- contructor -- //
		private:
			Graphics(GraphicsBox *control)
			{
				this->control = control;
			}
			~Graphics()
			{
				SafeRelease(&bitmap);
				SafeRelease(&RT);
				SafeRelease(&D2DFactory);

				if (pixelMap) delete pixelMap;
			}


			// -- methods -- //
		private:
			void InitGraphics()
			{
				width = control->rect.width;
				height = control->rect.height;
				pixelMap = new PixelMap(width, height);

				// create D2D1Factory
				D2D1CreateFactory
				(
					D2D1_FACTORY_TYPE_SINGLE_THREADED,
					&D2DFactory
				);

				// create render target
				D2DFactory->CreateHwndRenderTarget
				(
					D2D1::RenderTargetProperties
					(
						D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT,
						D2D1::PixelFormat
						(
							DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
							D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_IGNORE
						),
						96.0f, 96.0f,
						D2D1_RENDER_TARGET_USAGE::D2D1_RENDER_TARGET_USAGE_NONE,
						D2D1_FEATURE_LEVEL::D2D1_FEATURE_LEVEL_DEFAULT
					),
					D2D1::HwndRenderTargetProperties
					(
						control->hControl,
						D2D1::SizeU(width, height)
					),
					&RT
				);

				// create bitmap for render
				RT->CreateBitmap
				(
					D2D1::SizeU(width, height),
					D2D1::BitmapProperties
					(
						D2D1::PixelFormat
						(
							DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
							D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_IGNORE
						)
					),
					&bitmap
				);

				// ------------------
				RT->CreateCompatibleRenderTarget(&BRT);
				BRT->CreateSolidColorBrush
				(
					D2D1::ColorF(0x00FF88),
					&brush
				);
			}
			template <class T> void SafeRelease(T **ppT)
			{
				if (*ppT)
				{
					(*ppT)->Release();
					*ppT = NULL;
				}
			}
			void Resize(unsigned int newWidth, unsigned int newHeight)
			{
				width = newWidth;
				height = newHeight;

				pixelMap->Resize(width, height);
				RT->Resize(D2D1::SizeU(width, height));

				SafeRelease(&bitmap);
				RT->CreateBitmap
				(
					D2D1::SizeU(width, height),
					D2D1::BitmapProperties
					(
						D2D1::PixelFormat
						(
							DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
							D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_IGNORE
						)
					),
					&bitmap
				);
			}
		public:
			void Render()
			{
				// copy data from pixelMap to bitmap
				bitmap->CopyFromMemory
				(
					&D2D1::RectU(0, 0, width, height),
					pixelMap->GetFirstAddress(),
					width * 4
				);

				// render bitmap from pixelMap
				RT->BeginDraw();
				RT->DrawBitmap
				(
					bitmap,
					&D2D1::RectF(0.0f, 0.0f, (float)width, (float)height),
					1.0f,
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					&D2D1::RectF(0.0f, 0.0f, (float)width, (float)height)
				);

				// render bitmap from BitmapRenderTarget
				BRT->GetBitmap(&compBitmap);
				RT->DrawBitmap
				(
					compBitmap,
					&D2D1::RectF(0.0f, 0.0f, (float)width, (float)height),
					0.2f,
					D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					&D2D1::RectF(0.0f, 0.0f, (float)width, (float)height)
				);
				RT->EndDraw();
			}
			void SetPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
			{
				pixelMap->SetPixel(x, y, r, g, b);
			}
			void Clear(unsigned char r, unsigned char g, unsigned char b)
			{
				// pixelMap
				unsigned int *ptr = pixelMap->GetFirstAddress();
				unsigned int range = width * height;
				for (unsigned int i = 0; i < range; i++)
				{
					*ptr = (0xFF000000 | (r << 16) | (g << 8) | b);
					++ptr;
				}

				// clear BRT bitmap
				BRT->BeginDraw();
				BRT->Clear(D2D1::ColorF((r << 16) | (g << 8) | b));
				BRT->EndDraw();
			}
			void DrawLine(float x1, float y1, float x2, float y2)
			{
				BRT->BeginDraw();
				BRT->Clear();
				BRT->DrawLine
				(
					D2D1::Point2F(x1, y1),
					D2D1::Point2F(x2, y2),
					brush,
					4.0f
				);
				BRT->EndDraw();
			}


			// -- friends -- //
		public:
			friend class GraphicsBox;
		};
	public:
		struct Config : WindowControl::Config
		{

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
			};
			Type type;

			Event()
			{
				type = Invalid;
			}
			Event(GraphicsBox::Event::Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler<GraphicsBox::Event>
		{
			virtual void HandleEvent(GraphicsBox::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
					// cases
				}
			}
			// virtual functions
		};
	private:
		WindowControl::Events<GraphicsBox::Event> events;
		Graphics graphics;


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
			// no GraphicsBox events yet
		}
	public:
		void Resize(unsigned int newWidth, unsigned int newHeight);


		// -- propetry fields -- //
	public:
		Graphics& Gfx;
	};
}

#endif // !WIN_API_WINDOW_CONTROLS_H