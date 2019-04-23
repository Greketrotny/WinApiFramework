#include "WinApiFramework.h"
#include <chrono>

using namespace WinApiFramework;


class MainForm
{
	// -- fields -- //
public:
	Window *MainWindow = nullptr;
	GraphicsBox *gfxBox1 = nullptr;


	// -- constructor -- //
public:
	MainForm()
	{
		// MainWindow
		Window::Config wc;
		wc.caption = L"GraphicsBox test";
		wc.position = Window::Position::Center;
		wc.rect.width = 600;
		wc.rect.height = 400;
		wc.sizeRect.minWidth = 300;
		wc.sizeRect.minHeight = 300;
		MainWindow = new Window(wc, new EHMainWindow(this));
		//MainWindow->DisableResize();

		// gfxBox1
		GraphicsBox::Config gbc;
		gbc.rect.x = 5;
		gbc.rect.y = 5;
		gbc.rect.width = MainWindow->Width - 30;
		gbc.rect.height = MainWindow->Height - 50;
		gfxBox1 = new GraphicsBox(gbc);
		MainWindow->AddControl(gfxBox1);
	}
	~MainForm()
	{
		if (gfxBox1) delete gfxBox1;

		if (MainWindow) delete MainWindow;
	}


	// -- event handlers -- //
	struct EHMainWindow :public Window::EventHandler
	{
		MainForm *form;
		EHMainWindow(MainForm *form) { this->form = form; }

		void Move() override
		{
			form->MainWindow->SetCaption(L"window moved: "
				+ std::to_wstring(form->MainWindow->X)
				+ L" : "
				+ std::to_wstring(form->MainWindow->Y));
		}
		void Resize() override
		{
			form->MainWindow->SetCaption(L"window resized: "
				+ std::to_wstring(form->MainWindow->Width)
				+ L" : "
				+ std::to_wstring(form->MainWindow->Height));

			form->gfxBox1->Resize(form->MainWindow->Width - 30, form->MainWindow->Height - 50);
		}
	};

	struct EHgfxBox1 : public GraphicsBox::EventHandler
	{
		MainForm *form;
		EHgfxBox1(MainForm *form) { this->form = form; }
	};

	struct EHKeyboardKey : Keyboard::KeyEventHandler
	{
		bool created = false;
		MainForm *form;
		EHKeyboardKey(MainForm *form) { this->form = form; }

		void Press(Keyboard::KeyEvent keyEvent) override
		{

		}
	};
	struct EHMouse : Mouse::EventHandler
	{
		MainForm *form;
		EHMouse(MainForm *form) { this->form = form; }

		void Move()
		{
		}
	};

	// -- methods -- //
public:
	float *values, *values2;
	unsigned int count;
	bool toogle = true;

	struct Object
	{
		float x, y;
		float vx, vy;
		float ax, ay;

		void Update()
		{
			x += vx;
			y += vy;

			vx += ax;
			vy += ay;
		}
		void Friction()
		{
			vx *= 0.999f;
			vy *= 0.999f;
		}
	};
	Object ball;

	// -----------------
	void Init()
	{
		count = gfxBox1->Width * gfxBox1->Height;
		values = new float[count];
		values2 = new float[count];
		for (unsigned int i = 0; i < count; i++)
		{
			values[i] = 0.0f;
			values2[i] = 0.0f;
		}		

		ball.x = 50;
		ball.y = 100;
		ball.vx = 10.0f;
		ball.vy = -3.0f;
	}
	void UnInit()
	{
		if (values) delete[] values;
		if (values2) delete[] values2;
	}
	// -----------------



