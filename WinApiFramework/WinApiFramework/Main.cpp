#include "WinApiFramework.h"
//#include "NeuralNetwork.h"

#include <iostream>
#include <string>
//#include "Matrix.h"
#include <memory>

//#include "ToolKitC++/GraphicEngine.h"
//#include "ToolKitC++/World.h"

using namespace WinApiFramework;
//using namespace G3DE::GraphicEngine;
//using namespace G3DE::World;

//unsigned long long FibGeneration(const unsigned int n);

class MainForm
{
public:
	Window *MainWindow = nullptr;
	Button *button1 = nullptr, *button2 = nullptr, *button3 = nullptr;
	CheckBox *cb1, *cb2, *cb3, *cb4;
	Label *mainLabel, *label2;
	RadioButton *radioButton1;
	GroupBox *gb1;
	ProgressBar *pb1;

	// -- constructor -- //
public:
	MainForm()
	{
		// MainWindow
		Window::Config wc;
		wc.caption = L"WinApiFramework development";
		wc.position = Window::Position::Center;
		wc.rect.width = 1000;
		wc.rect.height = 600;
		wc.sizeRect.minWidth = 300;
		wc.sizeRect.minHeight = 300;
		wc.sizeRect.maxWidth = 1200;
		wc.sizeRect.maxHeight = 800;
		MainWindow = new Window(wc);
		MainWindow->SetEventHandler(new MainWindowEH(this));

		// button1
		Button::Config bc;
		bc.caption = L"Button1 caption";
		bc.rect.x = 20;
		bc.rect.y = 30;
		bc.rect.width = 120;
		bc.rect.height = 50;
		button1 = new Button(bc);
		MainWindow->AddControl(button1);
		button1->SetEventHandler(new Button1EH(this));

		// button2
		bc.caption = L"Button2 caption";
		bc.rect.x = 20;
		bc.rect.y = 90;
		button2 = new Button(bc);
		MainWindow->AddControl(button2);
		button2->SetEventHandler(new Button2EH(this));

		// button3
		bc.caption = L"Button3 caption";
		bc.rect.x = 20;
		bc.rect.y = 150;
		button3 = new Button(bc);
		MainWindow->AddControl(button3);
		button3->SetEventHandler(new Button3EH(this));

		// checkBoxes -------------
		CheckBox::Config cbc;
		cbc.rect.x = 200;
		cbc.rect.y = 20;
		cbc.rect.width = 150;
		cbc.rect.height = 50;
		cbc.boxState = CheckBox::BoxState::UnCheck;
		cbc.caption = L"First CheckBox";
		cbc.isTripleState = false;
		cb1 = new CheckBox(cbc);
		MainWindow->AddControl(cb1);

		cbc.rect.y = 60;
		cbc.caption = L"Second CheckBox";
		cb2 = new CheckBox(cbc);
		MainWindow->AddControl(cb2);

		cbc.rect.y = 100;
		cbc.caption = L"Third CheckBox";
		cbc.isTripleState = true;
		cb3 = new CheckBox(cbc);
		MainWindow->AddControl(cb3);

		cbc.rect.y = 140;
		cbc.caption = L"Fourth CheckBox";
		cb4 = new CheckBox(cbc);
		MainWindow->AddControl(cb4);

		// mainLabel
		Label::Config lc;
		lc.rect.x = 400;
		lc.rect.y = 20;
		lc.rect.width = 250;
		lc.rect.height = 250;
		lc.textAlignment = Label::Center;
		lc.caption = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris et orci ac erat scelerisque suscipit in eget libero. Fusce suscipit ornare risus, sit amet convallis lorem aliquam at. Cras tellus dui, sagittis nec semper vitae, facilisis ut odio. In aliquet metus lectus, eget maximus sem suscipit et. Sed tempor eros vel ultrices convallis. Maecenas nec volutpat mi. Vestibulum lectus ante, aliquet ut consequat sit amet, pretium at augue. Nulla luctus erat id pretium luctus. Sed dignissim augue ut efficitur suscipit. Vivamus eget aliquet elit. Nulla sagittis condimentum sapien eget sagittis. Sed lobortis, enim tempus hendrerit lobortis, justo sem laoreet justo, in interdum mauris nibh quis velit. Pellentesque in dui faucibus, interdum nibh ac, tincidunt leo. Pellentesque faucibus turpis at ligula egestas tincidunt. Praesent eu interdum mauris, nec commodo elit. Nam sodales odio est.";
		mainLabel = new Label(lc);
		//MainWindow->AddControl(mainLabel);

		lc.caption = L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris et orci ac erat scelerisque suscipit in eget libero. Fusce suscipit ornare risus, sit amet convallis lorem aliquam at. Cras tellus dui, sagittis nec semper vitae, facilisis ut odio. In aliquet metus lectus, eget maximus sem suscipit et. Sed tempor eros vel ultrices convallis. Maecenas nec volutpat mi. Vestibulum lectus ante, aliquet ut consequat sit amet, pretium at augue. Nulla luctus erat id pretium luctus. Sed dignissim augue ut efficitur suscipit. Vivamus eget aliquet elit. Nulla sagittis condimentum sapien eget sagittis. Sed lobortis, enim tempus hendrerit lobortis, justo sem laoreet justo, in interdum mauris nibh quis velit. Pellentesque in dui faucibus, interdum nibh ac, tincidunt leo. Pellentesque faucibus turpis at ligula egestas tincidunt. Praesent eu interdum mauris, nec commodo elit. Nam sodales odio est.";
		lc.rect.x = 700;
		label2 = new Label(lc);
		//MainWindow->AddControl(label2);

		mainLabel->SetTextAligment(Label::Right);

		// radioButton1
		RadioButton::Config rbc;
		rbc.rect.x = 20;
		rbc.rect.y = 300;
		rbc.rect.width = 100;
		rbc.rect.height = 50;
		radioButton1 = new RadioButton(rbc);
		MainWindow->AddControl(radioButton1);

		// gb1
		GroupBox::Config gbc;
		gbc.rect.x = 10;
		gbc.rect.y = 10;
		gbc.rect.width = 150;
		gbc.rect.height = 200;
		gbc.caption = L"group box caption";
		gb1 = new GroupBox(gbc);
		MainWindow->AddControl(gb1);

		gb1->AddControl(button1);
		gb1->AddControl(button2);
		gb1->AddControl(button3);

		//pb1
		ProgressBar::Config pbc;
		pbc.rect.x = 200;
		pbc.rect.y = 300;
		pbc.rect.width = 300;
		pbc.rect.height = 20;
		pbc.position = 25;
		pbc.maxValue = 2000;
		pb1 = new ProgressBar(pbc);
		MainWindow->AddControl(pb1);
	}
	~MainForm()
	{
		if (button1) delete button1;
		if (button2) delete button2;
		if (button3) delete button3;

		if (cb1) delete cb1;
		if (cb2) delete cb2;
		if (cb3) delete cb3;
		if (cb4) delete cb4;

		if (mainLabel) delete mainLabel;
		if (label2) delete label2;

		if (radioButton1) delete radioButton1;

		if (gb1) delete gb1;

		if (pb1) delete pb1;

		if (MainWindow) delete MainWindow;
	}

