#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include "WinApiFramework.h"

#include <math.h>
#include <time.h>
#include <sstream>

namespace WAF = WinApiFramework;

Graphics::Bitmap *texture1 = nullptr, *texture2 = nullptr;
void GenerateTexture(Graphics::Bitmap*& tex, Graphics::Color color)
{
	if (tex) delete tex;
	tex = new Graphics::Bitmap(500, 400);
	for (int x = 0; x < tex->Width; ++x)
	{
		for (int y = 0; y < tex->Height; ++y)
		{
			if ((x % 2 == 0) ^ (y % 2 == 0))
			{
				tex->SetPixel(x, y, color);
			}
			else
			{
				tex->SetPixel(x, y, Graphics::Color(color.GetR(), color.GetG(), color.GetB(), 0x00));
			}
			//unsigned char value = ((x % 2 == 0) ^ (y % 2 == 0)) ? 255 : 0;
			////texture->SetPixel(x, y, Graphics::Color(value, value, value, 0xFF));
			//tex->SetPixel(x, y, Graphics::Color(color.GetR(), color.GetG(), color.GetB(), value));
		}
	}
}
void DestroyTextures()
{
	if (texture1) delete texture1;
	if (texture2) delete texture2;
}

class MainForm
{
	// -- MainForm::fields -- //
public:
	WAF::Window *MainWindow = nullptr;

	WAF::Label* eventHistoryLabel = nullptr;

	WAF::CheckBox* checkBox1 = nullptr;
	WAF::Edit* edit1 = nullptr;
	WAF::Label* label1 = nullptr;
	WAF::ProgressBar* progressBar = nullptr;
	WAF::TrackBar* trackBar = nullptr;
	WAF::GraphicsBox* gfxBox = nullptr;
	WAF::GroupBox* groupBox = nullptr;

	std::vector<std::wstring> eventsHistory;


