#include "WinApiFramework.h"

namespace WAF = WinApiFramework;


class MainForm
{
public:
	WAF::Window *MainWindow = nullptr;
	//WAF::GraphicsBox *GfxBox = nullptr;

public:
	MainForm()
	{
		WAF::Window::Config wc;
		wc.rect.width = 1000;
		wc.rect.height = 600;
		wc.caption = L"WinApiFramework Test";
		wc.sizeRect.minWidth = 400;
		wc.sizeRect.minHeight = 200;
		wc.startStyle = WAF::Window::StartStyle::Normal;
		MainWindow = new WAF::Window(wc, new MainWindowEH(this));

		/*WAF::GraphicsBox::Config gbc;
		gbc.rect.x = 10;
		gbc.rect.y = 10;
		gbc.rect.width = wc.rect.width - 20;
		gbc.rect.height = wc.rect.height - 20;
		GfxBox = new WAF::GraphicsBox(gbc);
		MainWindow->AddControl(GfxBox);*/
	}
	~MainForm()
	{
		//delete GfxBox;

		delete MainWindow;
	}

	// EventHandlers
	struct MainWindowEH : public WAF::Window::EventHandler
	{
		MainForm *MF;
		MainWindowEH(MainForm *mf)
		{
			MF = mf;
		}


		void HandleEvent(WAF::Window::Event event) override
		{
			switch (event.type)
			{
				case WAF::Window::Event::Type::Resize:
					//MF->GfxBox->Resize(MF->MainWindow->ClientWidth - 20, MF->MainWindow->ClientHeight - 20);
					MF->MainWindow->SetCaption(std::to_wstring(MF->MainWindow->GetWindowWidth()) + L":" + std::to_wstring(MF->MainWindow->GetWindowHeight()));
					break;
				case WAF::Window::Event::Type::Move:
					MF->MainWindow->SetCaption(std::to_wstring(MF->MainWindow->GetWindowX()) + L":" + std::to_wstring(MF->MainWindow->GetWindowY()));
					break;
			}
		}
	};
};
MainForm *MF;

void FreeTimeFunction()
{
	//static int counter = 0u;
	//int mouseX = MF->GfxBox->GetMouseX();
	//int mouseY = MF->GfxBox->GetMouseY();

	//for (unsigned int i = 0u; i < MF->GfxBox->Gfx.Width; ++i)
	//{
	//	for (unsigned int j = 0u; j < MF->GfxBox->Gfx.Height; ++j)
	//	{
	//		MF->GfxBox->Gfx.SetPixel(i, j, i + counter, j + counter, i * j);
	//	}
	//}
	//counter += 1;
	////MF->GfxBox->Gfx.Clear(255u, 0u, 0u);
	////MF->GfxBox->Gfx.FillEllipse(Graphics::Point2D<float>(mouseX, mouseY), Graphics::Point2D<float>(20.0, 20.0f));

	//MF->GfxBox->Gfx.Render();
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR args, INT ncmd)
{
	MF = new MainForm();

	WAF::Framework::SetFreeTimeFunction(FreeTimeFunction);
	WAF::Framework::ProcessMessages();

	delete MF;
	return 0;
}