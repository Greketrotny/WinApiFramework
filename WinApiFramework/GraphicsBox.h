#ifndef GRAPHICSBOX_H
#define GRAPHICSBOX_H

#include "base_window.h"

// Direct 2D
#pragma comment(lib, "d2d1")
#include <d2d1.h>
#include <d2d1helper.h>
#include <d2d1effects.h>

// DWrite factory for fonts
#pragma comment(lib, "Dwrite")
#include "dwrite.h"

// for Graphics
#include "Color.h"
#include "Bitmap.h"
#include "Point.h"
#include "Rect.h"


#include "atlbase.h"
#include <memory>
#include <vector>
#include <string>

namespace G = Graphics;

namespace WinapiFramework
{
	class GraphicsBox;
	template <> struct ConStruct<GraphicsBox>;

	class GraphicsBox 
		: public BaseWindow
		, public HasWindowProcedure<GraphicsBox>
	{
	public:
		class GBGraphics;
	public:
		enum class FontWeight
		{
			Thin = DWRITE_FONT_WEIGHT_THIN,
			ExtraLight = DWRITE_FONT_WEIGHT_LIGHT,
			UltraLight = DWRITE_FONT_WEIGHT_ULTRA_LIGHT,
			Light = DWRITE_FONT_WEIGHT_LIGHT,
			SemiLight = DWRITE_FONT_WEIGHT_SEMI_LIGHT,
			Normal = DWRITE_FONT_WEIGHT_NORMAL,
			Medium = DWRITE_FONT_WEIGHT_MEDIUM,
			SemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD,
			Bold = DWRITE_FONT_WEIGHT_BOLD,
			ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			UltraBold = DWRITE_FONT_WEIGHT_ULTRA_BOLD,
			Heavy = DWRITE_FONT_WEIGHT_HEAVY,
			UltraHeavy = DWRITE_FONT_WEIGHT_ULTRA_BLACK
		};
		enum class FontStyle
		{
			Normal = DWRITE_FONT_STYLE_NORMAL,
			Italic = DWRITE_FONT_STYLE_ITALIC,
			Oblique = DWRITE_FONT_STYLE_OBLIQUE
		};
		enum class FontStretch
		{
			UltraCondensed = DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
			ExtraCondensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
			Condensed = DWRITE_FONT_STRETCH_CONDENSED,
			SemiCondensed = DWRITE_FONT_STRETCH_SEMI_CONDENSED,
			Normal = DWRITE_FONT_STRETCH_NORMAL,
			SemiExpanded = DWRITE_FONT_STRETCH_SEMI_EXPANDED,
			Expanded = DWRITE_FONT_STRETCH_EXPANDED,
			ExtraExpanded = DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			UltraExpanded = DWRITE_FONT_STRETCH_ULTRA_EXPANDED
		};
		enum class TextAlignment
		{
			Left = DWRITE_TEXT_ALIGNMENT_LEADING,
			Right = DWRITE_TEXT_ALIGNMENT_TRAILING,
			Center = DWRITE_TEXT_ALIGNMENT_CENTER,
			Justified = DWRITE_TEXT_ALIGNMENT_JUSTIFIED
		};
		enum class ParagraphAlignment
		{
			Top = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
			Bottom = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
			Center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER
		};
		enum class WrapMode
		{
			Wrap = DWRITE_WORD_WRAPPING_WRAP,
			NoWrap = DWRITE_WORD_WRAPPING_NO_WRAP,
			WholeWorld = DWRITE_WORD_WRAPPING_WHOLE_WORD
		};
		enum class FlowDirection
		{
			TopToBottom = DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM,
			BottomToTop = DWRITE_FLOW_DIRECTION_BOTTOM_TO_TOP
		};
		enum class ReadingDirection
		{
			LeftToRight = DWRITE_READING_DIRECTION_LEFT_TO_RIGHT,
			RightToLeft = DWRITE_READING_DIRECTION_RIGHT_TO_LEFT
		};
		enum class LineSpacingMethod
		{
			Proportional = DWRITE_LINE_SPACING_METHOD_PROPORTIONAL,
			Uniform = DWRITE_LINE_SPACING_METHOD_UNIFORM
		};

		enum class RenderType
		{
			Default = 0,
			Software = 1,
			Hardware = 2
		};
		enum class PresentOption
		{
			WaitForDisplay = 0,
			RenderImmediately = 1,
			RetainContents = 2
		};
		enum class InterpolationMode
		{
			Linear = 0,
			NearestNeighbor = 1
		};
		enum class BrushType
		{
			Solid = 0,
			LinearGradient = 1,
			RadialGradient = 2
		};

		struct TextFormatDescription
		{
			FontWeight fontWeight = FontWeight::Normal;
			FontStyle fontStyle = FontStyle::Normal;
			FontStretch fontStretch = FontStretch::Normal;
			TextAlignment textAlignment = TextAlignment::Left;
			ParagraphAlignment paragraphAlignment = ParagraphAlignment::Top;
			WrapMode wrapMode = WrapMode::Wrap;
			FlowDirection flowDirection = FlowDirection::TopToBottom;
			ReadingDirection readingDirection = ReadingDirection::LeftToRight;
			LineSpacingMethod lineSpacingMethod = LineSpacingMethod::Proportional;

			std::wstring fontName = L"Arial";
			float fontSize = 15.0f;
			float lineSpacing = 1.0f;
			float baseLine = 1.0f;
			float tabStopWidth = 20.0f;

			TextFormatDescription(
				const std::wstring& fontName = L"Arial",
				float fontSize = 15.0f,
				FontWeight fontWeight = FontWeight::Normal,
				FontStyle fontStyle = FontStyle::Normal,
				FontStretch fontStretch = FontStretch::Normal,
				TextAlignment textAlignment = TextAlignment::Left,
				ParagraphAlignment paragraphAlignment = ParagraphAlignment::Top,
				WrapMode wrapMode = WrapMode::Wrap,
				FlowDirection flowDirection = FlowDirection::TopToBottom,
				ReadingDirection readingDirection = ReadingDirection::LeftToRight,
				LineSpacingMethod lineSpacingMethod = LineSpacingMethod::Proportional,
				float lineSpacing = 1.0f,
				float baseLine = 1.0f,
				float tabStopWidth = 20.0f)
				: fontName(fontName)
				, fontSize(fontSize)
				, fontWeight(fontWeight)
				, fontStyle(fontStyle)
				, fontStretch(fontStretch)
				, textAlignment(textAlignment)
				, paragraphAlignment(paragraphAlignment)
				, wrapMode(wrapMode)
				, flowDirection(flowDirection)
				, readingDirection(readingDirection)
				, lineSpacingMethod(lineSpacingMethod)
				, lineSpacing(lineSpacing)
				, baseLine(baseLine)
				, tabStopWidth(tabStopWidth)
			{
			}
			~TextFormatDescription()
			{
			}
		};
		struct TextFormat
		{
			// -- TextFormat::fields -- //
		private:
			CComPtr<IDWriteTextFormat> writeTextFormat = nullptr;


			// -- TextFormat::constructor -- //
		public:
			TextFormat(TextFormatDescription formatDesc, CComPtr<IDWriteFactory>& dWriteFactory)
			{
				// [>] Validate formatDesc
				if (formatDesc.baseLine < 0.0f) formatDesc.baseLine = 0.0f;
				if (formatDesc.fontName.empty()) formatDesc.fontName = L"Arial";
				if (formatDesc.fontSize < 0.1f) formatDesc.fontSize = 0.1f;
				if (formatDesc.lineSpacing < 0.0f) formatDesc.lineSpacing = 0.0f;
				if (formatDesc.tabStopWidth < 0.0f) formatDesc.tabStopWidth = 0.0f;


				// [>] Create TextFormat on WriteFactory
				dWriteFactory->CreateTextFormat(
					formatDesc.fontName.c_str(),
					NULL,
					(DWRITE_FONT_WEIGHT)formatDesc.fontWeight,
					(DWRITE_FONT_STYLE)formatDesc.fontStyle,
					(DWRITE_FONT_STRETCH)formatDesc.fontStretch,
					formatDesc.fontSize,
					L"en-us",
					&(writeTextFormat.p));

				writeTextFormat->SetFlowDirection((DWRITE_FLOW_DIRECTION)formatDesc.flowDirection);
				writeTextFormat->SetIncrementalTabStop(formatDesc.tabStopWidth);
				writeTextFormat->SetLineSpacing((DWRITE_LINE_SPACING_METHOD)formatDesc.lineSpacingMethod, formatDesc.lineSpacing, formatDesc.baseLine);
				writeTextFormat->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)formatDesc.paragraphAlignment);
				writeTextFormat->SetReadingDirection((DWRITE_READING_DIRECTION)formatDesc.readingDirection);
				writeTextFormat->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)formatDesc.textAlignment);
				writeTextFormat->SetWordWrapping((DWRITE_WORD_WRAPPING)formatDesc.wrapMode);
			}
		public:
			~TextFormat()
			{
			}


			// -- TextFormat::frields -- //
		private:
			friend class GraphicsBox::GBGraphics;
		};
	public:
		class GBGraphics
		{	
		private:
			GraphicsBox *const mp_control;
			unsigned int m_width = 0u, m_height = 0u;
			const RenderType m_renderType;
			const PresentOption m_presentOption;
			InterpolationMode m_interpolationMode;
		private:
			std::unique_ptr<TextFormat> defaultTextFormat = nullptr;
			std::vector<std::unique_ptr<TextFormat>> customTextFormats;

			// directX 2D
			CComPtr<ID2D1Factory> m_pD2DFactory = nullptr;
			CComPtr<ID2D1HwndRenderTarget> m_pHwndRenderTarget = nullptr;
			ID2D1Brush **m_ppBrush = nullptr;
			CComPtr<ID2D1SolidColorBrush> m_pSolidBrush = nullptr;
			CComPtr<ID2D1LinearGradientBrush> m_pLinearGradientBrush = nullptr;
			CComPtr<ID2D1RadialGradientBrush> m_pRadialGradientBrush = nullptr;

			// DirectWrite
			CComPtr<IDWriteFactory> m_pDWriteFactory = nullptr;
			CComPtr<IDWriteFontCollection> m_pFontCollection = nullptr;
		public:
			struct ConStruct
			{
				RenderType renderType;
				PresentOption presentOption;
				InterpolationMode interpolationMode;
				TextFormatDescription defaultTextFormatDesc;

				ConStruct(
					RenderType renderType = RenderType::Default,
					PresentOption presentOption = PresentOption::WaitForDisplay,
					InterpolationMode interpolationMode = InterpolationMode::Linear,
					TextFormatDescription defaultTextFormatDesc = TextFormatDescription())
					: renderType(renderType)
					, presentOption(presentOption)
					, interpolationMode(interpolationMode)
					, defaultTextFormatDesc(defaultTextFormatDesc)
				{
				}
			};


		private:
			GBGraphics(const GBGraphics& graphics) = delete;
			GBGraphics(GBGraphics&& graphics) = delete;
			GBGraphics(GraphicsBox *control, const ConStruct& config);
			~GBGraphics();


		private:
			bool InitGraphics(const GBGraphics::ConStruct& conStruct);
			template <class T> void SafeRelease(T **ppT)
			{
				if (*ppT)
				{
					(*ppT)->Release();
					*ppT = NULL;
				}
			}
			void Resize(unsigned int newWidth, unsigned int newHeight);
		public:
			// TextFormat methods
			void SetDefaultTextFormat(TextFormatDescription textFormatDesc);
			const TextFormat* const CreateTextFormat(TextFormatDescription textFormatDesc);
			void DestroytextFormat(const TextFormat*& textFormat);

			void SetInterpolationMode(InterpolationMode newInterpolationMode);

			// brush methods
			void ChangeActiveBrush(BrushType newBrushType);
			void SetSolidBrush(const G::Color& brushColor, const float opacity = 1.0f);

			void BeginDraw();
			void EndDraw();
			void Clear(const G::Color& color);

			// drawing text
			void DrawString(std::wstring text, G::Rect<float> rect);
			void DrawString(std::wstring text, G::Rect<float> rect, const TextFormat* textFormat);

			void DrawBitmap(
				const G::Bitmap& bitmap,
				const G::Rect<float>& destinationRect,
				const G::Rect<float>& sourceRect,
				const float opacity = 1.0f,
				InterpolationMode interpolationMode = InterpolationMode::Linear);
			void DrawBitmap(
				const G::Bitmap& bitmap,
				const float opacity = 1.0f,
				InterpolationMode interpolationMode = InterpolationMode::Linear);
			void DrawLine(
				const G::Point<float>& point1,
				const G::Point<float>& point2,
				const float& stroke = 1.0f);
			void DrawEllipse(const G::Point<float>& center, const G::Point<float>& size, const float& stroke = 1.0f);
			void DrawRectangle(const G::Point<float>& topLeft, const G::Point<float>& bottomRight, const float& stroke = 1.0f);
			void DrawRoundedRectangle(
				const G::Point<float>& topLeft,
				const G::Point<float>& bottomRight,
				const float& radiousX, const float& radiousY,
				const float& stroke = 1.0f);

			void FillEllipse(const G::Point<float>& center, const G::Point<float>& size);
			void FillRectangle(const G::Point<float>& topLeft, const G::Point<float>& bottomRight);
			void FillRoundedRectangle(
				const G::Point<float>& topLeft,
				const G::Point<float>& bottomRight,
				const float& radiousX, const float& radiousY);


		public:
			const unsigned int& Width;
			const unsigned int& Height;


			friend class GraphicsBox;
		};
	private:
		GBGraphics m_graphics;

	public:
		struct Events : public BaseWindowEvents {};


	private:
		GraphicsBox(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox(const GraphicsBox&& graphicsBox) = delete;
		GraphicsBox(ParentWindow* parent, const ConStruct<GraphicsBox> &conStruct);
		~GraphicsBox();


	public:
		GraphicsBox& operator=(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox& operator=(const GraphicsBox&& graphicsBox) = delete;


	private:
		LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void Resize(int newWidth, int newHeight) override;


	public:
		GBGraphics& Gfx;


		friend class ObjectCreator;
	};

	template <> struct ConStruct<GraphicsBox>
	{
		Rect rect;
		GraphicsBox::GBGraphics::ConStruct graphics;

		ConStruct(const Rect& rect,
			GraphicsBox::GBGraphics::ConStruct graphicsConStruct = GraphicsBox::GBGraphics::ConStruct())
			: rect(rect)
			, graphics(graphicsConStruct)
		{}
	};
}

#endif // !GRAPHICS_BOX_H

