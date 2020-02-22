#ifndef GRAPHICS_BOX_H
#define GRAPHICS_BOX_H

#include "WindowControl.h"


namespace G = Graphics;

namespace WinApiFramework
{
	class GraphicsBox : public WindowControl
	{
		// -- GraphicsBox::fields -- //
	public:
		class GBGraphics
		{
			// -- GBGraphics::fields -- //
		public:
			// configuration enums
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
		private:
			GraphicsBox *const m_pControl;
			unsigned int m_width = 0u, m_height = 0u;
			const RenderType m_renderType;
			const PresentOption m_presentOption;
			InterpolationMode m_interpolationMode;

			// directX 2D
			ID2D1Factory *m_pD2DFactory = nullptr;
			ID2D1HwndRenderTarget *m_pHwndRenderTarget = nullptr;
			ID2D1Brush **m_ppBrush = nullptr;
			ID2D1SolidColorBrush *m_pSolidBrush = nullptr;
			ID2D1LinearGradientBrush *m_pLinearGradientBrush = nullptr;
			ID2D1RadialGradientBrush *m_pRadialGradientBrush = nullptr;

			IDWriteFactory *m_pDWriteFactory = nullptr;
			IDWriteTextFormat *m_pTextFormat = nullptr;
		public:
			struct Configuration
			{
				RenderType renderType;
				PresentOption presentOption;
				InterpolationMode interpolationMode;

				Configuration(
					RenderType renderType = RenderType::RenderTypeDefault,
					PresentOption presentOption = PresentOption::PresentOptionWaitForDisplay,
					InterpolationMode interpolationMode = InterpolationMode::InterpolationModeLinear)
					: renderType(renderType)
					, presentOption(presentOption)
					, interpolationMode(interpolationMode)
				{
				}
				~Configuration()
				{
				}
			};

			struct TextFormat
			{
			public:
				enum TextAlignment
				{
					TextAlignmentLeft		= 0,
					TextAlignmentRight		= 1,
					TextAlignmentCenter		= 2,
					TextAlignmentJustified	= 3
				};
				enum ParagraphAlignment
				{
					ParagraphAlignmentTop		= 0,
					ParagraphAlignmentBottom	= 1,
					ParagraphAlignmentCenter	= 2
				};
				enum WrapMode
				{
					WrapModeWrap		= 0,
					WrapModeNoWrap		= 1,
					WrapModeWholeWorld	= 2
				};
				enum FontWeight
				{
					FontWeightThin,
					FontWeightExtraLight,
					FontWeightUltraLight,
					FontWeightLight,
					FontWeightSemiLight,
					FontWeightNormal,
					FontWeightMedium,
					FontWeightSemiBold,
					FontWeightBold,
					FontWeightExtraBold,
					FontWeightUltraBold,
					FontWeightHeavy,
					FontWeightUltraHeavy
				};
				enum FontStyle
				{
					FontStyleNormal,
					FontStyleItalic,
					FontStyleOblique
				};
				enum FontStretch
				{
					FontStretchUptraCondensed,
					FontStretchExtraCondensed,
					FontStretchCondensed,
					FontStretchSemiCondensed,
					FontStretchNormal,
					FontStretchSemiExpanded,
					FontStretchExpanded,
					FontStretchExtraExpanded,
					FontStretchUltraExpanded
				};

			private:
				TextAlignment textAlignment;
				ParagraphAlignment paragraphAlignment;
				WrapMode wrapMode;
				FontWeight fontWeight;
				FontStyle fontStyle;
				FontStretch fontStretch;
				float fontSize;

			private:
				friend class GraphicsBox::GBGraphics;
			};


			// -- GBGraphics::contructor -- //
		private:
			GBGraphics(const GBGraphics& graphics) = delete;
			GBGraphics(GBGraphics&& graphics) = delete;
			GBGraphics(GraphicsBox *control, const Configuration& config);
			~GBGraphics();


			// -- GBGraphics::methods -- //
		private:
			bool InitGraphics();
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
			void SetInterpolationMode(InterpolationMode newInterpolationMode);

			// brush methods
			void ChangeActiveBrush(BrushType newBrushType);
			void SetSolidBrush(const G::Color& brushColor, const float opacity = 1.0f);

			void BeginDraw();
			void EndDraw();
			void Clear(const G::Color& color);

			// drawing text
			void DrawString(std::wstring text, G::Rect<float> rect);

			void DrawBitmap(
				const G::Bitmap& bitmap,
				const G::Rect<float>& destinationRect,
				const G::Rect<float>& sourceRect,
				const float opacity = 1.0f,
				GBGraphics::InterpolationMode interpolationMode = GBGraphics::InterpolationModeLinear);
			void DrawBitmap(
				const G::Bitmap& bitmap,
				const float opacity = 1.0f,
				GBGraphics::InterpolationMode interpolationMode = GBGraphics::InterpolationModeLinear);
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
		struct Config : public WindowControl::Config
		{
			GBGraphics::Configuration graphicsConfiguration;

			Config()
			{
			}
			~Config()
			{
			}
		};
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
		WindowControl::EventsManager<GraphicsBox::Event> events;
		GBGraphics graphics;


		// -- constructors -- //
	public:
		GraphicsBox(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox(const GraphicsBox&& graphicsBox) = delete;
		GraphicsBox(const Config &config);
		~GraphicsBox();


		// -- operators -- //
	public:
		GraphicsBox& operator=(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox& operator=(const GraphicsBox&& graphicsBox) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(GraphicsBox::Event((GraphicsBox::Event::Type)event.type));
		}
	public:
		void Resize(unsigned int newWidth, unsigned int newHeight);


		// -- propetry fields -- //
	public:
		GBGraphics& Gfx;
		WindowControl::EventsManager<GraphicsBox::Event>& Events;
	};
}

#endif // !GRAPHICS_BOX_H