	struct MainWindowEH : public Window::EventHandler
	{
		MainForm *form = nullptr;
		MainWindowEH(MainForm *f) { form = f; }

		void Move() override
		{
			WriteData();
		}
		void Resize() override
		{
			WriteData();
		}
		void WriteData()
		{
			form->MainWindow->SetCaption(L"Coordinates: [" +
				std::to_wstring(form->MainWindow->X) + L";" +
				std::to_wstring(form->MainWindow->Y) + L"]" +
				L" Dimensions: [" +
				std::to_wstring(form->MainWindow->Width) + L";" +
				std::to_wstring(form->MainWindow->Height) + L"]");
		}
	};


	struct ControlEventHandler
	{
		MainForm *form = nullptr;
		ControlEventHandler(MainForm *form)
		{
			this->form = form;
		}
	};
	struct Button1EH : public ControlEventHandler, public Button::EventHandler
	{
		Button1EH(MainForm *form)
			: ControlEventHandler(form)
		{}
		void Click() override
		{
			form->button1->SetCaption(L"button clicked! \n and new line!");
		}
		void DoubleClick() override
		{
			form->button1->SetCaption(L"button double clicked!");
		}
		void Focus() override
		{
			form->button1->SetCaption(L"button focused!");
		}
		void Unfocus() override
		{
			form->button1->SetCaption(L"button unfocused!");
		}

	};
	struct Button2EH : public ControlEventHandler, public Button::EventHandler
	{
		Button2EH(MainForm *form)
			:ControlEventHandler(form)
		{}

		void HandleEvent(Button::Event event) override
		{
			if (event.type == Button::Event::Click) Click();
			if (event.type == Button::Event::DoubleClick) DoubleClick();
			if (event.type == Button::Event::Focus) Focus();
			if (event.type == Button::Event::Unfocus) UnFocus();
		}
		void Click()
		{
			form->button2->SetCaption(L"button clicked!");
		}
		void DoubleClick()
		{
			form->button2->SetCaption(L"button double clicked!");
		}
		void Focus()
		{
			form->button2->SetCaption(L"button focused!");
		}
		void UnFocus()
		{
			form->button2->SetCaption(L"button unfocused!");
		}
	};
	struct Button3EH : public ControlEventHandler, public Button::EventHandler
	{
		Button3EH(MainForm *form)
			: ControlEventHandler(form)
		{}

		void HandleEvent(Button::Event event) override
		{
			if (event.type == Button::Event::Click) Click();
			if (event.type == Button::Event::DoubleClick) DoubleClick();
			if (event.type == Button::Event::Focus) Focus();
			if (event.type == Button::Event::Unfocus) UnFocus();
		}
		void Click()
		{
			form->button3->SetCaption(L"button clicked!");
		}
		void DoubleClick()
		{
			form->button3->SetCaption(L"button double clicked!");
		}
		void Focus()
		{
			form->button3->SetCaption(L"button focused!");
		}
		void UnFocus()
		{
			form->button3->SetCaption(L"button unfocused!");
		}
	};
};


MainForm *mainForm = nullptr;

void FreeTimeProcess()
{
	static int i = 0;
	Sleep(10);


	if (mainForm->pb1->Position < 1000)
	{
		mainForm->pb1->SetPosition(i);
		i++;
	}
	else
	{
		mainForm->pb1->SetState(ProgressBar::BarState::Pause);
	}

	/*i++;
	if (i == 200)
	{
		delete mainForm->button1;
		mainForm->cb1->DisableControl();
		mainForm->button3->DisableControl();
		mainForm->label2->DisableControl();
	}*/
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR args, INT ncmd)
{
	mainForm = new MainForm;

	Framework::SetFreeTimeFunction(FreeTimeProcess);
	Framework::ProcessMessages();

	delete mainForm;
	return 0;
}