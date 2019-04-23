#include "GraphicsBox.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

// [CLASS] GraphicsBox -------------------------|
// -- constructors -- //
GraphicsBox::GraphicsBox(const GraphicsBox::Config &config)
	:WindowControl(config),
	graphics(this),
	Gfx(graphics)
{

}
GraphicsBox::~GraphicsBox()
{

}

// -- methods -- //
// private:
int GraphicsBox::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT msg = HIWORD(wParam);
	switch (msg)
	{
	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool GraphicsBox::CreateControlWindow()
{
	controlStyle |= WS_BORDER;

	// create window
	hControl = CreateWindow(L"STATIC", NULL,
		controlStyle,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"GraphicsBox creation failed.", L"GraphicsBox create error", MB_OK | MB_ICONERROR);
		return false;
	}

	if (!graphics.InitGraphics())
	{
		MessageBox(nullptr, L"GraphicsBox::Graphics initialization failed.", L"Graphics error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
// public:
void GraphicsBox::Resize(unsigned int newWidth, unsigned int newHeight)
{
	graphics.Resize(newWidth, newHeight);
	WindowControl::Resize(newWidth, newHeight);
}
// [CLASS] GraphicsBox -------------------------|



// [CLASS] GraphicsBox::Graphics ---------------|
// -- constructors -- //
GraphicsBox::Graphics::Graphics(GraphicsBox *control)
{
	this->control = control;
}
GraphicsBox::Graphics::~Graphics()
{
	SafeRelease(&brush);
	SafeRelease(&pixelBitmap);
	SafeRelease(&compBitmap);
	SafeRelease(&RT);
	SafeRelease(&BRT);
	SafeRelease(&D2DFactory);

	if (pixelMap) delete pixelMap;
}

// -- methods -- //
// private:
bool GraphicsBox::Graphics::InitGraphics()
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

	// create bitmap for pixelMap
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
		&pixelBitmap
	);

	// ------------------
	RT->CreateCompatibleRenderTarget(&BRT);
	BRT->CreateSolidColorBrush
	(
		D2D1::ColorF(0x00FF88, 1.0f),
		&brush
	);

	return true;
}
void GraphicsBox::Graphics::Resize(unsigned int newWidth, unsigned int newHeight)
{
	// set new dimensions
	width = newWidth;
	height = newHeight;

	// resize pixelMap
	pixelMap->Resize(width, height);
	RT->Resize(D2D1::SizeU(width, height));

	// resize pixelBitmap
	SafeRelease(&pixelBitmap);
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
		&pixelBitmap
	);

	SafeRelease(&BRT);
	RT->CreateCompatibleRenderTarget(&BRT);
}
// public:
void GraphicsBox::Graphics::Render()
{
	// copy data from pixelMap to bitmap
	pixelBitmap->CopyFromMemory
	(
		&D2D1::RectU(0, 0, width, height),
		pixelMap->GetMapAddress(),
		width * 4
	);

	// render bitmap from pixelMap
	RT->BeginDraw();
	RT->DrawBitmap
	(
		pixelBitmap,
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
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&D2D1::RectF(0.0f, 0.0f, (float)width, (float)height)
	);
	RT->EndDraw();
}
void GraphicsBox::Graphics::SetPixel(const unsigned int& x, const unsigned int& y, const unsigned int& colorValue)
{
	pixelMap->SetPixel(x, y, colorValue);
}
void GraphicsBox::Graphics::SetPixel(const unsigned int& x, const unsigned int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	pixelMap->SetPixel(x, y, 0xFF000000 | (r << 16) | (g << 8) | b);
}
void GraphicsBox::Graphics::SetPixel(const unsigned int& x, const unsigned int& y, const G::Color& color)
{
	pixelMap->SetPixel(x, y, color.GetColor());
}
void GraphicsBox::Graphics::ClearPixelMap(const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	pixelMap->Clear(0xFF000000 | (r << 16) | (g << 8) | b);
}
void GraphicsBox::Graphics::ClearPixelMap(const G::Color& color)
{
	pixelMap->Clear(color.GetColor());
}
void GraphicsBox::Graphics::ClearGraphicsMap(const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	BRT->BeginDraw();
	BRT->Clear(D2D1::ColorF((r << 16) | (g << 8) | b, 0.0f));
	BRT->EndDraw();
}
void GraphicsBox::Graphics::ClearGraphicsMap(const G::Color& color)
{
	BRT->BeginDraw();
	BRT->Clear(D2D1::ColorF(color.GetColor(), 0.0f));
	BRT->EndDraw();
}

void GraphicsBox::Graphics::SetBrushColor(const G::Color& color)
{
	brush->SetColor(D2D1::ColorF(color.GetColor(), 1.0f));
}
void GraphicsBox::Graphics::DrawLine(const float& x1, const float& y1, const float& x2, const float& y2, const float& width)
{
	BRT->BeginDraw();
	BRT->DrawLine
	(
		D2D1::Point2F(x1, y1),
		D2D1::Point2F(x2, y2),
		brush,
		width
	);
	BRT->EndDraw();
}
void GraphicsBox::Graphics::DrawEllipse(const float& x, const float& y, const float& a, const float& b, const float& width)
{
	BRT->BeginDraw();
	BRT->DrawEllipse
	(
		D2D1::Ellipse(D2D1::Point2F(x, y), a, b),
		brush,
		width
	);
	BRT->EndDraw();
}
void GraphicsBox::Graphics::FillEllipse(const float& x, const float& y, const float& a, const float& b)
{
	BRT->BeginDraw();
	BRT->FillEllipse
	(
		D2D1::Ellipse(D2D1::Point2F(x, y), a, b),
		brush
	);
	BRT->EndDraw();
}
// [CLASS] GraphicsBox::Graphics ---------------|