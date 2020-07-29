#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include "winapi_framework.h"

#include <math.h>
#include <time.h>
#include <sstream>

namespace WAF = WinapiFramework;

void MainWindow_OnActivatedGlobal(WAF::Window::Event<WAF::Window::EventTypeActivated>& event);

class MainForm
{
public:
	WAF::Window *MainWindow = nullptr;

	WAF::Button* button1 = nullptr;
	WAF::Button* button2 = nullptr;

	std::vector<WAF::Button*> panelButtons;
	WAF::Panel* panel = nullptr;


public:
	MainForm()
	{
		WAF::Framework::Keyboard.KeyEvents.AddEventHandler<MainForm>(this, &MainForm::FrameworkKeyboardEventHandler);
		WAF::Framework::Mouse.Events.AddEventHandler<MainForm>(this, &MainForm::FrameworkMouse_EH);


		// MainWindow
		MainWindow = WAF::Framework::CreateNewWindow(
			WAF::ConStruct<WAF::Window>(
				L"WinApiFramework test",
				WAF::Rect(50, 50, 1000, 600),
				WAF::Window::Position::Center,
				WAF::Window::StartStyle::Normal,
				WAF::SizeRect(200u, 100u, 2000u, 1000u),
				WAF::Size(800, 400)));
		MainWindow->AddEventHandler<WAF::Window::EventTypeActivated>(&MainForm::MainWindow_OnActivated, this);
		MainWindow->AddEventHandler<WAF::Window::EventTypeActivated>(MainWindow_OnActivatedGlobal);
		//MainWindow->AddEventHandler<WAF::Window::EventTypeActivated>(&MainForm::MainWindow_OnActivated2, this);
		MainWindow->AddEventHandler<WAF::Window::EventTypeDeactivated>(&MainForm::MainWindow_OnDeactivated, this);

		MainWindow->AddEventHandler<WAF::Window::EventTypeResized>(&MainForm::MainWindow_OnResized, this);
		MainWindow->AddEventHandler<WAF::Window::EventTypeMoved>(&MainForm::MainWindow_OnMoved, this);

		MainWindow->AddEventHandler<WAF::Window::EventTypeCaptionChanged>(&MainForm::MainWindow_OnCaptionChanged, this);

		MainWindow->AddEventHandler<WAF::Window::EventTypeClose>(&MainForm::MainWindow_OnClose, this);



		// button1
		button1 = MainWindow->CreateControl<WAF::Button>(WAF::ConStruct<WAF::Button>(
			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(WAF::Point(20, 20), { 100, 50 })),
			L"Button1",
			WAF::Button::CaptionPosition::Center));
		button1->Events.AddEventHandler<MainForm>(this, &MainForm::Button1_EH);

