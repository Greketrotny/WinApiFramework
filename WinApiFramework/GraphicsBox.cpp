#include "Precompiled.h"
#include "GraphicsBox.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;

// ~~~~~~~~ [CLASS] GraphicsBox ~~~~~~~~
// -- GraphicsBox::constructors -- //
GraphicsBox::GraphicsBox(const GraphicsBox::Config &config)
	:WindowControl(config),
	graphics(this, config.graphicsConfiguration),
	Gfx(graphics),
	Events(events)
{
}
GraphicsBox::~GraphicsBox()
{

}

// -- GraphicsBox::methods -- //
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
	hControl = CreateWindowW(L"STATIC", NULL,
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
		MessageBox(nullptr, L"GraphicsBox::GBGraphics initialization failed.", L"GBGraphics error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
// public:
void GraphicsBox::Resize(unsigned int newWidth, unsigned int newHeight)
{
	graphics.Resize(newWidth - 2, newHeight - 2);
	WindowControl::Resize(newWidth, newHeight);
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~ [CLASS] GBGraphics ~~~~~~~~
// -- GraphicsBox::GBGraphics::constructors -- //
GraphicsBox::GBGraphics::GBGraphics(GraphicsBox *control, const GraphicsBox::GBGraphics::Configuration& config)
	: Width(m_width)
	, Height(m_height)
	, m_pControl(control)
	, m_renderType(config.renderType)
	, m_presentOption(config.presentOption)
	, m_interpolationMode(config.interpolationMode)
{
}
GraphicsBox::GBGraphics::~GBGraphics()
{
	// release brushes
	SafeRelease(&m_pSolidBrush);
	SafeRelease(&m_pLinearGradientBrush);
	SafeRelease(&m_pRadialGradientBrush);

	// release render target
	SafeRelease(&m_pHwndRenderTarget);

	// release factory
	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pDWriteFactory);
}

// -- GraphicsBox::GBGraphics::methods -- //
// private:
bool GraphicsBox::GBGraphics::InitGraphics()
{
	m_width = m_pControl->rect.width - 2;
	m_height = m_pControl->rect.height - 2;
	

	// [>] Create D2D1Factory
	D2D1CreateFactory(
		D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory);

	// [>] Create render target
	D2D1_RENDER_TARGET_TYPE renderTargetType;
	switch (m_renderType)
	{
		case WinApiFramework::GraphicsBox::GBGraphics::RenderTypeDefault:
			renderTargetType = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT;
			break;
		case WinApiFramework::GraphicsBox::GBGraphics::RenderTypeSoftware:
			renderTargetType = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_SOFTWARE;
			break;
		case WinApiFramework::GraphicsBox::GBGraphics::RenderTypeHardware:
			renderTargetType = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE;
			break;
	}

	D2D1_PRESENT_OPTIONS presentOptions;
	switch (m_presentOption)
	{
		case WinApiFramework::GraphicsBox::GBGraphics::PresentOptionWaitForDisplay:
			presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_NONE;
			break;
		case WinApiFramework::GraphicsBox::GBGraphics::PresentOptionRenderImmediately:
			presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_IMMEDIATELY;
			break;
		case WinApiFramework::GraphicsBox::GBGraphics::PresentOptionRetainContents:
			presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS;
			break;
	}

	// [>] Create render target
	m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(
			renderTargetType,
			D2D1::PixelFormat(
				DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED),
			96.0f, 96.0f,
			D2D1_RENDER_TARGET_USAGE::D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL::D2D1_FEATURE_LEVEL_DEFAULT),
		D2D1::HwndRenderTargetProperties(
			m_pControl->hControl,
			D2D1::SizeU(m_width, m_height),
			presentOptions),
		&m_pHwndRenderTarget);


	// [>] Create brushes
	// solid brush
	m_ppBrush = (ID2D1Brush**)&m_pSolidBrush;
	m_pHwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF((0x00 << 16) | (0x00 << 8) | (0x00), 1.0f), &m_pSolidBrush);
	// TODO: linear gradient brush

	// TODO: radial gradient brush


	// [>] Initialize text writing objects
	// create direct write factory
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory));

	// create and setup text format
	m_pDWriteFactory->CreateTextFormat(
		L"Verdana",
		NULL,
		DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
		15.0f,
		L"en-us",
		&m_pTextFormat);

	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_WRAP);

	return true;
}
void GraphicsBox::GBGraphics::Resize(unsigned int newWidth, unsigned int newHeight)
{
	// set new dimensions
	m_width = newWidth;
	m_height = newHeight;

	// resize pixelMap
	m_pHwndRenderTarget->Resize(D2D1::SizeU(m_width, m_height));
}
// public:
void GraphicsBox::GBGraphics::SetInterpolationMode(GBGraphics::InterpolationMode newInterpolationMode)
{
	m_interpolationMode = newInterpolationMode;
}

void GraphicsBox::GBGraphics::ChangeActiveBrush(BrushType newBrushType)
{
	switch (newBrushType)
	{
		case GraphicsBox::GBGraphics::BrushTypeSolid:
			m_ppBrush = (ID2D1Brush**)m_pSolidBrush;
			break;
		case GraphicsBox::GBGraphics::BrushTypeLinearGradient:
			m_ppBrush = (ID2D1Brush**)m_pLinearGradientBrush;
			break;
		case GraphicsBox::GBGraphics::BrushTypeRadialGradient:
			m_ppBrush = (ID2D1Brush**)m_pRadialGradientBrush;
			break;
		default:
			break;
	}
}
void GraphicsBox::GBGraphics::SetSolidBrush(const G::Color& brushColor, const float opacity)
{
	m_pSolidBrush->SetColor(D2D1::ColorF((brushColor.GetR() << 16) | (brushColor.GetG() << 8) | (brushColor.GetB()), brushColor.GetA()));
	m_pSolidBrush->SetOpacity(opacity);
}