	void Simulation()
	{
		gfxBox1->Gfx.ClearPixelMap(0, 0, 0);

		float mx = gfxBox1->GetMouseX() - 10;
		float my = gfxBox1->GetMouseY() - 29;
		float range = 10.0f;
		unsigned int width = gfxBox1->Width;
		unsigned int height = gfxBox1->Height;

		float dx = 0.0f, dy = 0.0f;


		// first corner
		ball.vx += (10 - ball.x) * 0.001f;
		ball.vy += (10 - ball.y) * 0.001f;

		// second corner
		ball.vx += ((width - 10) - ball.x) * 0.001f;
		ball.vy += (10 - ball.y) * 0.001f;

		// third corner
		ball.vx += (10 - ball.x) * 0.001f;
		ball.vy += ((height - 10) - ball.y) * 0.001f;

		// fourth corner
		ball.vx += ((width - 10) - ball.x) * 0.001f;
		ball.vy += ((height - 10) - ball.y) * 0.001f;

		ball.vy += 0.2f;


		ball.Friction();
		ball.Update();


		//auto index = [width](unsigned int x, unsigned int y) -> int
		//{
		//	return y * width + x;
		//};
		//// Add heat
		//if (Framework::Mouse.LeftPressed)
		//{
		//	for (int x = mx - range; x < mx + range; x++)
		//	{
		//		for (int y = my - range; y < my + range; y++)
		//		{
		//			if (x >= 0 && x < gfxBox1->Width &&
		//				y >= 0 && y < gfxBox1->Height)
		//			{
		//				float distance = sqrtf((mx - x) * (mx - x) + (my - y) * (my - y));
		//				if (distance <= range)
		//				{
		//					values[index(x, y)] += 5.0f;
		//				}
		//			}
		//		}
		//	}
		//}
		//// take heat
		//else if (Framework::Mouse.RightPressed)
		//{
		//	for (int x = mx - range; x < mx + range; x++)
		//	{
		//		for (int y = my - range; y < my + range; y++)
		//		{
		//			if (x >= 0 && x < gfxBox1->Width &&
		//				y >= 0 && y < gfxBox1->Height)
		//			{
		//				float distance = sqrtf((mx - x) * (mx - x) + (my - y) * (my - y));
		//				if (distance <= range)
		//				{
		//					if (values[index(x, y)] > 5.0f)
		//						values[index(x, y)] -= 5.0f;
		//				}
		//			}
		//		}
		//	}
		//}
		//
		//// simulate heat transfer
		//for (unsigned int x = 1; x < width - 1; x++)
		//{
		//	for (unsigned int y = 1; y < height - 1; y++)
		//	{
		//		values2[index(x, y)] = values[index(x, y)];
		//		values2[index(x, y)] -= (values[index(x, y)] - values[index(x - 1, y)]) * 0.25;
		//		values2[index(x, y)] -= (values[index(x, y)] - values[index(x + 1, y)]) * 0.25;
		//		values2[index(x, y)] -= (values[index(x, y)] - values[index(x, y - 1)]) * 0.25;
		//		values2[index(x, y)] -= (values[index(x, y)] - values[index(x, y + 1)]) * 0.25;
		//	}
		//}
		//float *temp = values;
		//values = values2;
		//values2 = temp;
		//// render values
		//float min = 10000.0f;
		//for (unsigned int i = 0; i < count; i++)
		//{
		//	if (values[i] < min) min = values[i];
		//}
		//float max = 0.0f;
		//for (unsigned int i = 0; i < count; i++)
		//{
		//	if (values[i] > max) max = values[i];
		//}
		//for (unsigned int x = 0; x < width; x++)
		//{
		//	for (unsigned int y = 0; y < height; y++)
		//	{
		//		unsigned int color = (values[index(x, y)] - min) / (max - min) * 255.0f;
		//		gfxBox1->Gfx.SetPixel(x, y, color, color, color);
		//	}
		//}

		gfxBox1->Gfx.ClearGraphicsMap(Graphics::Color(0));
		gfxBox1->Gfx.ClearPixelMap(Graphics::Color(0));

		gfxBox1->Gfx.SetBrushColor(Graphics::Color(255, 0, 0));
		gfxBox1->Gfx.DrawLine(10, 10, ball.x, ball.y);
		gfxBox1->Gfx.DrawLine(width - 10, 10, ball.x, ball.y);
		gfxBox1->Gfx.DrawLine(10, height - 10, ball.x, ball.y);
		gfxBox1->Gfx.DrawLine(width - 10, height - 10, ball.x, ball.y);
		gfxBox1->Gfx.FillEllipse(ball.x, ball.y, 10.0f, 10.0f);

		gfxBox1->Gfx.DrawLine(ball.x, ball.y, ball.x + ball.vx * 5.0f, ball.y + ball.vy * 5.0f, 2.0f);
		gfxBox1->Gfx.Render();
	}
};

MainForm *MF;

