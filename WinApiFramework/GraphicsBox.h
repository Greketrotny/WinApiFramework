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

	class GraphicsBox : public ChildControl
	{
	public:
		class GBGraphics;

		// -- GraphicsBox::fields -- //
	public:
		enum FontWeight
		{
			FontWeightThin = DWRITE_FONT_WEIGHT_THIN,
			FontWeightExtraLight = DWRITE_FONT_WEIGHT_LIGHT,
			FontWeightUltraLight = DWRITE_FONT_WEIGHT_ULTRA_LIGHT,
			FontWeightLight = DWRITE_FONT_WEIGHT_LIGHT,
			FontWeightSemiLight = DWRITE_FONT_WEIGHT_SEMI_LIGHT,
			FontWeightNormal = DWRITE_FONT_WEIGHT_NORMAL,
			FontWeightMedium = DWRITE_FONT_WEIGHT_MEDIUM,
			FontWeightSemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD,
			FontWeightBold = DWRITE_FONT_WEIGHT_BOLD,
			FontWeightExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			FontWeightUltraBold = DWRITE_FONT_WEIGHT_ULTRA_BOLD,
			FontWeightHeavy = DWRITE_FONT_WEIGHT_HEAVY,
			FontWeightUltraHeavy = DWRITE_FONT_WEIGHT_ULTRA_BLACK
		};
		enum FontStyle
		{
			FontStyleNormal = DWRITE_FONT_STYLE_NORMAL,
			FontStyleItalic = DWRITE_FONT_STYLE_ITALIC,
			FontStyleOblique = DWRITE_FONT_STYLE_OBLIQUE
		};
		enum FontStretch
		{
			FontStretchUltraCondensed = DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
			FontStretchExtraCondensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
			FontStretchCondensed = DWRITE_FONT_STRETCH_CONDENSED,
			FontStretchSemiCondensed = DWRITE_FONT_STRETCH_SEMI_CONDENSED,
			FontStretchNormal = DWRITE_FONT_STRETCH_NORMAL,
			FontStretchSemiExpanded = DWRITE_FONT_STRETCH_SEMI_EXPANDED,
			FontStretchExpanded = DWRITE_FONT_STRETCH_EXPANDED,
			FontStretchExtraExpanded = DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			FontStretchUltraExpanded = DWRITE_FONT_STRETCH_ULTRA_EXPANDED
		};
		enum TextAlignment
		{
			TextAlignmentLeft = DWRITE_TEXT_ALIGNMENT_LEADING,
			TextAlignmentRight = DWRITE_TEXT_ALIGNMENT_TRAILING,
			TextAlignmentCenter = DWRITE_TEXT_ALIGNMENT_CENTER,
			TextAlignmentJustified = DWRITE_TEXT_ALIGNMENT_JUSTIFIED
		};
		enum ParagraphAlignment
		{
			ParagraphAlignmentTop = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
			ParagraphAlignmentBottom = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
			ParagraphAlignmentCenter = DWRITE_PARAGRAPH_ALIGNMENT_CENTER
		};
		enum WrapMode
		{
			WrapModeWrap = DWRITE_WORD_WRAPPING_WRAP,
			WrapModeNoWrap = DWRITE_WORD_WRAPPING_NO_WRAP,
			WrapModeWholeWorld = DWRITE_WORD_WRAPPING_WHOLE_WORD
		};
		enum FlowDirection
		{
			FlowDirectionTopToBottom = DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM,
			FlowDirectionBottomToTop = DWRITE_FLOW_DIRECTION_BOTTOM_TO_TOP
		};
		enum ReadingDirection
		{
			ReadingDirectionLeftToRight = DWRITE_READING_DIRECTION_LEFT_TO_RIGHT,
			ReadingDirectionRightToLeft = DWRITE_READING_DIRECTION_RIGHT_TO_LEFT
		};
		enum LineSpacingMethod
		{
			LineSpacingMethodProportional = DWRITE_LINE_SPACING_METHOD_PROPORTIONAL,
			LineSpacingMethodUniform = DWRITE_LINE_SPACING_METHOD_UNIFORM
		};

		enum RenderType
		{
			RenderTypeDefault = 0,
			RenderTypeSoftware = 1,
			RenderTypeHardware = 2
		};
		enum PresentOption
		{
			PresentOptionWaitForDisplay = 0,
			PresentOptionRenderImmediately = 1,
			PresentOptionRetainContents = 2
		};
		enum InterpolationMode
		{
			InterpolationModeLinear = 0,
			InterpolationModeNearestNeighbor = 1
		};
		enum BrushType
		{
			BrushTypeSolid = 0,
			BrushTypeLinearGradient = 1,
			BrushTypeRadialGradient = 2
		};

		struct TextFormatDescription
		{
			FontWeight fontWeight = FontWeight::FontWeightNormal;
			FontStyle fontStyle = FontStyle::FontStyleNormal;
			FontStretch fontStretch = FontStretch::FontStretchNormal;
			TextAlignment textAlignment = TextAlignment::TextAlignmentLeft;
			ParagraphAlignment paragraphAlignment = ParagraphAlignment::ParagraphAlignmentTop;
			WrapMode wrapMode = WrapMode::WrapModeWrap;
			FlowDirection flowDirection = FlowDirection::FlowDirectionTopToBottom;
			ReadingDirection readingDirection = ReadingDirection::ReadingDirectionLeftToRight;
			LineSpacingMethod lineSpacingMethod = LineSpacingMethod::LineSpacingMethodProportional;

			std::wstring fontName = L"Arial";
			float fontSize = 15.0f;
			float lineSpacing = 1.0f;
			float baseLine = 1.0f;
			float tabStopWidth = 20.0f;

			TextFormatDescription(std::wstring fontName = L"Arial",
								  float fontSize = 15.0f,
								  FontWeight fontWeight = FontWeight::FontWeightNormal,
								  FontStyle fontStyle = FontStyle::FontStyleNormal,
								  FontStretch fontStretch = FontStretch::FontStretchNormal,
								  TextAlignment textAlignment = TextAlignment::TextAlignmentLeft,
								  ParagraphAlignment paragraphAlignment = ParagraphAlignment::ParagraphAlignmentTop,
								  WrapMode wrapMode = WrapMode::WrapModeWrap,
								  FlowDirection flowDirection = FlowDirection::FlowDirectionTopToBottom,
								  ReadingDirection readingDirection = ReadingDirection::ReadingDirectionLeftToRight,
								  LineSpacingMethod lineSpacingMethod = LineSpacingMethod::LineSpacingMethodProportional,
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
			// -- GBGraphics::fields -- //			
		private:
			GraphicsBox *const m_pControl;
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
					RenderType renderType = RenderType::RenderTypeDefault,
					PresentOption presentOption = PresentOption::PresentOptionWaitForDisplay,
					InterpolationMode interpolationMode = InterpolationMode::InterpolationModeLinear,
					TextFormatDescription defaultTextFormatDesc = TextFormatDescription())
					: renderType(renderType)
					, presentOption(presentOption)
					, interpolationMode(interpolationMode)
					, defaultTextFormatDesc(defaultTextFormatDesc)
				{
				}
			};


			// -- GBGraphics::contructor -- //
		private:
			GBGraphics(const GBGraphics& graphics) = delete;
			GBGraphics(GBGraphics&& graphics) = delete;
			GBGraphics(GraphicsBox *control, const ConStruct& config);
			~GBGraphics();


			// -- GBGraphics::methods -- //
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
				InterpolationMode interpolationMode = InterpolationMode::InterpolationModeLinear);
			void DrawBitmap(
				const G::Bitmap& bitmap,
				const float opacity = 1.0f,
				InterpolationMode interpolationMode = InterpolationMode::InterpolationModeLinear);
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


			// -- GBGraphics::properties -- //
		public:
			const unsigned int& Width;
			const unsigned int& Height;


			// -- GBGraphics::friends -- //
		public:
			friend class GraphicsBox;
		};
	public:
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
	private:
		ChildControl::EventsManager<GraphicsBox::Event> m_events;
		GBGraphics m_graphics;


		// -- constructors -- //
	private:
		GraphicsBox(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox(const GraphicsBox&& graphicsBox) = delete;
		GraphicsBox(ParentControl* parentControl, const ConStruct<GraphicsBox> &conStruct);
		~GraphicsBox();


		// -- operators -- //
	public:
		GraphicsBox& operator=(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox& operator=(const GraphicsBox&& graphicsBox) = delete;


		// -- methods -- //
	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(GraphicsBox::Event((GraphicsBox::Event::Type)event.type));
		}
	public:
		void Resize(unsigned int newWidth, unsigned int newHeight);


		// -- propetry fields -- //
	public:
		GBGraphics& Gfx;
		ChildControl::EventsManager<GraphicsBox::Event>& Events;

		// -- friends -- //
	public:
		friend class ControlCreator;
	};

	template <> struct ConStruct<GraphicsBox> : ConStruct<ChildControl>
	{
		GraphicsBox::GBGraphics::ConStruct graphics;

		ConStruct(ConStruct<ChildControl> windowControlConStruct = ConStruct<ChildControl>(),
			GraphicsBox::GBGraphics::ConStruct graphicsConStruct = GraphicsBox::GBGraphics::ConStruct())
			: ConStruct<ChildControl>::ConStruct(windowControlConStruct)
			, graphics(graphicsConStruct)
		{}
	};
}

#endif // !GRAPHICS_BOX_H