		// button2
		button2 = MainWindow->CreateControl<WAF::Button>(WAF::ConStruct<WAF::Button>(
			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(WAF::Point(700, 350), { 100, 50 })),
			L"button2"));
		button2->Events.AddEventHandler<MainForm>(this, &MainForm::Button2_EH);


		// panel
		panel = MainWindow->CreateControl<WAF::Panel>(WAF::ConStruct<WAF::Panel>(
			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(150, 10, 400, 200))));
		panel->Events.AddEventHandler<MainForm>(this, &MainForm::Panel_EH);

		// panel buttons
		int buttonWidth = 100;
		int buttonHeight = 50;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				WAF::Button* button = panel->CreateControl<WAF::Button>(WAF::ConStruct<WAF::Button>(
					WAF::ConStruct<WAF::ChildControl>(WAF::Rect(x * buttonWidth, y * buttonHeight, buttonWidth, buttonHeight)),
					L"button" + std::to_wstring(x * 2 + y)));
				panelButtons.push_back(button);
			}
		}
	}
	~MainForm()
	{
		if (MainWindow) MainWindow->Destroy();
	}


	// -- MainForm::event_handlers -- //
	void MainWindow_OnActivated(WAF::Window::Event<WAF::Window::EventTypeActivated>& event)
	{
		event.GetWindow()->SetCaption(L"Window Activated!!!");
	}
	void MainWindow_OnActivated2(WAF::Window::Event<WAF::Window::EventTypeActivated>& event)
	{
		event.GetWindow()->SetCaption(event.GetWindow()->GetCaption() + L" + second function");
	}
	void MainWindow_OnDeactivated(WAF::Window::Event<WAF::Window::EventTypeDeactivated>& event)
	{
		event.GetWindow()->SetCaption(L"Window Deactivated!!!");
	}
	void MainWindow_OnResized(WAF::Window::Event<WAF::Window::EventTypeResized>& event)
	{
		DisplayMainWindowProps();
	}
	void MainWindow_OnMoved(WAF::Window::Event<WAF::Window::EventTypeMoved>& event)
	{
		DisplayMainWindowProps();
		event.GetWindow()->RemoveEventHandler<WAF::Window::EventTypeActivated>(&MainWindow_OnActivatedGlobal);
	}
	void MainWindow_OnCaptionChanged(WAF::Window::Event<WAF::Window::EventTypeCaptionChanged>& event)
	{
		event.GetWindow()->Show();
	}
	void MainWindow_OnClose(WAF::Window::Event<WAF::Window::EventTypeClose>& event)
	{
		WAF::MessBoxButtonPressed mbp = event.GetWindow()->ShowMessageBox(
			L"Exit application", 
			L"Are you sure you want to exit the application right now?",
			WAF::MessBoxButtonLayout::YesNo,
			WAF::MessBoxIcon::IconWarning);

		if (mbp == WAF::MessBoxButtonPressed::ButtonNo)
			event.AbortClosing();
		else
		{
			event.GetWindow()->Destroy();
			MainWindow = nullptr;
		}
	}


	void Button1_EH(WAF::Button::Event event)
	{
		switch (event.type)
		{
			case WAF::Button::Event::Type::Click:
				event.button->SetCaption(L"button1 clicked!");
				//event.button->SetCaptionPosition((WAF::Button::CaptionPosition)(event.button->GetCaptionPosition() + 1));
				break;
			case WAF::Button::Event::Type::DoubleClick:
				event.button->SetCaption(L"button1 double clicked!");
				break;
		}
	}
	void Button2_EH(WAF::Button::Event event)
	{
		switch (event.type)
		{
			case WAF::Button::Event::Type::Click:
				event.button->SetCaption(L"button2 clicked!");
				break;
			case WAF::Button::Event::Type::DoubleClick:
				event.button->SetCaption(L"button2 double clicked!");
				break;
		}
	}
	void Button3_EH(WAF::Button::Event event)
	{
		switch (event.type)
		{
			case WAF::Button::Event::Type::Click:
				event.button->SetCaption(L"button3 clicked!");
				break;
			case WAF::Button::Event::Type::DoubleClick:
				event.button->SetCaption(L"button3 double clicked!");
				break;
		}
	}
	void Panel_EH(WAF::Panel::Event event)
	{
		switch (event.type)
		{
			case WAF::Panel::Event::Type::Resize:
				//button3->Resize(panel->Rectangle.size.width / 2, panel->Rectangle.size.height / 2);
				break;
		}
	}
	
	// FrameworkKeyboard
	void FrameworkKeyboardEventHandler(WAF::Keyboard::KeyEvent event)
	{
		switch (event.type)
		{
			case WAF::Keyboard::KeyEvent::Type::Press:
				if (event.key == WAF::Keyboard::Key::Esc)
				{
					WAF::Framework::Exit(0);
				}
		}
	}
	void FrameworkMouse_EH(WAF::Mouse::Event event)
	{
		switch (event.type)
		{
			case WAF::Mouse::Event::Type::RightPress:
				//panel->Move(MainWindow->GetCanvasMousePosition());
				break;
			case WAF::Mouse::Event::Type::Move:
			{
				/*MainWindow->SetCaption(
					L"WindowM: " +
					std::to_wstring(MainWindow->GetWindowMousePosition().x) +
					L" : " +
					std::to_wstring(MainWindow->GetWindowMousePosition().y) +
					L" ClientM: " +
					std::to_wstring(MainWindow->GetClientMousePosition().x) +
					L" : " +
					std::to_wstring(MainWindow->GetClientMousePosition().y) +
					L" CanvasM: " +
					std::to_wstring(MainWindow->GetCanvasMousePosition().x) +
					L" : " +
					std::to_wstring(MainWindow->GetCanvasMousePosition().y));*/

				//panel->Move(MainWindow->GetCanvasMousePosition() - WAF::Point(panel->GetRect().size.width / 2, panel->GetRect().size.height));
				break;
			}
		}
	}

	// -- MainForm::methods -- //
	void DisplayMainWindowProps()
	{
		MainWindow->SetCaption(
			L"Position: [" +
			std::to_wstring(MainWindow->WindowRect.position.x) + L":" + std::to_wstring(MainWindow->WindowRect.position.y) +
			L"] Resolution: [" +
			std::to_wstring(MainWindow->WindowRect.size.width) + L":" + std::to_wstring(MainWindow->WindowRect.size.height) +
			L"]");
	}
};
MainForm *MF;


void MainWindow_OnActivatedGlobal(WAF::Window::Event<WAF::Window::EventTypeActivated>& event)
{
	event.GetWindow()->SetCaption(L"Winodw Activated from global function.");
}

void CallBackFunction()
{
	Sleep(1);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR args, INT ncmd)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	MF = new MainForm();

	WAF::Framework::SetCallBackFunction(CallBackFunction);
	WAF::Framework::ProcessMessages();

	delete MF;

	return 0;
}