void FreeTimeProcess()
{
	MF->Simulation();
	//Sleep(10);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR args, INT ncmd)
{
	MF = new MainForm();
	MF->Init();

	Framework::Keyboard.SetKeyEventHandler(new MainForm::EHKeyboardKey(MF));
	Framework::Mouse.SetEventHandler(new MainForm::EHMouse(MF));
	Framework::SetFreeTimeFunction(FreeTimeProcess);

	Framework::ProcessMessages();
	MF->UnInit();

	delete MF;
	return 0;
}


////#include "NeuralNetwork.h"
//
//#include <iostream>
//#include <string>
////#include "Matrix.h"
//#include <memory>
//
////#include "ToolKitC++/GraphicEngine.h"
////#include "ToolKitC++/World.h"
//
//using namespace WinApiFramework;
////using namespace G3DE::GraphicEngine;
////using namespace G3DE::World;
//
////unsigned long long FibGeneration(const unsigned int n);
//
//class MainForm
//{
//public:
//	Window *MainWindow = nullptr;
//	Window *SecondWindow = nullptr;
//	Button *button1 = nullptr, *button2 = nullptr, *button3 = nullptr;
//	CheckBox *cb1, *cb2, *cb3, *cb4;
//	Label *mainLabel, *label2;
//	RadioButton *radioButton1;
//	GroupBox *gb1;
//	ProgressBar *pb1;
//
//	// -- constructor -- //
//public:
//	MainForm()
//	{
//		// MainWindow
//		Window::Config wc;
//		wc.caption = L"WinApiFramework development";
//		wc.position = Window::Position::Center;
//		//wc.startStyle = Window::StartStyle::Maximized;
//		wc.rect.width = 1000;
//		wc.rect.height = 600;
//		wc.sizeRect.minWidth = 300;
//		wc.sizeRect.minHeight = 300;
//		wc.sizeRect.maxWidth = 1200;
//		wc.sizeRect.maxHeight = 800;
//		MainWindow = new Window(wc);
//		MainWindow->SetEventHandler(new MainWindowEH(this));
//
//		wc.rect.x = 50;
//		wc.rect.y = 50;
//		wc.position = Window::Position::Custom;
//		wc.startStyle = Window::StartStyle::Maximized;
//		//SecondWindow = new Window(wc);
//
//		// button1
//		Button::Config bc;
//		bc.caption = L"Button1 caption";
//		bc.rect.x = 20;
//		bc.rect.y = 30;
//		bc.rect.width = 120;
//		bc.rect.height = 50;
//		button1 = new Button(bc);
//		MainWindow->AddControl(button1);
//		button1->SetEventHandler(new Button1EH(this));
//
//		// button2
//		bc.caption = L"Button2 caption";
//		bc.rect.x = 20;
//		bc.rect.y = 90;
//		button2 = new Button(bc);
//		MainWindow->AddControl(button2);
//		button2->SetEventHandler(new Button2EH(this));
//
//		// button3
//		bc.caption = L"Button3 caption";
//		bc.rect.x = 20;
//		bc.rect.y = 150;
//		button3 = new Button(bc);
//		MainWindow->AddControl(button3);
//		button3->SetEventHandler(new Button3EH(this));
//
//		// checkBoxes -------------
//		CheckBox::Config cbc;
//		cbc.rect.x = 200;
//		cbc.rect.y = 20;
//		cbc.rect.width = 150;
//		cbc.rect.height = 50;
//		cbc.boxState = CheckBox::BoxState::UnCheck;
//		cbc.caption = L"First CheckBox";
//		cbc.isTripleState = false;
//		cb1 = new CheckBox(cbc);
//		MainWindow->AddControl(cb1);
//
//		cbc.rect.y = 60;
//		cbc.caption = L"Second CheckBox";
//		cb2 = new CheckBox(cbc);
//		MainWindow->AddControl(cb2);
//
//		cbc.rect.y = 100;
//		cbc.caption = L"Third CheckBox";
//		cbc.isTripleState = true;
//		cb3 = new CheckBox(cbc);
//		MainWindow->AddControl(cb3);
//
//		cbc.rect.y = 140;
//		cbc.caption = L"Fourth CheckBox";
//		cb4 = new CheckBox(cbc);
//		MainWindow->AddControl(cb4);
//
//		// mainLabel
//		Label::Config lc;
//		lc.rect.x = 400;
//		lc.rect.y = 20;
//		lc.rect.width = 250;
//		lc.rect.height = 250;
//		lc.textAlignment = Label::Center;
//		lc.caption = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris et orci ac erat scelerisque suscipit in eget libero. Fusce suscipit ornare risus, sit amet convallis lorem aliquam at. Cras tellus dui, sagittis nec semper vitae, facilisis ut odio. In aliquet metus lectus, eget maximus sem suscipit et. Sed tempor eros vel ultrices convallis. Maecenas nec volutpat mi. Vestibulum lectus ante, aliquet ut consequat sit amet, pretium at augue. Nulla luctus erat id pretium luctus. Sed dignissim augue ut efficitur suscipit. Vivamus eget aliquet elit. Nulla sagittis condimentum sapien eget sagittis. Sed lobortis, enim tempus hendrerit lobortis, justo sem laoreet justo, in interdum mauris nibh quis velit. Pellentesque in dui faucibus, interdum nibh ac, tincidunt leo. Pellentesque faucibus turpis at ligula egestas tincidunt. Praesent eu interdum mauris, nec commodo elit. Nam sodales odio est.";
//		mainLabel = new Label(lc);
//		MainWindow->AddControl(mainLabel);
//
//		lc.caption = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris et orci ac erat scelerisque suscipit in eget libero. Fusce suscipit ornare risus, sit amet convallis lorem aliquam at. Cras tellus dui, sagittis nec semper vitae, facilisis ut odio. In aliquet metus lectus, eget maximus sem suscipit et. Sed tempor eros vel ultrices convallis. Maecenas nec volutpat mi. Vestibulum lectus ante, aliquet ut consequat sit amet, pretium at augue. Nulla luctus erat id pretium luctus. Sed dignissim augue ut efficitur suscipit. Vivamus eget aliquet elit. Nulla sagittis condimentum sapien eget sagittis. Sed lobortis, enim tempus hendrerit lobortis, justo sem laoreet justo, in interdum mauris nibh quis velit. Pellentesque in dui faucibus, interdum nibh ac, tincidunt leo. Pellentesque faucibus turpis at ligula egestas tincidunt. Praesent eu interdum mauris, nec commodo elit. Nam sodales odio est.";
//		lc.rect.x = 700;
//		label2 = new Label(lc);
//		MainWindow->AddControl(label2);
//
//		mainLabel->SetTextAligment(Label::Right);
//
//		// radioButton1
//		RadioButton::Config rbc;
//		rbc.rect.x = 20;
//		rbc.rect.y = 300;
//		rbc.rect.width = 100;
//		rbc.rect.height = 50;
//		//radioButton1 = new RadioButton(rbc);
//		//MainWindow->AddControl(radioButton1);
//
//		// gb1
//		GroupBox::Config gbc;
//		gbc.rect.x = 10;
//		gbc.rect.y = 10;
//		gbc.rect.width = 150;
//		gbc.rect.height = 200;
//		gbc.caption = L"group box caption";
//		//gb1 = new GroupBox(gbc);
//		//MainWindow->AddControl(gb1);
//
//		//gb1->AddControl(button1);
//		//gb1->AddControl(button2);
//		//gb1->AddControl(button3);
//
//		//pb1
//		ProgressBar::Config pbc;
//		pbc.rect.x = 200;
//		pbc.rect.y = 300;
//		pbc.rect.width = 300;
//		pbc.rect.height = 20;
//		pbc.position = 25;
//		pbc.maxValue = 100;
//		pb1 = new ProgressBar(pbc);
//		MainWindow->AddControl(pb1);
//	}
//	~MainForm()
//	{
//		if (button1) delete button1;
//		if (button2) delete button2;
//		if (button3) delete button3;
//
//		if (cb1) delete cb1;
//		if (cb2) delete cb2;
//		if (cb3) delete cb3;
//		if (cb4) delete cb4;
//
//		if (mainLabel) delete mainLabel;
//		if (label2) delete label2;
//
//		if (radioButton1) delete radioButton1;
//
//		if (gb1) delete gb1;
//
//		if (pb1) delete pb1;
//
//		if (MainWindow) delete MainWindow;
//	}
//
//	struct MainWindowEH : public Window::EventHandler
//	{
//		MainForm *form = nullptr;
//		MainWindowEH(MainForm *f) { form = f; }
//
//		void Move() override
//		{
//			WriteData();
//		}
//		void Resize() override
//		{
//			WriteData();
//		}
//		void WriteData()
//		{
//			form->MainWindow->SetCaption(L"Coordinates: [" +
//				std::to_wstring(form->MainWindow->X) + L";" +
//				std::to_wstring(form->MainWindow->Y) + L"]" +
//				L" Dimensions: [" +
//				std::to_wstring(form->MainWindow->Width) + L";" +
//				std::to_wstring(form->MainWindow->Height) + L"]");
//		}
//
//		void Minimize() override
//		{
//			form->MainWindow->SetCaption(L"Minimized");
//		}
//	};
//	
//	struct ControlEventHandler
//	{
//		MainForm *form = nullptr;
//		ControlEventHandler(MainForm *form)
//		{
//			this->form = form;
//		}
//	};
//	struct Button1EH : public ControlEventHandler, public Button::EventHandler
//	{
//		Button1EH(MainForm *form)
//			: ControlEventHandler(form)
//		{}
//		void Click() override
//		{
//			form->button1->SetCaption(L"button clicked! \n and new line!");
//			form->button1->DisableControl();
//		}
//		void DoubleClick() override
//		{
//			form->button1->SetCaption(L"button double clicked!");
//		}
//		void Focus() override
//		{
//			form->button1->SetCaption(L"button focused!");
//		}
//		void Unfocus() override
//		{
//			form->button1->SetCaption(L"button unfocused!");
//		}
//		void Disable() override
//		{
//			form->button1->SetCaption(L"control disabled");
//		}
//
//	};
//	struct Button2EH : public ControlEventHandler, public Button::EventHandler
//	{
//		Button2EH(MainForm *form)
//			:ControlEventHandler(form)
//		{}
//
//		void HandleEvent(Button::Event event) override
//		{
//			if (event.type == Button::Event::Click) Click();
//			if (event.type == Button::Event::DoubleClick) DoubleClick();
//			if (event.type == Button::Event::Focus) Focus();
//			if (event.type == Button::Event::Unfocus) UnFocus();
//		}
//		void Click()
//		{
//			form->button2->SetCaption(L"button clicked!");
//		}
//		void DoubleClick()
//		{
//			form->button2->SetCaption(L"button double clicked!");
//		}
//		void Focus()
//		{
//			form->button2->SetCaption(L"button focused!");
//		}
//		void UnFocus()
//		{
//			form->button2->SetCaption(L"button unfocused!");
//		}
//	};
//	struct Button3EH : public ControlEventHandler, public Button::EventHandler
//	{
//		Button3EH(MainForm *form)
//			: ControlEventHandler(form)
//		{}
//
//		void HandleEvent(Button::Event event) override
//		{
//			if (event.type == Button::Event::Click) Click();
//			if (event.type == Button::Event::DoubleClick) DoubleClick();
//			if (event.type == Button::Event::Focus) Focus();
//			if (event.type == Button::Event::Unfocus) UnFocus();
//		}
//		void Click()
//		{
//			form->button3->SetCaption(L"button clicked!");
//		}
//		void DoubleClick()
//		{
//			form->button3->SetCaption(L"button double clicked!");
//		}
//		void Focus()
//		{
//			form->button3->SetCaption(L"button focused!");
//		}
//		void UnFocus()
//		{
//			form->button3->SetCaption(L"button unfocused!");
//		}
//	};
//};
//
//
//MainForm *mainForm = nullptr;
//
//void FreeTimeProcess()
//{
//	static std::wstring str = L"";
//	Keyboard::CharEvent ev = Framework::Keyboard.GetCharEvent();
//	if (ev.type != Keyboard::CharEvent::Type::Invalid)
//	{
//		if (ev.type == Keyboard::CharEvent::Type::CharInput)
//		{
//			str += ev.character;
//		}
//		mainForm->MainWindow->SetCaption(str);
//	}
//
//	Keyboard::KeyEvent kev = Framework::Keyboard.GetKeyEvent();
//	if (kev.type == Keyboard::KeyEvent::Type::Press)
//	{
//		mainForm->button1->SetCaption(std::to_wstring(kev.key));
//	}
//
//	Mouse::Event mev = Framework::Mouse.GetEvent();
//	if (mev.type != Mouse::Event::Type::Invalid)
//	{
//		if (mev.type == Mouse::Event::Type::Move)
//		{
//			std::wstring position = L"[x: ";
//			position += std::to_wstring(Framework::Mouse.X);
//			position += L",  y: ";
//			position += std::to_wstring(Framework::Mouse.Y);
//			position += L"]";
//			mainForm->button2->SetCaption(position);
//		}
//		if (mev.type == Mouse::Event::Type::LeftPress)
//		{
//			mainForm->button3->SetCaption(L"Mouse left pressed!");
//			mainForm->MainWindow->EnableResize();
//		}
//		if (mev.type == Mouse::Event::Type::RightPress)
//		{
//			mainForm->MainWindow->DisableResize();
//		}
//	}
//
//	std::wstring buttonStates = L"";
//	if (Framework::Mouse.LeftPressed) buttonStates += L"left pressed\n";
//	if (Framework::Mouse.RightPressed) buttonStates += L"right pressed\n";
//	if (Framework::Mouse.MiddlePressed) buttonStates += L"Middle pressed\n";
//	mainForm->button3->SetCaption(buttonStates);
//	
//	Sleep(10);
//}
//
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR args, INT ncmd)
//{
//	mainForm = new MainForm;
//
//	Framework::SetFreeTimeFunction(FreeTimeProcess);
//	Framework::ProcessMessages();
//
//	delete mainForm;
//	return 0;
//}