	// -- MainForm::constructors -- //
public:
	MainForm()
	{
		WAF::Framework::Keyboard.KeyEvents.AddEventHandler<MainForm>(this, &MainForm::FrameworkKeyboardEventHandler);
		WAF::Framework::Mouse.Events.AddEventHandler<MainForm>(this, &MainForm::FrameworkMouse_EH);

		// MainWindow
		MainWindow = WAF::Framework::CreateNewWindow(
			WAF::ConStruct<WAF::Window>(
				L"WinApiFramework test",
				WAF::Rect(50, 50, 1200, 700),
				WAF::Window::Position::Center,
				WAF::Window::StartStyle::Normal,
				WAF::SizeRect(200u, 100u, 2000u, 1000u),
				WAF::Size(1100, 600)));
		MainWindow->Events.AddEventHandler<MainForm>(this, &MainForm::MainWindow_EH);


//		// eventHistoryLabel
//		eventHistoryLabel = MainWindow->CreateControl<WAF::Label>(WAF::ConStruct<WAF::Label>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(10, 10, 150, 400)),
//			L"events"));
//
//
		// checkBox1
		checkBox1 = MainWindow->CreateControl<WAF::CheckBox>(WAF::ConStruct<WAF::CheckBox>(
			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(300, 10, 100, 50)),
			L"check1",
			true,
			WAF::CheckBox::BoxState::MiddleState));
		checkBox1->Events.AddEventHandler<MainForm>(this, &MainForm::CheckBox1_EH);
//
//		// edit1
//		edit1 = MainWindow->CreateControl<WAF::Edit>(WAF::ConStruct<WAF::Edit>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(300, 70, 100, 50)),
//			L"default text", WAF::Edit::TextAlignment::Right,
//			WAF::Edit::LettersMode::UpperCase,
//			false,
//			false,
//			100u));
//		edit1->Events.AddEventHandler<MainForm>(this, &MainForm::Edit1_EH);
//
//		// label1
//		label1 = MainWindow->CreateControl<WAF::Label>(WAF::ConStruct<WAF::Label>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(300, 140, 100, 50)),
//			L"A box with a frame drawn with the same color as the window background. This color is whilte in the background.",
//			WAF::Label::TextAlignment::Right));
//		label1->Events.AddEventHandler<MainForm>(this, &MainForm::Label1_EH);
//
//		// progressBar
//		progressBar = MainWindow->CreateControl<WAF::ProgressBar>(WAF::ConStruct<WAF::ProgressBar>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(300, 200, 150, 20)),
//			WAF::Range(0, 100),
//			20,
//			WAF::ProgressBar::BarState::Normal,
//			WAF::ProgressBar::BarOrientation::Horizontal,
//			WAF::ProgressBar::BarDisplayStyle::Default, 1u));
//
//		// trackBar
//		trackBar = MainWindow->CreateControl<WAF::TrackBar>(WAF::ConStruct<WAF::TrackBar>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(300, 250, 150, 40)),
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
//		// graphicsBox
//		WAF::ConStruct<WAF::GraphicsBox> gbc;
//		//gbc.rect = WAF::Rect(10, 10, MainWindow->ClientRect.size.width - 20, MainWindow->ClientHeight - 20);
//		gbc.rect = WAF::Rect(500, 10, 600, 400);
//		gbc.graphics.renderType = WAF::GraphicsBox::RenderType::RenderTypeDefault;
//		gbc.graphics.presentOption = WAF::GraphicsBox::PresentOption::PresentOptionWaitForDisplay;
//		gfxBox = MainWindow->CreateControl<WAF::GraphicsBox>(gbc);
//
//		// groupBox
//		groupBox = MainWindow->CreateControl<WAF::GroupBox>(WAF::ConStruct<WAF::GroupBox>(
//			WAF::ConStruct<WAF::ChildControl>(WAF::Rect(10, 450, 350, 150))));
	}
	~MainForm()
	{
		if (MainWindow) MainWindow->Destroy();
	}

	void DisplayEventHistory()
	{
		while (eventsHistory.size() > 20)
		{
			eventsHistory.erase(eventsHistory.begin());
		}

		std::wstring events = L"";
		for (size_t i = 0; i < eventsHistory.size(); i++)
		{
			events += eventsHistory[i] + L"\n";
		}
		eventHistoryLabel->SetCaption(events);
	}

	// -- MainForm::event_handlers -- //
	// MainWindow events handler
	void MainWindow_EH(WAF::Window::Event event)
	{
		switch (event.type)
		{
			case WAF::Window::Event::Type::Move:
			{
				DisplayMainWindowProps();
				break;
			}
			case WAF::Window::Event::Type::Resize:
			{
				// display new MainWindow properties
				DisplayMainWindowProps();
				break;
			}
		}
	}
	
	void Button1_EH(WAF::Button::Event event)
	{
		switch (event.type)
		{
			case WAF::Button::Event::Type::Click:
				break;
			case WAF::Button::Event::Type::DoubleClick:
				break;
		}
	}
	void CheckBox1_EH(WAF::CheckBox::Event event)
	{
		switch (event.type)
		{
			case WAF::CheckBox::Event::Type::Check:
				if (checkBox1->State == WAF::CheckBox::BoxState::Check)
					checkBox1->SetCaption(L"check box checked!");
				break;
			case WAF::CheckBox::Event::Type::MiddleState:
				checkBox1->SetCaption(L"check box in middle state!");
				break;
			case WAF::CheckBox::Event::Type::UnCheck:
				checkBox1->SetCaption(L"check box unchecked!");
				break;
		}
	}
	void Edit1_EH(WAF::Edit::Event event)
	{
		switch (event.type)
		{
			case WAF::Edit::Event::Type::TextChanged:
				MainWindow->SetCaption(L"Texting!");
				break;
			case WAF::Edit::Event::Type::TextLimitReached:
				MainWindow->SetCaption(L"Limit reached!");
				break;

		}
	}
	void Label1_EH(WAF::Label::Event event)
	{
		switch (event.type)
		{
			case WAF::Label::Event::Type::Clicked:
				label1->SetCaption(L"Label Clicked!");
				break;
			case WAF::Label::Event::Type::DoubleClicked:
				label1->SetCaption(L"Label double clicked!");
				break;
		}
	}
	void TrackBar_EH(WAF::TrackBar::Event ev)
	{
		switch (ev.type)
		{
			case WAF::TrackBar::Event::Type::LinedUp:
				eventsHistory.push_back(L"LinedUp!");
				break;
			case WAF::TrackBar::Event::Type::LinedDown:
				eventsHistory.push_back(L"LinedDown!");
				break;
			case WAF::TrackBar::Event::Type::PagedUp:
				eventsHistory.push_back(L"PagedUp!");
				break;
			case WAF::TrackBar::Event::Type::PagedDown:
				eventsHistory.push_back(L"PagedDown!");
				break;
			case WAF::TrackBar::Event::Type::MovedToTop:
				eventsHistory.push_back(L"MovedToTop!");
				break;
			case WAF::TrackBar::Event::Type::MovedToBottom:
				eventsHistory.push_back(L"MovedToBottom!");
				break;
			case WAF::TrackBar::Event::Type::DraggingFinnished:
				eventsHistory.push_back(L"DraggingFinnished!");
				break;
			case WAF::TrackBar::Event::Type::ThumbDragged:
				eventsHistory.push_back(L"ThumbDragged!");
				break;
			case WAF::TrackBar::Event::Type::ThumbPositionChanged:
				eventsHistory.push_back(L"PositionChanged!");
				break;
		}
		DisplayEventHistory();
	}

	// FrameworkKeyboard
	void FrameworkKeyboardEventHandler(WAF::Keyboard::KeyEvent event)
	{
		switch (event.type)
		{
			case WAF::Keyboard::KeyEvent::Type::Press:
				if (event.key == WAF::Keyboard::Key::B)
				{
					WAF::Framework::Exit(0);
				}
				if (event.key == WAF::Keyboard::Key::Digit9)
				{
					//trackBar->SetThumbPosition(trackBar->GetPosition() - 5);
					trackBar->SetMinTrackValue(trackBar->GetMinTrackValue() - 5);
				}
				if (event.key == WAF::Keyboard::Key::Digit0)
				{
					//trackBar->SetThumbPosition(trackBar->GetPosition() + 5);
					trackBar->SetMaxSelectValue(trackBar->GetMaxSelectValue() + 5);
				}
				if (event.key == WAF::Keyboard::Key::Q)
				{
					if (trackBar)
					{
						trackBar->Destroy();
						trackBar = nullptr;
					}
				}
				break;
		}
	}
	void FrameworkMouse_EH(WAF::Mouse::Event event)
	{
		switch (event.type)
		{
			case WAF::Mouse::Event::Type::RightPress:
				break;
			case WAF::Mouse::Event::Type::Move:
			{
				MainWindow->SetCaption(
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
					std::to_wstring(MainWindow->GetCanvasMousePosition().y));

				//progressBar->SetPosition(MainWindow->GetClientMousePosition().x / float(MainWindow->ClientRect.size.width) * 100.0f);
				//progressBar->StepIt();

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

void CallBackFunction()
{
	Sleep(1);
	return;

	float mouseX = MF->gfxBox->GetMousePosition().x;
	float mouseY = MF->gfxBox->GetMousePosition().y;

	// [>] Begin Drawing
	MF->gfxBox->Gfx.BeginDraw();
	/*MF->gfxBox->Gfx.Clear(Graphics::Color(0xA0, 0xA0, 0xA0, 0xFF));*/
	MF->gfxBox->Gfx.Clear(Graphics::Color::White);
	MF->gfxBox->Gfx.SetSolidBrush(Graphics::Color(0xFF, 0x00, 0x00));

	if (WAF::Framework::Mouse.LeftPressed) MF->gfxBox->Gfx.DrawLine(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(500.0f, 500.0f), 5.0f);

	if (WAF::Framework::Mouse.RightPressed) MF->gfxBox->Gfx.DrawBitmap(
		*texture1,
		Graphics::Rect<float>(mouseX, mouseY, mouseX + 600.0f, mouseY + 400.0f),
		Graphics::Rect<float>(0.0f, 0.0f, texture1->Width, texture1->Height),
		1.0f, WAF::GraphicsBox::InterpolationMode::InterpolationModeNearestNeighbor);

	if (WAF::Framework::Mouse.RightPressed) MF->gfxBox->Gfx.DrawBitmap(
		*texture2,
		Graphics::Rect<float>(100.0f, 100.0f, 700.0f, 500.0f),
		Graphics::Rect<float>(0.0f, 0.0f, texture2->Width, texture2->Height),
		1.0f, WAF::GraphicsBox::InterpolationMode::InterpolationModeNearestNeighbor);

	//MF->gfxBox->Gfx.FillEllipse(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(100.0f, 50.0f));
	MF->gfxBox->Gfx.DrawRoundedRectangle(Graphics::Point<float>(0.0f, 0.0f), Graphics::Point<float>(mouseX, mouseY), 50.0f, 50.0f, mouseX / 100.0f);

	std::wostringstream os;
	os << L"Lorem ipsum dolor sit\t amet, consectetur adipiscing elit. Ut facilisis risus in neque ullamcorper finibus. Proin eu lectus dignissim enim porttitor ultricies vitae vel mauris. Vestibulum eleifend porta enim at vulputate. Nunc imperdiet quam vel eros mollis pellentesque sit amet eu leo. Sed at nisl vitae arcu maximus lobortis. Suspendisse sodales urna urna, eu venenatis enim faucibus et. Praesent gravida metus vel ipsum ullamcorper gravida. Ut pretium ex vitae cursus posuere. Maecenas justo urna, iaculis ac pulvinar eleifend, vestibulum vel neque. Ut bibendum, sapien in pretium rhoncus, leo eros ullamcorper urna, quis vehicula leo neque sollicitudin nibh. Etiam convallis leo vitae nibh mattis, sit amet pretium nibh efficitur. Phasellus maximus, mauris eu finibus suscipit, ligula dui ultricies libero, at efficitur sem ligula sed sapien.";

	MF->gfxBox->Gfx.SetSolidBrush(Graphics::Color(0x00, 0x00, 0x00), 0.5f);
	MF->gfxBox->Gfx.DrawString(os.str(), Graphics::Rect<float>(10.0f, 10.0f, mouseX - 10.0f, mouseY - 10.0f));

	if (WAF::Framework::Mouse.LeftPressed) MF->gfxBox->Gfx.FillEllipse(Graphics::Point<float>(mouseX, mouseY), Graphics::Point<float>(20.0f, 20.0f));
	MF->gfxBox->Gfx.EndDraw();
	//MF->gfxBox->Gfx.Render();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR args, INT ncmd)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	GenerateTexture(texture1, Graphics::Color(0xFF, 0x00, 0x00, 0xFF));
	GenerateTexture(texture2, Graphics::Color(0x00, 0xFF, 0x00, 0xFF));
	MF = new MainForm();

	WAF::Framework::SetCallBackFunction(CallBackFunction);
	WAF::Framework::ProcessMessages();

	delete MF;
	DestroyTextures();

	return 0;
}


//#include "WinApiFramework.h"
//
//#include <sstream>
//
//namespace WAF = WinApiFramework;
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