//#define _CRTDBG_MAP_ALLOC  
//#include <stdlib.h>  
//#include <crtdbg.h>  
//
//#include "WinApiFramework.h"
//
//#include <math.h>
//#include <time.h>
//#include <sstream>
//
//namespace WAF = WinapiFramework;
//
//Graphics::Bitmap *texture1 = nullptr, *texture2 = nullptr;
//void GenerateTexture(Graphics::Bitmap*& tex, Graphics::Color color)
//{
//	if (tex) delete tex;
//	tex = new Graphics::Bitmap(300, 200);
//	for (int x = 0; x < tex->Width; ++x)
//	{
//		for (int y = 0; y < tex->Height; ++y)
//		{
//			if ((x % 2 == 0) ^ (y % 2 == 0))
//			{
//				tex->SetPixel(x, y, color);
//			}
//			else
//			{
//				tex->SetPixel(x, y, Graphics::Color(color.GetR(), color.GetG(), color.GetB(), 0x00));
//			}
//			//unsigned char value = ((x % 2 == 0) ^ (y % 2 == 0)) ? 255 : 0;
//			////texture->SetPixel(x, y, Graphics::Color(value, value, value, 0xFF));
//			//tex->SetPixel(x, y, Graphics::Color(color.GetR(), color.GetG(), color.GetB(), value));
//		}
//	}
//}
//void DestroyTextures()
//{
//	if (texture1) delete texture1;
//	if (texture2) delete texture2;
//}
//
//class MainForm
//{
//	// -- MainForm::fields -- //
//public:
//	WAF::Window *MainWindow = nullptr;
//
//	WAF::Label* eventHistoryLabel = nullptr;
//
//	WAF::CheckBox* checkBox1 = nullptr;
//	WAF::Edit* edit1 = nullptr;
//	WAF::Label* label1 = nullptr;
//	WAF::ProgressBar* progressBar = nullptr;
//	WAF::TrackBar* trackBar = nullptr;
//	WAF::GraphicsBox* gfxBox = nullptr;
//	WAF::GroupBox* groupBox = nullptr;
//
//	WAF::Button* button1 = nullptr;
//	WAF::Button* button2 = nullptr;
//
//	WAF::Panel* panel = nullptr;
//
//	std::vector<std::wstring> eventsHistory;
//
//
//	// -- MainForm::constructors -- //
//public:
//	MainForm()
//	{
//		WAF::Framework::Keyboard.KeyEvents.AddEventHandler<MainForm>(this, &MainForm::FrameworkKeyboardEventHandler);
//		WAF::Framework::Mouse.Events.AddEventHandler<MainForm>(this, &MainForm::FrameworkMouse_EH);
//
//
//		// MainWindow
//		MainWindow = WAF::Framework::CreateNewWindow(
//			WAF::ConStruct<WAF::Window>(
//				L"WinApiFramework test",
//				WAF::Rect(50, 50, 1200, 700),
//				WAF::Window::Position::Center,
//				WAF::Window::StartStyle::Normal,
//				WAF::SizeRect(200u, 100u, 2000u, 1000u),
//				WAF::Size(1100, 600)));
//		MainWindow->Events.AddEventHandler<MainForm>(this, &MainForm::MainWindow_EH);
//
//
//		// graphicsBox
//		WAF::ConStruct<WAF::GraphicsBox> gbc;
//		//gbc.rect = WAF::Rect(10, 10, MainWindow->ClientRect.size.width - 20, MainWindow->ClientHeight - 20);
//		gbc.rect = WAF::Rect(10, 10, 600, 400);
//		gbc.graphics.renderType = WAF::GraphicsBox::RenderType::RenderTypeDefault;
//		gbc.graphics.presentOption = WAF::GraphicsBox::PresentOption::PresentOptionWaitForDisplay;
//		gbc.graphics.defaultTextFormatDesc = WAF::GraphicsBox::TextFormatDescription(
//			L"consolas",
//			15.0f,
//			WAF::GraphicsBox::FontWeight::FontWeightExtraBold,
//			WAF::GraphicsBox::FontStyle::FontStyleOblique,
//			WAF::GraphicsBox::FontStretch::FontStretchUltraExpanded);
//		gfxBox = MainWindow->CreateControl<WAF::GraphicsBox>(gbc);
//
//		// eventHistoryLabel
//		eventHistoryLabel = MainWindow->CreateControl<WAF::Label>(WAF::ConStruct<WAF::Label>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(620, 10, 100, 400)),
//			L"events"));
//
//
//		// checkBox1
//		checkBox1 = MainWindow->CreateControl<WAF::CheckBox>(WAF::ConStruct<WAF::CheckBox>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(750, 10, 100, 50)),
//			L"check1",
//			true,
//			WAF::CheckBox::BoxState::MiddleState));
//		checkBox1->Events.AddEventHandler<MainForm>(this, &MainForm::CheckBox1_EH);
//
//		// label1
//		label1 = MainWindow->CreateControl<WAF::Label>(WAF::ConStruct<WAF::Label>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(850, 10, 100, 50)),
//			L"A box with a frame drawn with the same color as the window background. This color is whilte in the background.",
//			WAF::Label::TextAlignment::Right));
//		label1->Events.AddEventHandler<MainForm>(this, &MainForm::Label1_EH);
//
//		// progressBar
//		progressBar = MainWindow->CreateControl<WAF::ProgressBar>(WAF::ConStruct<WAF::ProgressBar>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(750, 100, 150, 20)),
//			WAF::Range(0, 100),
//			20,
//			WAF::ProgressBar::BarState::Normal,
//			WAF::ProgressBar::BarOrientation::Horizontal,
//			WAF::ProgressBar::BarDisplayStyle::Default, 1u));
//
//		// trackBar
//		trackBar = MainWindow->CreateControl<WAF::TrackBar>(WAF::ConStruct<WAF::TrackBar>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(750, 140, 150, 40)),
//			WAF::Range(0, 20),
//			20,
//			1,
//			10,
//			WAF::TrackBar::Horizontal,
//			WAF::TrackBar::TickStyle::Both,
//			true,
//			WAF::Range(5, 12),
//			WAF::TrackBar::ToolTipsStyle::ToolTipsStyleTop));
//		trackBar->Events.AddEventHandler<MainForm>(this, &MainForm::TrackBar_EH);
//		trackBar->SetTicksFrequency(2u);
//
//		// edit1
//		edit1 = MainWindow->CreateControl<WAF::Edit>(WAF::ConStruct<WAF::Edit>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(750, 200, 250, 150)),
//			L"",
//			L"some cue text",
//			WAF::Edit::TextAlignment::Left,
//			WAF::Edit::LettersMode::All,
//			false, false, false, true, true,
//			10000u, L'\x25CF', WAF::Edit::ScrollingStyle::Vertical));
//		edit1->Events.AddEventHandler<MainForm>(this, &MainForm::Edit1_EH);
//
//		/*
//		WAF::GraphicsBox::ConStruct gbc;
////		gbc.rect = WAF::Rect(10, 10, MainWindow->ClientWidth - 20, MainWindow->ClientHeight - 20);
////		gbc.graphics.renderType = WAF::GraphicsBox::RenderType::RenderTypeDefault;
////		gbc.graphics.presentOption = WAF::GraphicsBox::PresentOption::PresentOptionWaitForDisplay;
////		gfxBox = new WAF::GraphicsBox(gbc);
////		gfxBox->Events.AddEventHandler<MainForm>(this, &MainForm::gfxBoxEventsReceiver);
////		MainWindow->AddControl(gfxBox);
//		*/
//
//
//		// groupBox
//		groupBox = MainWindow->CreateControl<WAF::GroupBox>(WAF::ConStruct<WAF::GroupBox>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(10, 420, 350, 150))));
//
//		// button1
//		button1 = MainWindow->CreateControl<WAF::Button>(WAF::ConStruct<WAF::Button>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(groupBox->Rectangle.position + WAF::Point(20, 20), { 100, 40 })),
//			L"Button1",
//			WAF::Button::CaptionPosition::Center));
//		button1->Events.AddEventHandler<MainForm>(this, &MainForm::Button1_EH);
//
//		// button2
//		button2 = MainWindow->CreateControl<WAF::Button>(WAF::ConStruct<WAF::Button>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(groupBox->Rectangle.position + WAF::Point(20, 80), { 100, 40 })),
//			L"button2"));
//		button2->Events.AddEventHandler<MainForm>(this, &MainForm::Button2_EH);
//
//
//		// panel
//		panel = MainWindow->CreateControl<WAF::Panel>(WAF::ConStruct<WAF::Panel>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(620, 420, 400, 200))));
//	}
//	~MainForm()
//	{
//		if (MainWindow) MainWindow->Destroy();
//	}
//
//	void DisplayEventHistory()
//	{
//		while (eventsHistory.size() > 20)
//		{
//			eventsHistory.erase(eventsHistory.begin());
//		}
//
//		std::wstring events = L"";
//		for (size_t i = 0; i < eventsHistory.size(); i++)
//		{
//			events += eventsHistory[i] + L"\n";
//		}
//		eventHistoryLabel->SetCaption(events);
//	}
//
//	// -- MainForm::event_handlers -- //
//	// MainWindow events handler
//	void MainWindow_EH(WAF::Window::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::Window::Event::Type::Move:
//			{
//				DisplayMainWindowProps();
//				break;
//			}
//			case WAF::Window::Event::Type::Resize:
//			{
//				// display new MainWindow properties
//				DisplayMainWindowProps();
//				break;
//			}
//		}
//	}
//	
//	void Button1_EH(WAF::Button::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::Button::Event::Type::Click:
//				event.button->SetCaption(L"button1 clicked!");
//				edit1->SelectAll();
//				//event.button->SetCaptionPosition((WAF::Button::CaptionPosition)(event.button->GetCaptionPosition() + 1));
//				break;
//			case WAF::Button::Event::Type::DoubleClick:
//				event.button->SetCaption(L"button1 double clicked!");
//				break;
//		}
//	}
//	void Button2_EH(WAF::Button::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::Button::Event::Type::Click:
//				event.button->SetCaption(L"button2 clicked!");
//				break;
//			case WAF::Button::Event::Type::DoubleClick:
//				event.button->SetCaption(L"button2 double clicked!");
//				break;
//		}
//	}
//
//	void CheckBox1_EH(WAF::CheckBox::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::CheckBox::Event::Type::Check:
//				if (checkBox1->State == WAF::CheckBox::BoxState::Check)
//					checkBox1->SetCaption(L"check box checked!");
//				break;
//			case WAF::CheckBox::Event::Type::MiddleState:
//				checkBox1->SetCaption(L"check box in middle state!");
//				break;
//			case WAF::CheckBox::Event::Type::UnCheck:
//				checkBox1->SetCaption(L"check box unchecked!");
//				break;
//		}
//	}
//	void Edit1_EH(WAF::Edit::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::Edit::Event::Type::TextChanged:
//			{
//				label1->SetCaption(L"Texting!");
//				//label1->SetCaption(std::to_wstring(edit1->GetLineCount()));
//			}
//				break;
//			case WAF::Edit::Event::Type::TextLimitReached:
//				label1->SetCaption(L"Limit reached!");
//				break;
//			case WAF::Edit::Event::Type::FocusSet:
//				label1->SetCaption(L"FocusSet");
//				break;
//			case WAF::Edit::Event::Type::FocusKilled:
//				label1->SetCaption(L"FocusKilled");
//				break;
//			case WAF::Edit::Event::Type::PasswordModeSet:
//				label1->SetCaption(L"PasswordModeSet");
//				break;
//			case WAF::Edit::Event::Type::NumberModeSet:
//				label1->SetCaption(L"NumberNodeSet");
//				break;
//		}
//	}
//	void Label1_EH(WAF::Label::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::Label::Event::Type::Clicked:
//				label1->SetCaption(L"Label Clicked!");
//				break;
//			case WAF::Label::Event::Type::DoubleClicked:
//				label1->SetCaption(L"Label double clicked!");
//				break;
//		}
//	}
//	void TrackBar_EH(WAF::TrackBar::Event ev)
//	{
//		switch (ev.type)
//		{
//			case WAF::TrackBar::Event::Type::LinedUp:
//				eventsHistory.push_back(L"LinedUp!");
//				break;
//			case WAF::TrackBar::Event::Type::LinedDown:
//				eventsHistory.push_back(L"LinedDown!");
//				break;
//			case WAF::TrackBar::Event::Type::PagedUp:
//				eventsHistory.push_back(L"PagedUp!");
//				break;
//			case WAF::TrackBar::Event::Type::PagedDown:
//				eventsHistory.push_back(L"PagedDown!");
//				break;
//			case WAF::TrackBar::Event::Type::MovedToTop:
//				eventsHistory.push_back(L"MovedToTop!");
//				break;
//			case WAF::TrackBar::Event::Type::MovedToBottom:
//				eventsHistory.push_back(L"MovedToBottom!");
//				break;
//			case WAF::TrackBar::Event::Type::DraggingFinnished:
//				eventsHistory.push_back(L"DraggingFinnished!");
//				break;
//			case WAF::TrackBar::Event::Type::ThumbDragged:
//				eventsHistory.push_back(L"ThumbDragged!");
//				break;
//			case WAF::TrackBar::Event::Type::ThumbPositionChanged:
//				eventsHistory.push_back(L"PositionChanged!");
//				break;
//		}
//		DisplayEventHistory();
//	}
//
//	// FrameworkKeyboard
//	void FrameworkKeyboardEventHandler(WAF::Keyboard::KeyEvent event)
//	{
//		switch (event.type)
//		{
//			case WAF::Keyboard::KeyEvent::Type::Press:
//				if (event.key == WAF::Keyboard::Key::Esc)
//				{
//					WAF::Framework::Exit(0);
//				}
//				if (event.key == WAF::Keyboard::Key::Digit9)
//				{
//					//trackBar->SetThumbPosition(trackBar->GetPosition() - 5);
//					if (trackBar) trackBar->SetMinTrackValue(trackBar->GetMinTrackValue() - 5);
//				}
//				if (event.key == WAF::Keyboard::Key::Digit0)
//				{
//					//trackBar->SetThumbPosition(trackBar->GetPosition() + 5);
//					if (trackBar) trackBar->SetMaxSelectValue(trackBar->GetMaxSelectValue() + 5);
//				}
//				if (event.key == WAF::Keyboard::Key::F1)
//				{
//					if (button1)
//					{
//						button1->SetCaptionPosition((WAF::Button::CaptionPosition)((button1->GetCaptionPosition() + 1) % 10));
//					}
//					if (groupBox)
//					{
//						groupBox->SetCaption(L"different caption");
//						groupBox->SetCaptionPosition((WAF::GroupBox::CaptionPosition)((groupBox->GetCaptionPosition() + 1) % 3));
//					}
//					if (trackBar)
//					{
//						trackBar->Destroy();
//						trackBar = nullptr;
//					}
//				}
//				switch (event.key)
//				{
//					case WAF::Keyboard::Digit1: edit1->SetPasswordMode(!edit1->GetPasswordMode());  break;
//					case WAF::Keyboard::Digit2: edit1->SetNumberOnlyMode(!edit1->GetNumberOnlyMode());	break;
//					case WAF::Keyboard::Digit3: edit1->SetReadOnlyMode(!edit1->GetReadOnlyMode());	break;
//					case WAF::Keyboard::Digit4: 
//						edit1->SetTextAlignment(WAF::Edit::TextAlignment((edit1->GetTextAlignment() + 1) % 3));	break;
//					case WAF::Keyboard::Digit5:
//						edit1->SetLettersMode(WAF::Edit::LettersMode((edit1->GetLettersMode() + 1) % 3));	break;
//					case WAF::Keyboard::Digit6:	edit1->SetPasswordChar(L'A');	break;
//					case WAF::Keyboard::Digit7:	edit1->SetPasswordChar(L'B');	break;
//				}
//
//				switch (event.key)
//				{
//					case WAF::Keyboard::Key::OpeningSquareBracket: edit1->SetSelection(10, 20);	break;
//					case WAF::Keyboard::Key::ClosingSquareBracket: edit1->SelectAll();	break;
//					case WAF::Keyboard::Key::BackSlash: edit1->RemoveSelection();	break;
//				}
//				break;
//		}
//	}
//	void FrameworkMouse_EH(WAF::Mouse::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::Mouse::Event::Type::RightPress:
//				//groupBox->Move(MainWindow->GetCanvasMousePosition());
//				edit1->SetRect(WAF::Rect(750, 200, edit1->GetMousePosition().x, edit1->GetMousePosition().y));
//				edit1->SetSelection(10, 20);
//				break;
//			case WAF::Mouse::Event::Type::Move:
//			{
//				MainWindow->SetCaption(
//					L"WindowM: " +
//					std::to_wstring(MainWindow->GetWindowMousePosition().x) +
//					L" : " +
//					std::to_wstring(MainWindow->GetWindowMousePosition().y) +
//					L" ClientM: " +
//					std::to_wstring(MainWindow->GetClientMousePosition().x) +
//					L" : " +
//					std::to_wstring(MainWindow->GetClientMousePosition().y) +
//					L" CanvasM: " +
//					std::to_wstring(MainWindow->GetCanvasMousePosition().x) +
//					L" : " +
//					std::to_wstring(MainWindow->GetCanvasMousePosition().y));
//
//				//progressBar->SetPosition(MainWindow->GetClientMousePosition().x / float(MainWindow->ClientRect.size.width) * 100.0f);
//				//progressBar->StepIt();
//
//				std::wostringstream os;
//				//os << L"charIndex: " << edit1->GetCharIndexFromPosition(edit1->GetMousePosition())
//				//	<< L"\nLine Index: " << edit1->GetLineIndexFromPosition(edit1->GetMousePosition());
//				//label1->SetCaption(os.str());
//
//				if (edit1->IsMouseInEditableRect())
//				{
//					//os << L"Line length: " << edit1->GetLineLength(edit1->GetLineIndexFromPosition(edit1->GetMousePosition()));
//					os << edit1->GetCharIndexFromPosition(edit1->GetMousePosition());
//					label1->SetCaption(os.str());
//				}
//				else
//				{
//					os << L"Mouse outsize!";
//					label1->SetCaption(os.str());
//				}
//				break;
//			}
//		}
//	}
//
//	// -- MainForm::methods -- //
//	void DisplayMainWindowProps()
//	{
//		MainWindow->SetCaption(
//			L"Position: [" +
//			std::to_wstring(MainWindow->WindowRect.position.x) + L":" + std::to_wstring(MainWindow->WindowRect.position.y) +
//			L"] Resolution: [" +
//			std::to_wstring(MainWindow->WindowRect.size.width) + L":" + std::to_wstring(MainWindow->WindowRect.size.height) +
//			L"]");
//	}
//};
//MainForm *MF;
//
//void CallBackFunction()
//{
//	Sleep(1);
//
//	float mouseX = MF->gfxBox->GetMousePosition().x;
//	float mouseY = MF->gfxBox->GetMousePosition().y;
//
//	// [>] Begin Drawing
//	MF->gfxBox->Gfx.BeginDraw();
//	/*MF->gfxBox->Gfx.Clear(Graphics::Color(0xA0, 0xA0, 0xA0, 0xFF));*/
//	MF->gfxBox->Gfx.Clear(Graphics::Color::White);
//	MF->gfxBox->Gfx.SetSolidBrush(Graphics::Color(0xFF, 0x00, 0x00));
//
//	if (WAF::Framework::Mouse.LeftPressed) MF->gfxBox->Gfx.DrawLine(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(200.0f, 200.0f), 5.0f);
//
//	if (WAF::Framework::Mouse.RightPressed) MF->gfxBox->Gfx.DrawBitmap(
//		*texture1,
//		Graphics::Rect<float>(mouseX, mouseY, mouseX + 300.0f, mouseY + 200.0f),
//		Graphics::Rect<float>(0.0f, 0.0f, texture1->Width, texture1->Height),
//		1.0f, WAF::GraphicsBox::InterpolationMode::InterpolationModeNearestNeighbor);
//
//	if (WAF::Framework::Mouse.RightPressed) MF->gfxBox->Gfx.DrawBitmap(
//		*texture2,
//		Graphics::Rect<float>(100.0f, 100.0f, 400.0f, 300.0f),
//		Graphics::Rect<float>(0.0f, 0.0f, texture2->Width, texture2->Height),
//		1.0f, WAF::GraphicsBox::InterpolationMode::InterpolationModeNearestNeighbor);
//
//	//MF->gfxBox->Gfx.FillEllipse(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(100.0f, 50.0f));
//	MF->gfxBox->Gfx.DrawRoundedRectangle(Graphics::Point<float>(0.0f, 0.0f), Graphics::Point<float>(mouseX, mouseY), 50.0f, 50.0f, mouseX / 100.0f);
//
//	std::wostringstream os;
//	os << L"Lorem ipsum dolor sit\t amet, consectetur adipiscing elit. Ut facilisis"
//		L"risus in neque ullamcorper finibus. Proin eu lectus dignissim enim porttitor"
//		L"ultricies vitae vel mauris. Vestibulum eleifend porta enim at vulputate. Nunc"
//		L"imperdiet quam vel eros mollis pellentesque sit amet eu leo. Sed at nisl vitae"
//		L"arcu maximus lobortis. Suspendisse sodales urna urna, eu venenatis enim faucibus et. "
//		L"Praesent gravida metus vel ipsum ullamcorper gravida. Ut pretium ex vitae cursus posuere."
//		L"Maecenas justo urna, iaculis ac pulvinar eleifend, vestibulum vel neque. Ut bibendum, "
//		L"sapien in pretium rhoncus, leo eros ullamcorper urna, quis vehicula leo neque sollicitudin "
//		L"nibh. Etiam convallis leo vitae nibh mattis, sit amet pretium nibh efficitur. Phasellus "
//		L"maximus, mauris eu finibus suscipit, ligula dui ultricies libero, at efficitur sem ligula sed sapien.";
//
//	MF->gfxBox->Gfx.SetSolidBrush(Graphics::Color(0x00, 0x00, 0x00), 1.0f);
//	MF->gfxBox->Gfx.DrawString(os.str(), Graphics::Rect<float>(10.0f, 10.0f, mouseX - 10.0f, mouseY - 10.0f));
//
//	if (WAF::Framework::Mouse.LeftPressed) MF->gfxBox->Gfx.FillEllipse(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(20.0f, 20.0f));
//	MF->gfxBox->Gfx.EndDraw();
//	//MF->gfxBox->Gfx.Render();
//}
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR args, INT ncmd)
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
//
//	GenerateTexture(texture1, Graphics::Color(0xFF, 0x00, 0x00, 0xFF));
//	GenerateTexture(texture2, Graphics::Color(0x00, 0xFF, 0x00, 0xFF));
//	MF = new MainForm();
//
//	WAF::Framework::SetCallBackFunction(CallBackFunction);
//	WAF::Framework::ProcessMessages();
//
//	delete MF;
//	DestroyTextures();
//
//	return 0;
//}