void GraphicsBox::GBGraphics::BeginDraw()
{
	m_pHwndRenderTarget->BeginDraw();
}
void GraphicsBox::GBGraphics::EndDraw()
{
	m_pHwndRenderTarget->EndDraw();
}
void GraphicsBox::GBGraphics::Clear(const G::Color& color)
{
	m_pHwndRenderTarget->Clear(D2D1::ColorF((color.GetR() << 16) | (color.GetG() << 8) | color.GetB(), 1.0f));
}

void GraphicsBox::GBGraphics::DrawString(std::wstring text, G::Rect<float> rect)
{
	m_pHwndRenderTarget->DrawText(
		text.c_str(),
		text.size(),
		m_pTextFormat,
		D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom),
		m_pSolidBrush);
}

void GraphicsBox::GBGraphics::DrawBitmap(
	const G::Bitmap& bitmap,
	const G::Rect<float>& destinationRect,
	const G::Rect<float>& sourceRect,
	const float opacity,
	GBGraphics::InterpolationMode interpolationMode)
{
	// [>] Create bitmap
	ID2D1Bitmap *pBitmap;
	HRESULT hr = m_pHwndRenderTarget->CreateBitmap(
		D2D1::SizeU(bitmap.Width, bitmap.Height),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(
				DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED)),
		&pBitmap);


	// [>] Copy G::bitmap to pBitmap
	pBitmap->CopyFromMemory(
		&D2D1::RectU(0, 0, bitmap.Width, bitmap.Height),
		bitmap.GetMapAddress(),
		bitmap.Width * sizeof(*bitmap.GetMapAddress()));


	// [>] create bitmap drawing configuration
	// set interpolation mode
	D2D1_BITMAP_INTERPOLATION_MODE interpMode;
	switch (interpolationMode)
	{
		case GraphicsBox::GBGraphics::InterpolationModeLinear:
			interpMode = D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
			break;
		case GraphicsBox::GBGraphics::InterpolationModeNearestNeighbor:
			interpMode = D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
			break;
	}


	// [>] Draw bitmap
	m_pHwndRenderTarget->DrawBitmap(
		pBitmap,
		D2D1::RectF(destinationRect.left, destinationRect.top, destinationRect.right, destinationRect.bottom),
		opacity,
		interpMode,
		D2D1::RectF(sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom));
}
void GraphicsBox::GBGraphics::DrawBitmap(
	const G::Bitmap& bitmap,
	const float opacity,
	GBGraphics::InterpolationMode interpolationMode)
{
	DrawBitmap(
		bitmap,
		G::Rect<float>(0.0f, 0.0f, static_cast<float>(this->m_width), static_cast<float>(this->m_height)),
		G::Rect<float>(0.0f, 0.0f, static_cast<float>(this->m_width), static_cast<float>(this->m_height)),
		opacity,
		interpolationMode);
}
void GraphicsBox::GBGraphics::DrawLine(
	const G::Point<float>& p0, 
	const G::Point<float>& p1, 
	const float& stroke)
{
	m_pHwndRenderTarget->DrawLine
	(
		D2D1::Point2F(p0.x, p0.y),
		D2D1::Point2F(p1.x, p1.y),
		*m_ppBrush,
		stroke);
}
void GraphicsBox::GBGraphics::DrawEllipse(const G::Point<float>& center, const G::Point<float>& size, const float& stroke)
{
	m_pHwndRenderTarget->DrawEllipse(
		D2D1::Ellipse(D2D1::Point2F(center.x, center.y), size.x, size.y),
		*m_ppBrush,
		stroke);
}
void GraphicsBox::GBGraphics::DrawRectangle(const G::Point<float>& topLeft, const G::Point<float>& bottomRight, const float& stroke)
{
	m_pHwndRenderTarget->DrawRectangle(
		D2D1::RectF(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y),
		*m_ppBrush,
		stroke);
}
void GraphicsBox::GBGraphics::DrawRoundedRectangle(
	const G::Point<float>& topLeft,
	const G::Point<float>& bottomRight,
	const float& radiousX, const float& radiousY,
	const float& stroke)
{
	m_pHwndRenderTarget->DrawRoundedRectangle(
		D2D1::RoundedRect(
			D2D1::RectF(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y), 
			radiousX, radiousY), 
		*m_ppBrush, 
		stroke);
}

void GraphicsBox::GBGraphics::FillEllipse(const G::Point<float>& center, const G::Point<float>& size)
{
	m_pHwndRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(center.x, center.y), size.x, size.y),
		*m_ppBrush);
}
void GraphicsBox::GBGraphics::FillRectangle(const G::Point<float>& topLeft, const G::Point<float>& bottomRight)
{
	m_pHwndRenderTarget->FillRectangle(
		D2D1::RectF(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y),
		*m_ppBrush);
}
void GraphicsBox::GBGraphics::FillRoundedRectangle(
	const G::Point<float>& topLeft,
	const G::Point<float>& bottomRight,
	const float& radiousX, const float& radiousY)
{
	m_pHwndRenderTarget->FillRoundedRectangle(
		D2D1::RoundedRect(
			D2D1::RectF(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y),
			radiousX, radiousY),
		*m_ppBrush);
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~