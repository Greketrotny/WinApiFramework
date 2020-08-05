#include "graphicsbox.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] GraphicsBox ~~~~~~~~
	GraphicsBox::GraphicsBox(ParentWindow* parent, const ConStruct<GraphicsBox>& conStruct)
		: BaseWindow(parent)
		, HasWindowProcedure(this, &GraphicsBox::WindowProcedure)
		, m_graphics(this, conStruct.graphics)
		, Gfx(m_graphics)
	{
		m_rect = conStruct.rect;
		m_window_class_name = mp_parent->GetWindowClassName() + L"GraphicsBoxClass" + std::to_wstring(int(this));

		CreateWinapiWindow();

		Gfx.InitGraphics(conStruct.graphics);
	}
	GraphicsBox::~GraphicsBox()
	{
		DestroyWinapiWindow();
	}

	LRESULT GraphicsBox::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (HandleMouseEvent(msg, wParam, lParam) == 0) return 0;

		switch (msg)
		{
			default: return 1;
		}

		return 0;
	}
	LRESULT GraphicsBox::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT msg = HIWORD(wParam);
		switch (msg)
		{
			default:
				return 1;
		}
		return 0;
	}
	bool GraphicsBox::CreateWinapiWindow()
	{
		// [>] Create WindowClassEx
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.hInstance = Framework::GetProgramInstance();
		wc.lpfnWndProc = GetFrameworkProcedure();
		wc.lpszClassName = m_window_class_name.c_str();
		wc.lpszMenuName = nullptr;
		wc.cbSize = (sizeof(WNDCLASSEX));
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.style = 0;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_DESKTOP);

		// register class
		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"Register graphics box class failed.", L"Register error", MB_OK | MB_ICONERROR);
			return false;
		}

		m_window_style |= WS_CHILD | WS_VISIBLE | WS_BORDER;

		// create window
		m_hWindow = CreateWindowW((LPCWSTR)m_window_class_name.c_str(), NULL,
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::GetProgramInstance(), nullptr);

		// check control creation
		if (!m_hWindow)
		{
			MessageBox(nullptr, L"GraphicsBox creation failed.", L"GraphicsBox create error", MB_OK | MB_ICONERROR);
			return false;
		}

		// set pointer to Window class to receive messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_window_procedure);

		return true;
	}
	void GraphicsBox::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
		UnregisterClass(m_window_class_name.c_str(), Framework::GetProgramInstance());
	}

	void GraphicsBox::Resize(int newWidth, int newHeight)
	{
		m_graphics.Resize(newWidth - 2, newHeight - 2);
		DoResize(newWidth, newHeight);

		RaiseEventByHandler<Events::EventResize>();
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] GBGraphics ~~~~~~~~
	GraphicsBox::GBGraphics::GBGraphics(GraphicsBox *control, const GraphicsBox::GBGraphics::ConStruct& conStruct)
		: Width(m_width)
		, Height(m_height)
		, mp_control(control)
		, m_renderType(conStruct.renderType)
		, m_presentOption(conStruct.presentOption)
		, m_interpolationMode(conStruct.interpolationMode)
	{
	}
	GraphicsBox::GBGraphics::~GBGraphics()
	{
	}

	bool GraphicsBox::GBGraphics::InitGraphics(const GBGraphics::ConStruct& conStruct)
	{
		m_width = mp_control->m_rect.size.width - 2;
		m_height = mp_control->m_rect.size.height - 2;


		// [>] Create D2D1Factory
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&m_pD2DFactory);


		// [>] Create render target
		// set render target type
		D2D1_RENDER_TARGET_TYPE renderTargetType;
		switch (m_renderType)
		{
			case WinapiFramework::GraphicsBox::RenderType::Default:
				renderTargetType = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT;
				break;
			case WinapiFramework::GraphicsBox::RenderType::Software:
				renderTargetType = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_SOFTWARE;
				break;
			case WinapiFramework::GraphicsBox::RenderType::Hardware:
				renderTargetType = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE;
				break;
		}

		// set presentation options
		D2D1_PRESENT_OPTIONS presentOptions;
		switch (m_presentOption)
		{
			case WinapiFramework::GraphicsBox::PresentOption::WaitForDisplay:
				presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_NONE;
				break;
			case WinapiFramework::GraphicsBox::PresentOption::RenderImmediately:
				presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_IMMEDIATELY;
				break;
			case WinapiFramework::GraphicsBox::PresentOption::RetainContents:
				presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS;
				break;
		}

		// create render target object
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
				mp_control->m_hWindow,
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

		// get system font collection
		m_pDWriteFactory->GetSystemFontCollection(&m_pFontCollection);

		// create and setup defaultTextFormat
		SetDefaultTextFormat(conStruct.defaultTextFormatDesc);

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

	void GraphicsBox::GBGraphics::SetDefaultTextFormat(GraphicsBox::TextFormatDescription textFormatDesc)
	{
		defaultTextFormat.reset(new GraphicsBox::TextFormat(textFormatDesc, this->m_pDWriteFactory));
	}
	const GraphicsBox::TextFormat* const GraphicsBox::GBGraphics::CreateTextFormat(GraphicsBox::TextFormatDescription textFormatDesc)
	{
		customTextFormats.push_back(std::make_unique<GraphicsBox::TextFormat>(textFormatDesc, this->m_pDWriteFactory));
		return customTextFormats.back().get();
	}
	void GraphicsBox::GBGraphics::DestroytextFormat(const GraphicsBox::TextFormat*& textFormat)
	{
		for (size_t i = 0; i < customTextFormats.size(); ++i)
		{
			if (customTextFormats[i].get() == textFormat)
			{
				customTextFormats.erase(customTextFormats.begin() + i);
			}
		}
		textFormat = nullptr;
	}

	void GraphicsBox::GBGraphics::SetInterpolationMode(GraphicsBox::InterpolationMode newInterpolationMode)
	{
		m_interpolationMode = newInterpolationMode;
	}

	void GraphicsBox::GBGraphics::ChangeActiveBrush(BrushType newBrushType)
	{
		switch (newBrushType)
		{
			case GraphicsBox::BrushType::Solid:
				m_ppBrush = (ID2D1Brush**)&(*m_pSolidBrush);
				break;
			case GraphicsBox::BrushType::LinearGradient:
				m_ppBrush = (ID2D1Brush**)&(*m_pLinearGradientBrush);
				break;
			case GraphicsBox::BrushType::RadialGradient:
				m_ppBrush = (ID2D1Brush**)&(*m_pRadialGradientBrush);
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
			this->defaultTextFormat->writeTextFormat,
			D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom),
			m_pSolidBrush);
	}
	void GraphicsBox::GBGraphics::DrawString(std::wstring text, G::Rect<float> rect, const TextFormat* textFormat)
	{
		if (textFormat == nullptr)
			return;

		m_pHwndRenderTarget->DrawText(
			text.c_str(),
			text.size(),
			textFormat->writeTextFormat,
			D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom),
			m_pSolidBrush);
	}

	void GraphicsBox::GBGraphics::DrawBitmap(
		const G::Bitmap& bitmap,
		const G::Rect<float>& destinationRect,
		const G::Rect<float>& sourceRect,
		const float opacity,
		GraphicsBox::InterpolationMode interpolationMode)
	{
		// [>] Create bitmap
		CComPtr<ID2D1Bitmap> pBitmap;
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
			case GraphicsBox::InterpolationMode::Linear:
				interpMode = D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
				break;
			case GraphicsBox::InterpolationMode::NearestNeighbor:
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
		GraphicsBox::InterpolationMode interpolationMode)
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
}