//#include "WinApiFramework.h"
//
//#include <sstream>
//
//namespace WAF = WinapiFramework;
//
//Graphics::Bitmap *texture1 = nullptr, *texture2 = nullptr;
//void GenerateTexture(Graphics::Bitmap*& tex, Graphics::Color color)
//{
//	if (tex) delete tex;
//	tex = new Graphics::Bitmap(50, 50);
//	for (int x = 0; x < tex->Width; ++x)
//	{
//		for (int y = 0; y < tex->Height; ++y)
//		{
//			if ((x % 2 == 0) ^ (y % 2 == 0))
//			{
//				tex->SetPixel(x, y, color);
//			}
//			else
//			{
//				tex->SetPixel(x, y, Graphics::Color(color.GetR(), color.GetG(), color.GetB(), 0x00));
//			}
//			//unsigned char value = ((x % 2 == 0) ^ (y % 2 == 0)) ? 255 : 0;
//			////texture->SetPixel(x, y, Graphics::Color(value, value, value, 0xFF));
//			//tex->SetPixel(x, y, Graphics::Color(color.GetR(), color.GetG(), color.GetB(), value));
//		}
//	}
//}
//
//
//
//class MainForm
//{
//public:
//	WAF::Window *MainWindow = nullptr;
//	WAF::GraphicsBox* gfxBox = nullptr;
//	WAF::TrackBar* trackBar1 = nullptr;
//
//public:
//	MainForm()
//	{
//		// MainWindow
//		WAF::Window::ConStruct wc;
//		wc.rect.width = 1000;
//		wc.rect.height = 600;
//		wc.caption = L"WinApiFramework Test";
//		wc.sizeRect.minWidth = 400;
//		wc.sizeRect.minHeight = 200;
//		wc.startStyle = WAF::Window::StartStyle::Normal;
//		MainWindow = new WAF::Window(wc);
//		MainWindow->Events.AddEventHandler<MainForm>(this, &MainForm::MainWindowEventsReceiver);
//
//		// gfxBox
//		WAF::GraphicsBox::ConStruct gbc;
//		gbc.rect = WAF::Rect(10, 10, MainWindow->ClientWidth - 20, MainWindow->ClientHeight - 20);
//		gbc.graphics.renderType = WAF::GraphicsBox::RenderType::RenderTypeDefault;
//		gbc.graphics.presentOption = WAF::GraphicsBox::PresentOption::PresentOptionWaitForDisplay;
//		gfxBox = new WAF::GraphicsBox(gbc);
//		gfxBox->Events.AddEventHandler<MainForm>(this, &MainForm::gfxBoxEventsReceiver);
//		MainWindow->AddControl(gfxBox);
//
//		WAF::GraphicsBox::TextFormatDescription tfd;
//		tfd.flowDirection = WAF::GraphicsBox::FlowDirection::FlowDirectionBottomToTop;
//		gfxBox->Gfx.SetDefaultTextFormat(tfd);
//
//		// trackBar1
//		WAF::TrackBar::ConStruct tbc;
//		tbc.rect = WAF::Rect(10, MainWindow->ClientHeight - 40, 300, 30);
//		tbc.trackRange = WAF::Range(0, 255);
//		tbc.startPosition = 0;
//		tbc.smallStep = 5;
//		tbc.largeStep = 20;
//		trackBar1 = new WAF::TrackBar(tbc);
//		trackBar1->Events.AddEventHandler<MainForm>(this, &MainForm::trackBar1EventsReceiver);
//		MainWindow->AddControl(trackBar1);
//
//		WAF::Framework::Keyboard.KeyEvents.AddEventHandler<MainForm>(this, &MainForm::FrameworkKeyboardEventHandler);
//	}
//	~MainForm()
//	{
//		if (gfxBox) delete gfxBox;
//		if (trackBar1) delete trackBar1;
//
//		if (MainWindow) delete MainWindow;
//	}
//
//
//	// MainWindow events receiver
//	void MainWindowEventsReceiver(WAF::Window::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::Window::Event::Type::Move:
//				MainWindow->SetCaption(std::to_wstring(MainWindow->WindowX) + L":" + std::to_wstring(MainWindow->WindowY));
//				break;
//			case WAF::Window::Event::Type::Resize:
//				MainWindow->SetCaption(std::to_wstring(MainWindow->WindowWidth) + L":" + std::to_wstring(MainWindow->WindowHeight));
//				gfxBox->Resize(MainWindow->ClientWidth - 20, MainWindow->ClientHeight - 50);
//				trackBar1->Move(10, MainWindow->ClientHeight - 40);
//				break;
//			case WAF::Window::Event::Type::CaptionChange:
//				MainWindow->Events.DisableEventHandlers();
//				MainWindow->SetCaption(MainWindow->GetCaption() + L" additional text" + std::to_wstring(gfxBox->Width) + L":" + std::to_wstring(gfxBox->Height));
//				MainWindow->Events.EnableEventHandlers();
//				break;
//		}
//	}
//
//	// gfxBox events receiver
//	void gfxBoxEventsReceiver(WAF::GraphicsBox::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::GraphicsBox::Event::Type::Resize:
//				//gfxBox->Gfx.SetBrushColor(Graphics::Color(0xFF, gfxBox->Width, 0x00));
//				break;
//		}
//	}
//
//	// trackBar1 events receiver
//	void trackBar1EventsReceiver(WAF::TrackBar::Event event)
//	{
//		switch (event.type)
//		{
//			case WAF::TrackBar::Event::Type::PositionChange:
//				//gfxBox->Gfx.SetBrushColor(Graphics::Color(trackBar1->Position, 0x00, 0x00));
//				break;
//		}
//	}
//
//
//	// FrameworkKeyboard
//	void FrameworkKeyboardEventHandler(WAF::Keyboard::KeyEvent event)
//	{
//		switch (event.type)
//		{
//			case WAF::Keyboard::KeyEvent::Type::Press:
//				if (event.key == WAF::Keyboard::Key::B)
//				{
//					WAF::Framework::Exit(0);
//				}
//				break;
//		}
//	}
//};
//MainForm *MF;
//
//void FreeTimeFunction()
//{
//	float mouseX = MF->gfxBox->GetMouseX();
//	float mouseY = MF->gfxBox->GetMouseY();
//
//	// [>] Begin Drawing
//	MF->gfxBox->Gfx.BeginDraw();
//	/*MF->gfxBox->Gfx.Clear(Graphics::Color(0xA0, 0xA0, 0xA0, 0xFF));*/
//	MF->gfxBox->Gfx.Clear(Graphics::Color::White);
//	MF->gfxBox->Gfx.SetSolidBrush(Graphics::Color(0xFF, 0x00, 0x00));
//
//	if (WAF::Framework::Mouse.LeftPressed) MF->gfxBox->Gfx.DrawLine(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(500.0f, 500.0f), 5.0f);
//
//	if (WAF::Framework::Mouse.RightPressed) MF->gfxBox->Gfx.DrawBitmap(
//		*texture1,
//		Graphics::Rect<float>(mouseX, mouseY, mouseX + 600.0f, mouseY + 400.0f),
//		Graphics::Rect<float>(0.0f, 0.0f, texture1->Width, texture1->Height),
//		1.0f, WAF::GraphicsBox::InterpolationMode::InterpolationModeNearestNeighbor);
//
//	if (WAF::Framework::Mouse.RightPressed) MF->gfxBox->Gfx.DrawBitmap(
//		*texture2,
//		Graphics::Rect<float>(100.0f, 100.0f, 700.0f, 500.0f),
//		Graphics::Rect<float>(0.0f, 0.0f, texture2->Width, texture2->Height),
//		1.0f, WAF::GraphicsBox::InterpolationMode::InterpolationModeNearestNeighbor);
//
//	//MF->gfxBox->Gfx.FillEllipse(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(100.0f, 50.0f));
//	MF->gfxBox->Gfx.DrawRoundedRectangle(Graphics::Point<float>(0.0f, 0.0f), Graphics::Point<float>(mouseX, mouseY), 50.0f, 50.0f, mouseX / 100.0f);
//
//	std::wostringstream os;
//	os << L"Lorem ipsum dolor sit\t amet, consectetur adipiscing elit. Ut facilisis risus in neque ullamcorper finibus. Proin eu lectus dignissim enim porttitor ultricies vitae vel mauris. Vestibulum eleifend porta enim at vulputate. Nunc imperdiet quam vel eros mollis pellentesque sit amet eu leo. Sed at nisl vitae arcu maximus lobortis. Suspendisse sodales urna urna, eu venenatis enim faucibus et. Praesent gravida metus vel ipsum ullamcorper gravida. Ut pretium ex vitae cursus posuere. Maecenas justo urna, iaculis ac pulvinar eleifend, vestibulum vel neque. Ut bibendum, sapien in pretium rhoncus, leo eros ullamcorper urna, quis vehicula leo neque sollicitudin nibh. Etiam convallis leo vitae nibh mattis, sit amet pretium nibh efficitur. Phasellus maximus, mauris eu finibus suscipit, ligula dui ultricies libero, at efficitur sem ligula sed sapien.";
//
//	MF->gfxBox->Gfx.SetSolidBrush(Graphics::Color(0x00, 0x00, 0x00), 0.5f);
//	MF->gfxBox->Gfx.DrawString(os.str(), Graphics::Rect<float>(10.0f, 10.0f, mouseX - 10.0f, mouseY - 10.0f));
//
//	MF->gfxBox->Gfx.EndDraw();
//	//if (WAF::Framework::Mouse.LeftPressed) MF->gfxBox->Gfx.FillEllipse(Graphics::Point2d<float>(mouseX, mouseY), Graphics::Point2d<float>(20.0f, 20.0f));
//	//MF->gfxBox->Gfx.Render();
//}
//
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR args, INT ncmd)
//{
//	GenerateTexture(texture1, Graphics::Color::Red);
//	GenerateTexture(texture2, Graphics::Color::Green);
//
//	MF = new MainForm();
//
//	WAF::Framework::SetCallBackFunction(FreeTimeFunction);
//	WAF::Framework::ProcessMessages();
//
//	delete MF;
//	return 0;
//}